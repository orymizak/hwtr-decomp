#!/usr/bin/env python3
"""Export HWTR .CAR/.OBJ model geometry to Wavefront OBJ (+ JSON for the viewer).

Faithful port of the game's own mesh path:

* Mesh_FillVtx (0x80079BD4) builds the 16-byte Vtx buffer bound to segment 3.
  Material +0x2C is a group table of u16[4] = {end, pos_base, uv_base,
  colour_base}; material +0x28 holds three index bytes per vertex (position,
  uv, colour) that are added to the current group's bases.  Positions are
  s16[3] at +0x1C, texture coordinates s16[2] at +0x20 and colours u16 at +0x24
  (RGB555 with bit 0 clear, or RGB444 + flag with bit 0 set).
* Mesh_BuildDisplayList (0x80079140) interprets the command stream at +0x18.
  Opcode B loads count vertices from segment-3 byte offset into cache slot 0,
  opcodes 0/1/2 emit triangles from cache slots (2 is relative to the previous
  triangle's slots), opcode 3 selects a texture and 4 sets the texture scale.
  Other opcodes only change render state.

Parts form first-child/next-sibling lists; each part's 3x4 matrix is applied
after its parent's (Mtx34_Mul(parent, local)).  The tool refuses input it
cannot decode exactly instead of emitting a plausible-but-wrong mesh.
"""
from __future__ import annotations

import argparse
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from car_inspector import inspect  # noqa: E402

MATERIAL_SIZE = 0x34


def u16(d: bytes, o: int) -> int: return struct.unpack_from(">H", d, o)[0]
def s16(d: bytes, o: int) -> int: return struct.unpack_from(">h", d, o)[0]
def u32(d: bytes, o: int) -> int: return struct.unpack_from(">I", d, o)[0]


def colour(value: int) -> tuple[int, int, int]:
    """RGB of a Mesh_FillVtx colour, ignoring the runtime brightness shift."""
    if value & 1 == 0:
        return (((value >> 8) & 0xF8) + 4, ((value >> 3) & 0xF8) + 4, ((value << 2) & 0xF8) + 4)
    return (((value >> 8) & 0xF0) + 8, ((value >> 4) & 0xF0) + 8, (value & 0xF0) + 8)


def fill_vtx(d: bytes, m: int) -> list[dict]:
    count = u16(d, m + 6)
    pos_arr, uv_arr, col_arr = (m + u32(d, m + f) for f in (0x1C, 0x20, 0x24))
    idx_arr, groups = m + u32(d, m + 0x28), m + u32(d, m + 0x2C)
    out: list[dict] = []
    g = groups
    while len(out) < count:
        end, pb, ub, cb = (u16(d, g + k * 2) for k in range(4))
        if end <= len(out) and end != count:
            if g - groups > 0x10000: raise ValueError("tabla de grupos de vértices sin fin")
        while len(out) < end:
            i = len(out)
            bp, bu, bc = d[idx_arr + i * 3: idx_arr + i * 3 + 3]
            p = pos_arr + (bp + pb) * 6
            t = uv_arr + (bu + ub) * 4
            c = col_arr + (bc + cb) * 2
            if max(p + 6, t + 4, c + 2) > len(d): raise ValueError(f"vértice {i} fuera del archivo")
            out.append({"pos": (s16(d, p), s16(d, p + 2), s16(d, p + 4)),
                        "st": (s16(d, t), s16(d, t + 2)), "rgb": colour(u16(d, c))})
        g += 8
    return out


