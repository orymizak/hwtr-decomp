#!/usr/bin/env python3
"""Build a standalone 3D viewer for HWTR cars.

Reads every decompressed .CAR and its .CWH (handling data) from assets/files and
writes one HTML page with the data embedded.  Geometry, UVs and textures come
from tools/model_mesh_export.py (a port of the game's Mesh_FillVtx and
Mesh_BuildDisplayList); each body LOD is a root part and its wheels are child
parts, so wheel spin, steering, braking and reverse can be watched on the real
model.

Game coordinates: inches, X right, Y forward (rear axle at Y=0), Z up.
"""
from __future__ import annotations

import argparse
import base64
import hashlib
import json
import subprocess
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from car_inspector import inspect  # noqa: E402
from model_mesh_export import decode_texture, export_model  # noqa: E402
from sound_export import decode_vadpcm, read_bank, read_effects  # noqa: E402

# Game sound events → BFX effect numbers (gSfxTable filled by Audio_Init; engines use the effect
# numbers of CarSnd_Init directly; RaceAudio_OnContact surface 2).
SOUND_EVENTS = {"turbo": 34, "no_turbo": 128, "crash": 57, "trick_turbo": 121, "trick": 122,
                "impact_light": 81, "impact_medium": 83, "impact_heavy": 85, "land": 26, "roll": 31}
# gSongTable (0x800EFC78, filled by Audio_Init): the game's music tracks as BFX effects.
SONG_EFFECTS = [53, 49, 51, 50, 48, 47, 45]
# CarSnd_Init: engine type (CWH flags top byte) → (on-throttle effect, off-throttle effect).
ENGINE_EFFECTS = {0: (4, 3), 1: (6, 5), 2: (8, 7), 3: (10, 9), 4: (12, 11), 5: (14, 13), 6: (16, 15),
                  7: (18, 17), 8: (20, 19), 9: (2, 1), 10: (21, 22)}

ROOT = Path(__file__).resolve().parent.parent
WHEEL_RADIUS_LIMIT = 60.0  # body parts have radii above 150; wheels are 10-24

# CWH field = car struct offset - 0x4C4 (see MECHANICS.md).
CWH_FIELDS = {
    "max_steer": 0x4CC,
    "mass": 0x4D0,
    "thrust_g": 0x4DC,
    "thrust_split": 0x4E0,
    "drag_cd": 0x4E4,
    "air_flip": 0x528,       # Car_AirControl: stick Y
    "air_yaw": 0x52C,        # Car_AirControl: stick X with button +0x24
    "air_roll": 0x530,       # Car_AirControl: stick X
    "crash_resistance": 0x53C,  # Car_CheckCrashContact / Car_CheckCrashPair
    "front_grip": 0x4FC,     # not simulated by the viewer; edited for the CWH export
    "rear_grip": 0x514,
    "ai_pace": 0x534,
}

ROM_MAIN = 0x80060400 - 0x1000  # vram - ROM offset of the main segment


def rom_tables(rom: Path, assets: Path) -> dict | None:
    """Trick tables read by Trick_Evaluate/Trick_GetName, and Tuning.prm."""
    if not rom.exists():
        return None
    data = rom.read_bytes()
    at = lambda vram: vram - ROM_MAIN  # noqa: E731
    text = (assets / "ENGLISH.HWT").read_bytes().decode("latin1").split("\r\n")
    names = []
    for i in range(49):  # D_800F316C: u8 count, then s32 text ids
        o = at(0x800F316C) + i * 0x1C
        names.append([text[w] for w in struct.unpack_from(f">{data[o]}i", data, o + 4)])
    return {
        "priority": list(data[at(0x800F3040): at(0x800F3040) + 49]),
        "turbos": list(data[at(0x800F3074): at(0x800F3074) + 49]),
        "points": list(struct.unpack_from(">49i", data, at(0x800F30A8))),
        "names": names,
        "tuning": list((assets / "TUNING.PRM").read_bytes()),
    }


def car_lights(data: bytes, report: dict) -> dict:
    """light_fans (headlight beams, 5 Vtx) and light_strips (exhaust flames, 7 Vtx)."""
    out = {}
    for name, size, count in (("light_fans", 0x50, 5), ("light_strips", 0x70, 7)):
        block = report["car"]["blocks"][name]
        items = []
        for i in range(block["count"] or 0):
            base = int(block["offset"], 16) + i * size
            vtx = [struct.unpack_from(">hhh6xBBBB", data, base + k * 16) for k in range(count)]
            items.append([[v[0], v[1], v[2], v[3], v[4], v[5], v[6]] for v in vtx])
        out[name] = items
    return out


