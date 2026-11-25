#!/usr/bin/env python3
"""Apply a garage edit (car_viewer.html → Save → .car-edit.json) to new game files.

Writes NEW decompressed .CAR and .CWH files; the originals are never modified.

* Paint: every G_SETPRIMCOLOR (opcode 7) whose RGB matches an edited colour is
  rewritten in place in the material command streams.
* Textures: each edited PNG is re-encoded in the texture's own format
  (func_8007F61C: RGBA16, I8, RGBA32, CI8).  CI8 keeps its palette when the new
  colours fit it, otherwise the texture's own palette is rebuilt (same size);
  I8 is greyscale only.  Unchanged
  pixels keep their original bytes, so an unedited texture round-trips exactly.
* Stats: the handling fields are written as f32 at their .CWH offsets, and the garage bars
  (top speed, durability, stunts, control) as the bytes 0x114-0x117.
* Shape: not applied (a CAR geometry importer does not exist yet).

The .CAR is LZ-compressed in the ROM; putting the new file in a ROM still needs
an LZ compressor and file-table update, which are not written yet.  The .CWH is
stored raw (same size), so it can replace rom_files/<NAME>.CWH.bin directly in
a modded build.
"""
from __future__ import annotations

import argparse
import base64
import json
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from car_inspector import inspect  # noqa: E402
from model_mesh_export import decode_texture, rgba5551, u16, u32  # noqa: E402

MATERIAL_SIZE = 0x34
STEP = {0: 4, 1: 2, 2: 1, 3: 2, 4: 3, 5: 1, 6: 1, 7: 4, 8: 2, 9: 2, 10: 1, 11: 4, 12: 1}
CWH_OFFSETS = {  # handling key -> .CWH file offset (car struct offset - 0x4C4)
    "max_steer": 0x008, "mass": 0x00C, "thrust_g": 0x018, "thrust_split": 0x01C, "drag_cd": 0x020,
    "front_grip": 0x038, "rear_grip": 0x050, "air_flip": 0x064, "air_yaw": 0x068, "air_roll": 0x06C,
    "ai_pace": 0x070, "crash_resistance": 0x078,
}


def fail(message: str) -> None:
    raise SystemExit(f"error: {message}")


def png_rgba(uri: str, width: int, height: int) -> bytes:
    if not uri.startswith("data:image/png;base64,"):
        fail("texture is not a PNG data URI")
    png = base64.b64decode(uri.split(",", 1)[1])
    out = subprocess.run(["magick", "png:-", "-alpha", "on", "-depth", "8", "rgba:-"], input=png,
                         capture_output=True, check=True).stdout
    if len(out) != width * height * 4:
        fail(f"texture is not {width}x{height}")
    return out


def same(a: bytes, b: bytes) -> bool:
    """Equal pixels; fully transparent pixels match whatever their colour."""
    return a == b or (a[3] == 0 and b[3] == 0)


def encode_texture(data: bytearray, tex: dict, rgba: bytes) -> list[str]:
    warnings = []
    w, h, fmt, ptr = tex["width"], tex["height"], tex["format_code"], int(tex["pixels_offset"], 16)
    old = decode_texture(bytes(data), tex)
    n = w * h
    px = lambda buf, i: buf[i * 4: i * 4 + 4]  # noqa: E731
    if fmt == 0:  # RGBA16 (RGBA5551)
        for i in range(n):
            p = px(rgba, i)
            if same(p, px(old, i)):
                continue
            v = (round(p[0] * 31 / 255) << 11) | (round(p[1] * 31 / 255) << 6) | (round(p[2] * 31 / 255) << 1) | (p[3] >= 128)
            struct.pack_into(">H", data, ptr + i * 2, v)
    elif fmt == 1:  # I8
        for i in range(n):
            p = px(rgba, i)
            if not same(p, px(old, i)):
                if not (p[0] == p[1] == p[2]):
                    warnings.append("I8 texture is greyscale: colours were converted to grey")
                data[ptr + i] = round(0.299 * p[0] + 0.587 * p[1] + 0.114 * p[2])
    elif fmt == 3:  # RGBA32
        for i in range(n):
            if not same(px(rgba, i), px(old, i)):
                data[ptr + i * 4: ptr + i * 4 + 4] = px(rgba, i)
    elif fmt == 4:  # CI8: the palette lives with the texture, so it can be rebuilt in place
        colours = struct.unpack_from(">h", data, ptr)[0]
        pal = [rgba5551(u16(data, ptr + 8 + k * 2)) for k in range(colours)]
        start = ptr + 8 + colours * 2
        changed = [i for i in range(n) if not same(px(rgba, i), px(old, i))]
        if not changed:
            return warnings
        dist = lambda p, q: sum((p[c] - q[c]) ** 2 for c in range(4))  # noqa: E731
        worst = max(min(dist(px(rgba, i), q) for q in pal) for i in changed)
        if worst <= 3 * 16 ** 2:
            # The palette already has these colours: keep it, remap changed pixels.
            for i in changed:
                data[start + i] = min(range(colours), key=lambda k: dist(px(rgba, i), pal[k]))
            return warnings
        # New colours: quantise the whole texture to the same number of entries.
        img = subprocess.run(["magick", "-size", f"{w}x{h}", "-depth", "8", "rgba:-", "-dither", "None",
                              "-colors", str(colours), "-depth", "8", "rgba:-"], input=rgba, capture_output=True, check=True).stdout
        entries, index = [], {}
        for i in range(n):
            p = px(img, i)
            v = (round(p[0] * 31 / 255) << 11) | (round(p[1] * 31 / 255) << 6) | (round(p[2] * 31 / 255) << 1) | (p[3] >= 128)
            if v not in index:
                if len(entries) == colours:
                    v = min(entries, key=lambda e: dist(rgba5551(e), p))
                else:
                    index[v] = len(entries); entries.append(v)
            data[start + i] = index[v]
        entries += [0] * (colours - len(entries))
        for k, v in enumerate(entries):
            struct.pack_into(">H", data, ptr + 8 + k * 2, v)
        warnings.append(f"CI8 texture {tex['index']}: palette rebuilt ({len(index)} of {colours} colours)")
    else:
        fail(f"texture format {fmt} not supported")
    return warnings