def build_triangles(d: bytes, m: int, vertex_count: int) -> list[dict]:
    """Return [{"v": (a, b, c), "texture": t, "scale": (s, t)}] in buffer indices."""
    cmds = u16(d, m + 2)
    pos = m + u32(d, m + 0x18)
    cache: list[int | None] = [None] * 64
    prev = (0, 0, 0)
    have_prev = False
    texture, scale, prim = None, (0xFFFF, 0xFFFF), (255, 255, 255, 255)
    mode, combine = 0, 0  # opcode 5 (render mode) and 6 (combiner) selectors
    tris: list[dict] = []

    def emit(slots: tuple[int, int, int]) -> None:
        for s in slots:
            if s >= 64 or cache[s] is None:
                raise ValueError(f"triángulo usa la caché {s} sin cargar (offset {pos:#x})")
        tris.append({"v": tuple(cache[s] for s in slots), "texture": texture, "scale": scale, "prim": prim, "mode": mode, "combine": combine})

    for _ in range(cmds):
        if pos >= len(d): raise ValueError("stream truncado")
        b0 = d[pos]; op, low = b0 >> 4, b0 & 15
        if op == 0:
            prev = tuple(d[pos + 1: pos + 4]); emit(prev); have_prev = True; pos += 4
        elif op == 1:
            prev = (low, d[pos + 1] >> 4, d[pos + 1] & 15); emit(prev); have_prev = True; pos += 2
        elif op == 2:
            if not have_prev: raise ValueError("triángulo relativo sin triángulo anterior")
            prev = (prev[0], prev[1] + 1 + ((b0 >> 2) & 3), prev[2] + 1 + (b0 & 3)); emit(prev); pos += 1
        elif op == 3:
            texture = low + d[pos + 1] * 16; pos += 2
        elif op == 4:
            # G_TEXTURE: the low nibble only switches texturing on; the scale always applies.
            scale = ((d[pos + 1] * 0x200 - 1) & 0xFFFF, (d[pos + 2] * 0x200 - 1) & 0xFFFF)
            pos += 3
        elif op == 7:  # G_SETPRIMCOLOR r, g, b, alpha = low nibble * 16 + 15 (unfaded)
            prim = tuple(d[pos + 1: pos + 4]) + ((low << 4) | 15,); pos += 4
        elif op in (8, 9): pos += 2
        elif op == 5:  # render mode: 0 opaque, 1 decal (ZMODE_DEC), 2 other
            mode = low; pos += 1
        elif op == 6:  # colour combiner preset
            combine = low; pos += 1
        elif op in (10, 12): pos += 1
        elif op == 11:
            n = d[pos + 1] + 1
            start = (d[pos + 2] + d[pos + 3] * 256) * 16 // 16
            if start + n > vertex_count: raise ValueError("carga de vértices fuera del buffer")
            for k in range(n): cache[k] = start + k
            pos += 4
        else:
            raise ValueError(f"opcode {op:X} desconocido en {pos:#x}")
    return tris


def mat_mul(a: list, b: list) -> list:
    """3x4 affine product a*b (rows of [r0 r1 r2 t])."""
    out = []
    for r in range(3):
        row = [sum(a[r][k] * b[k][c] for k in range(3)) for c in range(4)]
        row[3] += a[r][3]
        out.append(row)
    return out


def part_world_matrices(report: dict) -> dict[int, list]:
    parts = report["parts"]
    local = {p["index"]: [p["matrix"][r] + [p["translation"][r]] for r in range(3)] for p in parts}
    world: dict[int, list] = {}

    def resolve(i: int, depth: int = 0) -> list:
        if i in world: return world[i]
        if depth > len(parts): raise ValueError("jerarquía de piezas cíclica")
        parent = parts[i]["parent_index"]
        world[i] = local[i] if parent is None else mat_mul(resolve(parent, depth + 1), local[i])
        return world[i]

    for p in parts: resolve(p["index"])
    return world


def export_model(source: Path) -> dict:
    d = source.read_bytes()
    report = inspect(source)
    if report["problems"]:
        raise ValueError("; ".join(report["problems"][:3]))
    mat_base = u32(d, 0x30)
    textures = report["textures"]
    world = part_world_matrices(report)
    materials: dict[int, tuple[list, list]] = {}
    for i in range(u16(d, 2)):
        m = mat_base + i * MATERIAL_SIZE
        vtx = fill_vtx(d, m)
        materials[i] = (vtx, build_triangles(d, m, len(vtx)))
    meshes = []
    users = [p for p in report["parts"] if p["material_index"] is not None]
    if not users:  # Small UI OBJs have no part table entries; draw materials untransformed.
        users = [{"index": i, "material_index": i} for i in materials]
        world = {i: [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0]] for i in materials}
    for part in users:
        vtx, tris = materials[part["material_index"]]
        mtx = world[part["index"]]
        pos = [[round(sum(mtx[r][k] * v["pos"][k] for k in range(3)) + mtx[r][3], 4) for r in range(3)] for v in vtx]
        uv = []
        for t in tris:
            tex = textures[t["texture"]] if t["texture"] is not None and t["texture"] < len(textures) else None
            w, h = (tex["width"], tex["height"]) if tex else (32, 32)
            # S,T are s10.5 texels scaled by G_TEXTURE (0xFFFF = 1.0).  The RDP puts texel centres on
            # integer coordinates and GL at +0.5, hence the half-texel shift.
            uv.append([[(vtx[k]["st"][0] / 32 * t["scale"][0] / 65536 + 0.5) / max(w, 1),
                        (vtx[k]["st"][1] / 32 * t["scale"][1] / 65536 + 0.5) / max(h, 1)] for k in t["v"]])
        meshes.append({"part": part["index"], "material": part["material_index"],
                       "positions": pos, "colors": [list(v["rgb"]) for v in vtx],
                       "triangles": [list(t["v"]) for t in tris],
                       "textures": [t["texture"] for t in tris], "prims": [list(t["prim"]) for t in tris], "modes": [[t["mode"], t["combine"]] for t in tris], "uvs": uv,
                       "parent": part.get("parent_index"), "radius": part.get("radius"),
                       "origin": [mtx[r][3] for r in range(3)]})
    return {"name": source.stem, "kind": source.suffix.upper().lstrip("."), "meshes": meshes,
            "textures": [{k: t[k] for k in ("index", "width", "height", "format_code", "flags")} for t in textures]}