def png_data_uri(rgba: bytes, width: int, height: int) -> str:
    png = subprocess.run(["magick", "-size", f"{width}x{height}", "-depth", "8", "rgba:-", "png:-"],
                         input=rgba, capture_output=True, check=True).stdout
    return "data:image/png;base64," + base64.b64encode(png).decode()


def mesh_groups(mesh: dict, textures: list) -> list:
    """Split a part into render groups of indexed triangles with normalised UVs;
    positions are relative to the part origin.

    Combiners (opcode 6): 0 PRIM*SHADE, 1 TEXEL0*SHADE, 2 PRIM*TEXEL0, 3 SHADE,
    4 TEXEL0 with PRIM alpha.  Opcode 5 mode 1 is ZMODE_DEC.  Texture 0 is the
    environment map (EnvMap_Apply), drawn with combiner 4 as a translucent decal
    pass over the painted base triangles.
    """
    groups: dict = {}
    origin = mesh["origin"]
    for tri, tex, uvs, prim, (mode, combine) in zip(mesh["triangles"], mesh["textures"], mesh["uvs"],
                                                    mesh["prims"], mesh["modes"]):
        uses_prim = tex is None or combine in (0, 2, 4)
        key = (tex, tuple(prim) if uses_prim else None, mode, combine)
        g = groups.setdefault(key, {"texture": tex, "color": prim if uses_prim else None, "mode": mode,
                                    "combine": combine, "pos": [], "uv": [], "idx": [], "map": {}})
        for v, uv in zip(tri, uvs):
            k = (v, round(uv[0], 4), round(uv[1], 4))
            if k not in g["map"]:
                g["map"][k] = len(g["pos"]) // 3
                g["pos"] += [round(mesh["positions"][v][r] - origin[r], 2) for r in range(3)]
                g["uv"] += [round(uv[0], 4), round(uv[1], 4)]
            g["idx"].append(g["map"][k])
    for g in groups.values():
        del g["map"]
    return list(groups.values())


def brake_lights(data: bytes, report: dict) -> dict | None:
    """CAR header byte 0x150 names the CI8 texture whose first 7 palette entries are the brake lights.
    Model_SwapPalette keeps them while braking and otherwise reduces every 5-bit channel to its top bit
    (almost black).  Returns the texture index and its 'lights off' image."""
    index = data[0x150]
    if not index or index >= len(report["textures"]):
        return None
    tex = report["textures"][index]
    if tex["format_code"] != 4:
        return None
    off = bytearray(data)
    pal = int(tex["pixels_offset"], 16) + 8
    for k in range(7):
        v = struct.unpack_from(">H", off, pal + k * 2)[0]
        r5, g5, b5 = v >> 11, (v >> 6) & 31, (v >> 1) & 31
        struct.pack_into(">H", off, pal + k * 2, ((r5 >> 4) << 11) | ((g5 >> 4) << 6) | ((b5 >> 4) << 1) | 1)
    return {"texture": index, "off": png_data_uri(decode_texture(bytes(off), tex), tex["width"], tex["height"])}


def car_geometry(car_path: Path, report: dict) -> dict:
    model = export_model(car_path)
    data = car_path.read_bytes()
    textures = []
    for tex in report["textures"]:
        rgba = decode_texture(data, tex)
        textures.append(png_data_uri(rgba, tex["width"], tex["height"]) if rgba else None)
    meshes = {m["part"]: m for m in model["meshes"]}
    lods = []
    for root in report["hierarchy"]["roots"]:
        if root not in meshes:
            continue
        wheels = [m for m in model["meshes"] if m["parent"] == root and m["radius"] < WHEEL_RADIUS_LIMIT]
        lods.append({
            "part": root,
            "body": mesh_groups(meshes[root], textures),
            "wheels": [{"part": w["part"], "pos": [round(v, 3) for v in w["origin"]], "radius": round(w["radius"], 3),
                        "groups": mesh_groups(w, textures)} for w in wheels],
        })
    return {"textures": textures, "lods": lods}