def apply_paint(data: bytearray, paint: dict) -> int:
    swaps = {}
    for old, new in paint.items():
        a, b = bytes.fromhex(old.lstrip("#")), bytes.fromhex(new.lstrip("#"))
        if a != b:
            swaps[a] = b
    count = 0
    for m in range(u16(data, 2)):
        base = u32(data, 0x30) + m * MATERIAL_SIZE
        pos = base + u32(data, base + 0x18)
        for _ in range(u16(data, base + 2)):
            op = data[pos] >> 4
            if op == 7 and bytes(data[pos + 1: pos + 4]) in swaps:
                data[pos + 1: pos + 4] = swaps[bytes(data[pos + 1: pos + 4])]
                count += 1
            pos += STEP[op]
    return count


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("edit", type=Path, help=".car-edit.json saved from the garage")
    p.add_argument("car", type=Path, help="original decompressed .CAR (its .CWH is read from the same folder)")
    p.add_argument("out", type=Path, help="output folder for the new .CAR and .CWH")
    a = p.parse_args()
    edit = json.loads(a.edit.read_text())
    if edit.get("format") != "hwtr-car-edit-v1":
        fail("not a car edit file")
    if edit.get("demo"):
        fail("this edit is for a demo car; it has no game files")
    if edit.get("base", "").upper() != a.car.stem.upper():
        fail(f"edit is for {edit.get('base')}, not {a.car.stem}")
    a.out.mkdir(parents=True, exist_ok=True)
    out_car, out_cwh = a.out / a.car.name, a.out / a.car.with_suffix(".CWH").name
    if out_car.resolve() == a.car.resolve():
        fail("output must not replace the original")

    data = bytearray(a.car.read_bytes())
    report = inspect(a.car)
    print(f"paint: {apply_paint(data, edit.get('paint', {}))} prim colour commands changed")
    warnings = []
    for key, uri in edit.get("textures", {}).items():
        tex = report["textures"][int(key)]
        warnings += encode_texture(data, tex, png_rgba(uri, tex["width"], tex["height"]))
    print(f"textures: {len(edit.get('textures', {}))} re-encoded")
    out_car.write_bytes(data)
    inspect(out_car)  # the result must still parse

    cwh_in = a.car.with_suffix(".CWH")
    if cwh_in.exists() and edit.get("handling"):
        cwh = bytearray(cwh_in.read_bytes())
        for key, value in edit["handling"].items():
            if key in CWH_OFFSETS:
                struct.pack_into(">f", cwh, CWH_OFFSETS[key], float(value))
        for key, off in (("top_speed", 0x114), ("durability", 0x115), ("stunts", 0x116), ("control", 0x117)):
            if key in edit.get("game_stats", {}):
                cwh[off] = max(0, min(255 if key != "control" else 3, int(edit["game_stats"][key])))
        out_cwh.write_bytes(cwh)
        print(f"stats: written to {out_cwh}")
    shape = edit.get("shape", {})
    if any(abs(v - (0 if k == "ride" else 1)) > 1e-6 for k, v in shape.items()):
        warnings.append("shape edits are viewer-only and were not applied")
    for w in dict.fromkeys(warnings):
        print(f"warning: {w}")
    print(f"car: {out_car}")


if __name__ == "__main__":
    main()