def rgba5551(v: int) -> bytes:
    return bytes((((v >> 11) & 31) * 255 // 31, ((v >> 6) & 31) * 255 // 31, ((v >> 1) & 31) * 255 // 31, 255 if v & 1 else 0))


def decode_texture(d: bytes, tex: dict) -> bytes | None:
    """RGBA8888 pixels following func_8007F61C's format codes; None if not in the file."""
    w, h, fmt, ptr = tex["width"], tex["height"], tex["format_code"], int(tex["pixels_offset"], 16)
    if ptr == 0:
        return None  # Filled at runtime.
    n = w * h
    if fmt == 0:  # RGBA16
        return b"".join(rgba5551(u16(d, ptr + i * 2)) for i in range(n))
    if fmt == 1:  # I8
        return b"".join(bytes((v, v, v, 255)) for v in d[ptr: ptr + n])
    if fmt == 3:  # RGBA32
        return d[ptr: ptr + n * 4]
    if fmt == 4:  # CI8: u16 colour count, palette at +8, then indices
        colours = s16(d, ptr)
        pal = [rgba5551(u16(d, ptr + 8 + i * 2)) for i in range(colours)]
        start = ptr + 8 + colours * 2
        return b"".join(pal[i] if i < colours else b"\0\0\0\0" for i in d[start: start + n])
    raise ValueError(f"formato de textura {fmt} desconocido")


def write_textures(source: Path, model: dict, out: Path) -> list[str]:
    import subprocess
    d = source.read_bytes()
    names = []
    for tex in inspect(source)["textures"]:
        rgba = decode_texture(d, tex)
        if rgba is None:
            names.append(""); continue
        name = f"{source.stem}.tex{tex['index']:02d}.png"
        subprocess.run(["magick", "-size", f"{tex['width']}x{tex['height']}", "-depth", "8", "rgba:-", str(out / name)],
                       input=rgba, check=True)
        names.append(name)
    return names


def write_mtl(model: dict, names: list[str], path: Path) -> None:
    lines = ["newmtl untextured", "Kd 1 1 1"]
    for i, name in enumerate(names):
        lines += [f"newmtl tex{i:02d}", "Kd 1 1 1"] + ([f"map_Kd {name}"] if name else [])
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_obj(model: dict, path: Path) -> tuple[int, int]:
    lines = [f"# {model['name']}.{model['kind']} — exportado con tools/model_mesh_export.py",
             "# Coordenadas del juego (pulgadas): X derecha, Y adelante, Z arriba",
             f"mtllib {path.stem}.mtl"]
    base = vt_base = 0
    nv = nf = 0
    # CAR roots are body LODs with their wheels as children; the OBJ keeps the
    # most detailed one (the JSON keeps them all).
    first = model["meshes"][0]["part"] if model["meshes"] else None
    lod0 = [m for m in model["meshes"] if m["part"] == first or m.get("parent") == first]
    for mesh in lod0:
        lines.append(f"o part{mesh['part']:02d}_mat{mesh['material']:02d}")
        for p, c in zip(mesh["positions"], mesh["colors"]):
            lines.append("v {} {} {} {:.3f} {:.3f} {:.3f}".format(*p, *(x / 255 for x in c)))
        current = object()
        for tri, tex, uv in zip(mesh["triangles"], mesh["textures"], mesh["uvs"]):
            if tex != current:
                lines.append(f"usemtl tex{tex:02d}" if tex is not None else "usemtl untextured"); current = tex
            for u, v in uv: lines.append(f"vt {u:.5f} {1 - v:.5f}")
            a, b, c = (base + i + 1 for i in tri)
            lines.append(f"f {a}/{vt_base + 1} {b}/{vt_base + 2} {c}/{vt_base + 3}")
            vt_base += 3
        base += len(mesh["positions"]); nv += len(mesh["positions"]); nf += len(mesh["triangles"])
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return nv, nf


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("sources", type=Path, nargs="+", help=".CAR/.OBJ descomprimidos")
    p.add_argument("--out", type=Path, required=True, help="carpeta de salida (OBJ + JSON)")
    a = p.parse_args()
    failed = 0
    for src in a.sources:
        try:
            model = export_model(src)
            obj_path = a.out / f"{src.stem}.{model['kind'].lower()}.obj"
            nv, nf = write_obj(model, obj_path)
            model["texture_files"] = write_textures(src, model, a.out)
            write_mtl(model, model["texture_files"], obj_path.with_suffix(".mtl"))
            (a.out / f"{src.stem}.{model['kind'].lower()}.json").write_text(json.dumps(model, separators=(",", ":")))
            print(f"{src.name}: {len(model['meshes'])} piezas, {nv} vértices, {nf} triángulos")
        except (OSError, ValueError, struct.error, IndexError) as e:
            failed += 1
            print(f"{src.name}: ERROR {e}", file=sys.stderr)
    if failed: raise SystemExit(f"{failed} archivo(s) con error")


if __name__ == "__main__":
    main()