def car_entry(car_path: Path) -> dict:
    data = car_path.read_bytes()
    report = inspect(car_path)
    bbox = struct.unpack_from(">6f", data, 0x14)
    wheels = [p for p in report["parts"] if p["radius"] < WHEEL_RADIUS_LIMIT]
    # Wheels appear twice (two sets with their own materials); the first set is used.
    wheels = wheels[: len(wheels) // 2] or wheels
    entry = {
        "name": car_path.stem,
        "bbox_min": [round(v, 3) for v in bbox[:3]],
        "bbox_max": [round(v, 3) for v in bbox[3:]],
        "wheels": [
            {"part": w["index"], "pos": [round(v, 3) for v in w["translation"]], "radius": round(w["radius"], 3)}
            for w in wheels
        ],
    }
    entry.update(car_geometry(car_path, report))
    entry.update(car_lights(data, report))
    entry["brake_lights"] = brake_lights(data, report)
    cwh_path = car_path.with_suffix(".CWH")
    if cwh_path.exists():
        cwh = cwh_path.read_bytes()
        # The garage exports an asset_workbench CWH document from these.
        entry["cwh"] = {"raw_hex": cwh.hex(), "sha1": hashlib.sha1(cwh).hexdigest(), "size": len(cwh)}
        # +0x4C8..+0x4CB: front driven, front steered, rear driven, rear steered (Car_ComputeMass wheel flags).
        entry["engine_type"] = cwh[0x7C]                   # CWH flags top byte (CarSnd_Init)
        # Garage bars (GarageCar_LoadP1 / Garage_DrawStatBars): rows TOP SPEED, STUNTS, DURABILITY use
        # bytes 0x114, 0x116, 0x115 (value / 5 segments); CONTROL is a 0-3 level in byte 0x117.
        entry["game_stats"] = {"top_speed": cwh[0x114], "stunts": cwh[0x116], "durability": cwh[0x115], "control": cwh[0x117]}
        entry["axles"] = {"front_drive": cwh[4], "front_steer": cwh[5], "rear_drive": cwh[6], "rear_steer": cwh[7]}
        entry["handling"] = {
            name: round(struct.unpack_from(">f", cwh, field - 0x4C4)[0], 4) for name, field in CWH_FIELDS.items()
        }
    return entry


def effect_sprites(assets: Path) -> list:
    """EFFECTS.TEX: 30 entries of {w, h, format (0 RGBA5551, 3 RGBA32), flags, u32, pixels}, 8-aligned."""
    path = assets / "EFFECTS.TEX"
    if not path.exists():
        return []
    data, pos, out = path.read_bytes(), 0, []
    for _ in range(30):
        w, h, fmt = data[pos], data[pos + 1], data[pos + 2]
        start = pos + 8
        if fmt == 3:
            rgba, size = data[start: start + w * h * 4], w * h * 4
        else:
            size = w * h * 2
            rgba = b"".join(bytes((((v >> 11) & 31) * 255 // 31, ((v >> 6) & 31) * 255 // 31, ((v >> 1) & 31) * 255 // 31,
                                   255 if v & 1 else 0)) for v in struct.unpack_from(f">{w * h}H", data, start))
        out.append(png_data_uri(rgba, w, h))
        pos = (start + size + 7) & ~7
    return out


def sound_bundle(assets: Path, rate: int = 16000) -> dict | None:
    """The effects the viewer plays, with their samples as WAV data URIs at each sample's natural rate."""
    import io
    import wave
    names = ("CCCSND.PTR", "CCCSND.WBK", "CCCSND.BFX")
    if not all((assets / n).exists() for n in names):
        return None
    bank = read_bank((assets / "CCCSND.PTR").read_bytes())
    wbk = (assets / "CCCSND.WBK").read_bytes()
    effects = read_effects((assets / "CCCSND.BFX").read_bytes())
    wanted = set(SOUND_EVENTS.values()) | {e for pair in ENGINE_EFFECTS.values() for e in pair}
    out_fx, out_samples = {}, {}
    music = [{"name": f"Track {n + 1}", "sample": effects[e]["sample"]} for n, e in enumerate(SONG_EFFECTS)]
    songs = {m["sample"] for m in music}
    # Other half-rate (music) samples: in the bank but not in the game's song table.
    music += [{"name": f"Not in the song table {n + 1} (sample {k})", "sample": k}
              for n, k in enumerate(i for i, e in enumerate(bank) if e["basenote"] == -12 and i not in songs)]
    wanted_samples = {m["sample"] for m in music}
    for i in sorted(wanted):
        fx = effects[i]
        if "sample" not in fx:
            continue
        out_fx[i] = {"sample": fx["sample"], "note": fx.get("note", 48), "length": fx.get("length"), "volume": fx.get("volume", 127)}
        wanted_samples.add(fx["sample"])
    for k in sorted(wanted_samples):
        e = bank[k]
        pcm = decode_vadpcm(wbk[e["base"]: e["base"] + e["length"]], e["book"], e["order"])
        natural = round(rate * 2 ** ((e["basenote"] * 100 + e["detune_cents"]) / 1200))
        buf = io.BytesIO()
        with wave.open(buf, "wb") as w:
            w.setnchannels(1); w.setsampwidth(2); w.setframerate(natural)
            w.writeframes(struct.pack(f"<{len(pcm)}h", *pcm))
        loop = e.get("loop")
        out_samples[k] = {"uri": "data:audio/wav;base64," + base64.b64encode(buf.getvalue()).decode(), "rate": natural,
                          "loop": [loop["start"], loop["end"]] if loop and loop["count"] else None}
    return {"events": SOUND_EVENTS, "engines": ENGINE_EFFECTS, "effects": out_fx, "samples": out_samples, "music": music}


# Linear models of the garage bars from the handling floats, fitted on the game's 41 cars.  The bars are
# display-only in the game (nothing outside the garage reads bytes 0x114-0x117), so these only estimate
# how a bar would move after editing the handling.  Each bar only takes the fields that produce that
# behaviour in the game's code, so a correlated but unrelated field (e.g. drag for stunts) cannot move it:
#   stunts     <- the air-control coefficients (Car_AirControl)
#   durability <- crash resistance (Car_CheckCrashContact / Car_CheckCrashPair)
#   control    <- grips and maximum steering
# TOP SPEED is not fitted: in the game's data it does not follow thrust or drag (R² ≈ 0), so the viewer
# derives it from its own top speed, which scales with √(thrust / Cd).
BAR_FEATURES = {
    "durability": ["crash_resistance"],
    "stunts": ["air_roll", "air_flip", "air_yaw"],
    "control": ["front_grip", "rear_grip", "max_steer"],
}
BAR_SOURCE = {"top_speed": 0x114, "durability": 0x115, "stunts": 0x116, "control": 0x117}
HANDLING_OFFSETS = {"max_steer": 0x8, "mass": 0xC, "thrust_g": 0x18, "thrust_split": 0x1C, "drag_cd": 0x20,
                    "front_grip": 0x38, "rear_grip": 0x50, "air_flip": 0x64, "air_yaw": 0x68, "air_roll": 0x6C,
                    "ai_pace": 0x70, "crash_resistance": 0x78}


def least_squares(rows: list[list[float]], ys: list[float]) -> list[float]:
    """Ordinary least squares with an intercept (normal equations, Gaussian elimination)."""
    xs = [[1.0] + r for r in rows]
    n = len(xs[0])
    a = [[sum(x[i] * x[j] for x in xs) + (1e-6 if i == j else 0) for j in range(n)] + [sum(x[i] * y for x, y in zip(xs, ys))] for i in range(n)]
    for c in range(n):
        piv = max(range(c, n), key=lambda r: abs(a[r][c]))
        a[c], a[piv] = a[piv], a[c]
        for r in range(n):
            if r != c:
                f = a[r][c] / a[c][c]
                a[r] = [v - f * w for v, w in zip(a[r], a[c])]
    return [a[i][n] / a[i][i] for i in range(n)]


def bar_models(assets: Path) -> dict | None:
    files = sorted(assets.glob("*.CWH"))
    if not files:
        return None
    cars = []
    for f in files:
        d = f.read_bytes()
        h = {k: struct.unpack_from(">f", d, o)[0] for k, o in HANDLING_OFFSETS.items()}
        cars.append((h, {k: d[o] for k, o in BAR_SOURCE.items()}))
    out = {}
    for bar, feats in BAR_FEATURES.items():
        rows = [[h[f] for f in feats] for h, _ in cars]
        ys = [b[bar] for _, b in cars]
        coef = least_squares(rows, ys)
        pred = [coef[0] + sum(c * x for c, x in zip(coef[1:], r)) for r in rows]
        mean = sum(ys) / len(ys)
        r2 = 1 - sum((y - q) ** 2 for y, q in zip(ys, pred)) / max(1e-9, sum((y - mean) ** 2 for y in ys))
        out[bar] = {"features": feats, "intercept": coef[0], "weights": coef[1:], "r2": round(r2, 2)}
    return out


def envmap_uri(assets: Path) -> str | None:
    """ENVMAP1.TEX: 8-byte header + 32x32 RGBA16, loaded by EnvMap_LoadStatic."""
    path = assets / "ENVMAP1.TEX"
    if not path.exists():
        return None
    data = path.read_bytes()
    rgba = b"".join(bytes((((v >> 11) & 31) * 255 // 31, ((v >> 6) & 31) * 255 // 31, ((v >> 1) & 31) * 255 // 31, 255))
                    for v in struct.unpack_from(">1024H", data, 8))
    return png_data_uri(rgba, 32, 32)


def sky_panoramas(assets: Path) -> list:
    """.SKY: a tile table at 0x60 ({w, h, fmt, -, u32 offset} × 16; offsets from 0x5C) of 32×64 RGBA16
    tiles; on the desert and glacial tracks the first 8 make the 256×64 sky panorama, on the haunted
    ones the last 8 — the half whose top rows vary least is taken as the sky."""
    out = []
    for path in sorted(assets.glob("*.SKY")):
        d = path.read_bytes()
        halves = []
        for half in range(2):
            rgba, top = bytearray(256 * 64 * 4), []
            for t in range(8):
                e = 0x60 + (half * 8 + t) * 8
                w, h, off = d[e], d[e + 1], struct.unpack_from(">I", d, e + 4)[0] + 0x5C
                if (w, h) != (32, 64):
                    break
                for y in range(64):
                    for x in range(32):
                        v = struct.unpack_from(">H", d, off + (y * 32 + x) * 2)[0]
                        px = (((v >> 11) & 31) * 255 // 31, ((v >> 6) & 31) * 255 // 31, ((v >> 1) & 31) * 255 // 31)
                        i = (y * 256 + t * 32 + x) * 4
                        rgba[i: i + 4] = bytes((*px, 255))
                        if y < 6:
                            top.append(sum(px))
            mean = sum(top) / max(1, len(top))
            halves.append((sum((v - mean) ** 2 for v in top) / max(1, len(top)), rgba))
        if len(halves) == 2:
            out.append({"name": path.stem, "uri": png_data_uri(bytes(min(halves, key=lambda h: h[0])[1]), 256, 64)})
    return out


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--assets", type=Path, default=ROOT / "assets" / "files", help="folder with .CAR/.CWH files")
    parser.add_argument("--rom", type=Path, default=ROOT / "baserom.z64", help="ROM for the trick tables")
    parser.add_argument("--demo", action="store_true",
                        help="build a page with procedural demo cars and no game data (safe to publish)")
    parser.add_argument("--output", type=Path, default=ROOT / "build" / "car_viewer.html", help="HTML page to write")
    args = parser.parse_args()

    template = (Path(__file__).resolve().parent / "car_viewer.html").read_text()
    if args.demo:
        # Nothing from the game: the page builds its own cars, textures and tables.
        page = template.replace("const DEMO = /*DEMO*/false;", "const DEMO = true;")
        page = page.replace("<title>HWTR Car Viewer</title>", "<title>Car Physics Sandbox</title>")
        page = page.replace("<h1>HWTR Car Viewer</h1>", "<h1>Car Physics Sandbox</h1>")
        start, end = page.index("<!--SOURCES-->"), page.index("<!--/SOURCES-->") + len("<!--/SOURCES-->")
        page = page[:start] + (
            "<details><summary>About</summary>Unofficial fan project, not affiliated with any game publisher or toy "
            "maker. Cars, textures, effects and trick tables are generated procedurally in the page. The driving, "
            "air-control, trick and crash rules are a re-implementation of reverse-engineered mechanics.</details>"
        ) + page[end:]
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(page)
        print(f"demo page written to {args.output}")
        return

    cars = []
    for car_path in sorted(args.assets.glob("*.CAR")):
        try:
            cars.append(car_entry(car_path))
        except (OSError, ValueError, struct.error) as error:
            print(f"warning: skipping {car_path.name}: {error}", file=sys.stderr)
    if not cars:
        raise SystemExit(f"error: no .CAR files found in {args.assets}")

    page = template.replace("/*CAR_DATA*/[]", json.dumps(cars, separators=(",", ":")))
    page = page.replace("/*ENVMAP*/null", json.dumps(envmap_uri(args.assets)))
    page = page.replace("/*EFFECTS*/[]", json.dumps(effect_sprites(args.assets)))
    page = page.replace("/*GAME*/null", json.dumps(rom_tables(args.rom, args.assets), separators=(",", ":")))
    page = page.replace("/*SOUND*/null", json.dumps(sound_bundle(args.assets), separators=(",", ":")))
    page = page.replace("/*BARS*/null", json.dumps(bar_models(args.assets), separators=(",", ":")))
    page = page.replace("/*SKIES*/[]", json.dumps(sky_panoramas(args.assets), separators=(",", ":")))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(page)
    print(f"{len(cars)} cars written to {args.output}")


if __name__ == "__main__":
    main()
