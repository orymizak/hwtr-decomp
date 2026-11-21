#!/usr/bin/env python3
"""Read-only structural inspection for HWTR N64 CAR/OBJ model assets.

This emits the parts, materials, texture descriptors, and CAR-only blocks as
the game's own loader sees them (WorldData_Relocate for the shared CAR/OBJ
container, ModelData_Load for the CAR extras).  It does not modify game data
and is the input contract for the in-progress 3D decoder.
"""
from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path


# The shared CAR/OBJ header fields end at 0x54.  Some small UI OBJ models are
# shorter than a CAR's 0x160-byte preamble, so only CARs require the full one.
HEADER_SIZE = 0x54
CAR_HEADER_SIZE = 0x160
PART_SIZE = 0x54
MATERIAL_SIZE = 0x34
TEXTURE_SIZE = 0x08
EXTRA_SIZE = 0x14
PALETTE_ANIM_SIZE = 0x08
NONE = 0xFFFFFFFF

# Per-material relative pointers fixed up by Mesh_Relocate (relative to the
# material record itself, not to the file).
MATERIAL_STREAMS = (0x18, 0x1C, 0x20, 0x24, 0x28, 0x2C)
MATERIAL_COLORS = 0x14

# CAR-only blocks fixed up by ModelData_Load: (header offset, name, count
# field, count size, entry size, what the loader does with it).
CAR_BLOCKS = (
    (0x110, "light_fans", 0xFF, 1, 0x50,
     "5 vertices each; a G_VTX + 2 triangles display list per entry, one point light allocated"),
    (0x114, "light_strips", 0x10C, 1, 0x70,
     "7 vertices each; a G_VTX + 3 triangle-pair display list per entry"),
    (0x118, "shadow_quad", None, 0, 0x40,
     "4 vertices; UVs rebuilt from texture 1's size at load time"),
    (0x12C, "light_fan_dl_buffer", 0xFF, 1, None,
     "runtime scratch: count*0xC headers + count*0x40 display lists"),
    (0x130, "light_strip_dl_buffer", 0x10C, 1, None,
     "runtime scratch: count*0xC headers + count*0x48 display lists"),
    (0x134, "shadow_dl", None, 0, None, "runtime display list written at load time"),
    (0x138, "dl_138", None, 0, None, "relocated; written at load time"),
    (0x108, "block_108", None, 0, None, "relocated; purpose unknown"),
    (0x128, "block_128", None, 0, None, "relocated; purpose unknown"),
)


def u8(data: bytes, offset: int) -> int:
    return data[offset]


def u16(data: bytes, offset: int) -> int:
    return struct.unpack_from(">H", data, offset)[0]


def s16(data: bytes, offset: int) -> int:
    return struct.unpack_from(">h", data, offset)[0]


def u32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def f32(data: bytes, offset: int) -> float:
    return struct.unpack_from(">f", data, offset)[0]


def inside(value: int, data: bytes, size: int = 1) -> bool:
    return 0 <= value and value + size <= len(data)


def hexo(value: int) -> str:
    return f"0x{value:X}"


def index_or_none(value: int) -> int | None:
    return None if value == NONE else value


def read_parts(data: bytes, base: int, count: int, material_count: int, problems: list) -> list:
    parts = []
    for index in range(count):
        offset = base + index * PART_SIZE
        if not inside(offset, data, PART_SIZE):
            problems.append(f"part {index} at {hexo(offset)} runs past the end of the file")
            break
        # 3x4 row-major matrix: rotation/scale in columns 0-2, translation in column 3.
        rows = [[f32(data, offset + (r * 4 + c) * 4) for c in range(4)] for r in range(3)]
        material = index_or_none(u32(data, offset + 0x3C))
        nxt = index_or_none(u32(data, offset + 0x4C))
        child = index_or_none(u32(data, offset + 0x50))
        if material is not None and material >= material_count:
            problems.append(f"part {index}: material {material} out of range")
        for name, ref in (("next", nxt), ("child", child)):
            if ref is not None and ref >= count:
                problems.append(f"part {index}: {name} {ref} out of range")
        parts.append({
            "index": index,
            "offset": hexo(offset),
            "matrix": [row[:3] for row in rows],
            "translation": [row[3] for row in rows],
            "flags": hexo(u32(data, offset + 0x30)),
            "id": u32(data, offset + 0x34),
            "radius": f32(data, offset + 0x38),
            "material_index": material,
            "extra_index": u32(data, offset + 0x48),
            "next_index": nxt,
            "child_index": child,
        })
    return parts


def build_tree(parts: list) -> dict:
    """Parent links and roots from the first-child/next-sibling lists."""
    parent = {}
    for part in parts:
        child, seen = part["child_index"], set()
        while child is not None and child < len(parts) and child not in seen:
            seen.add(child)
            parent[child] = part["index"]
            child = parts[child]["next_index"]
    for part in parts:
        part["parent_index"] = parent.get(part["index"])
    roots, seen = [], set()
    for part in parts:
        if part["parent_index"] is None and part["index"] not in seen:
            node = part["index"]
            while node is not None and node < len(parts) and node not in seen:
                seen.add(node)
                roots.append(node)
                node = parts[node]["next_index"]
    return {"roots": roots}


def read_materials(data: bytes, base: int, count: int, problems: list) -> list:
    materials = []
    for index in range(count):
        offset = base + index * MATERIAL_SIZE
        if not inside(offset, data, MATERIAL_SIZE):
            problems.append(f"material {index} at {hexo(offset)} runs past the end of the file")
            break
        streams = []
        for field in MATERIAL_STREAMS:
            rel = u32(data, offset + field)
            target = offset + rel
            if not inside(target, data):
                problems.append(f"material {index}: stream +{hexo(field)} points outside the file")
            streams.append({"field": hexo(field), "relative": hexo(rel), "offset": hexo(target),
                            "in_file": inside(target, data)})
        colors = u32(data, offset + MATERIAL_COLORS)
        color_info = None
        if colors:
            target = offset + colors
            if inside(target, data, 4):
                color_info = {"offset": hexo(target), "count": u16(data, target)}
            else:
                problems.append(f"material {index}: color table points outside the file")
        materials.append({
            "index": index,
            "offset": hexo(offset),
            "magic": hexo(u16(data, offset)),
            "count_02": u16(data, offset + 2),
            "colors": color_info,
            "streams": streams,
            "fields": [f"0x{u32(data, offset + field):08X}" for field in range(0, MATERIAL_SIZE, 4)],
        })
    return materials


def read_textures(data: bytes, base: int, count: int, problems: list) -> list:
    textures = []
    for index in range(count):
        offset = base + index * TEXTURE_SIZE
        if not inside(offset, data, TEXTURE_SIZE):
            problems.append(f"texture {index} at {hexo(offset)} runs past the end of the file")
            break
        descriptor, pixels = u32(data, offset), u32(data, offset + 4)
        if not inside(pixels, data):
            problems.append(f"texture {index}: pixels point outside the file")
        textures.append({
            "index": index,
            "offset": hexo(offset),
            "width": descriptor >> 24,
            "height": (descriptor >> 16) & 0xFF,
            "format_code": (descriptor >> 8) & 0xFF,
            "flags": descriptor & 0xFF,
            "pixels_offset": hexo(pixels),
            "pixels_in_file": inside(pixels, data),
        })
    return textures


def read_palette_anims(data: bytes, base: int, count: int, texture_count: int, problems: list) -> list:
    anims = []
    for index in range(count):
        offset = base + index * PALETTE_ANIM_SIZE
        if not inside(offset, data, PALETTE_ANIM_SIZE):
            problems.append(f"palette animation {index} runs past the end of the file")
            break
        texture = u16(data, offset)
        if texture >= texture_count:
            problems.append(f"palette animation {index}: texture {texture} out of range")
        anims.append({
            "index": index,
            "texture_index": texture,
            "first_color": u8(data, offset + 2),
            "last_color": u8(data, offset + 3),
            "speed": s16(data, offset + 4),
            "time": u16(data, offset + 6),
        })
    return anims


def read_car_blocks(data: bytes, problems: list) -> dict:
    blocks = {}
    for field, name, count_field, count_size, entry_size, note in CAR_BLOCKS:
        ptr = u32(data, field)
        count = None
        if count_field is not None:
            count = u8(data, count_field) if count_size == 1 else u16(data, count_field)
        size = None
        if entry_size is not None:
            size = entry_size * (count if count is not None else 1)
        block = {"header_field": hexo(field), "offset": hexo(ptr) if ptr else None,
                 "count": count, "size": hexo(size) if size is not None else None, "note": note}
        if ptr and not inside(ptr, data, size or 1):
            problems.append(f"CAR block {name} at {hexo(ptr)} runs outside the file")
        blocks[name] = block
    shadow = read_vertices(data, u32(data, 0x118), 4)
    # The file ships this block as 0xCD filler; ModelData_Load fills it in RAM.
    if shadow and any(b != 0xCD for b in data[u32(data, 0x118):u32(data, 0x118) + 0x40]):
        blocks["shadow_quad"]["vertices"] = shadow
    return blocks


def read_vertices(data: bytes, offset: int, count: int) -> list | None:
    """N64 Vtx: s16 x, y, z, flag, s16 s, t, u8 r/nx, g/ny, b/nz, a."""
    if not offset or not inside(offset, data, count * 0x10):
        return None
    out = []
    for i in range(count):
        o = offset + i * 0x10
        out.append({"pos": [s16(data, o), s16(data, o + 2), s16(data, o + 4)],
                    "st": [s16(data, o + 8), s16(data, o + 10)],
                    "rgba": list(data[o + 12:o + 16])})
    return out


def inspect(path: Path) -> dict:
    data = path.read_bytes()
    kind = path.suffix.upper().lstrip(".")
    minimum = CAR_HEADER_SIZE if kind == "CAR" else HEADER_SIZE
    if len(data) < minimum:
        raise ValueError(f"{path} is too small to be a {kind or 'CAR/OBJ'} model asset")
    part_count, material_count = u16(data, 0), u16(data, 2)
    texture_count, extra_count = u16(data, 4), u16(data, 0xC)
    palette_anim_count = u16(data, 0x12)
    part_offset = u32(data, 0x2C)
    material_offset = u32(data, 0x30)
    texture_offset = u32(data, 0x34)
    if not inside(part_offset, data) or not inside(material_offset, data) or not inside(texture_offset, data):
        raise ValueError("model header points outside the file")
    problems: list[str] = []
    parts = read_parts(data, part_offset, part_count, material_count, problems)
    tree = build_tree(parts)
    materials = read_materials(data, material_offset, material_count, problems)
    textures = read_textures(data, texture_offset, texture_count, problems)

    # Optional tables: zero means absent; WorldData_Relocate adds the base otherwise.
    tables = {}
    for field, name in ((0x38, "table_38"), (0x3C, "table_3C"), (0x40, "table_40"),
                        (0x44, "extras"), (0x48, "table_48"), (0x4C, "table_4C"),
                        (0x50, "palette_anims")):
        ptr = u32(data, field)
        tables[name] = hexo(ptr) if ptr else None
        # Empty tables point at the end of the model data (the file size for OBJs).
        if ptr and ptr != len(data) and not inside(ptr, data):
            problems.append(f"header field {hexo(field)} points outside the file")
    extras = []
    if u32(data, 0x44):
        base = u32(data, 0x44)
        for index in range(extra_count):
            offset = base + index * EXTRA_SIZE
            if not inside(offset, data, EXTRA_SIZE):
                problems.append(f"extra {index} runs past the end of the file")
                break
            ptr = u32(data, offset + 0x10)
            extras.append({"index": index, "offset": hexo(offset),
                           "fields": [f"0x{u32(data, offset + f):08X}" for f in range(0, 0x10, 4)],
                           "data_offset": hexo(ptr), "data_in_file": inside(ptr, data)})
    palette_anims = []
    if u32(data, 0x50) and palette_anim_count:
        palette_anims = read_palette_anims(data, u32(data, 0x50), palette_anim_count, texture_count, problems)

    result = {
        "format": "hwtr-model-inspection-v2",
        "asset_kind": kind,
        "source": path.name,
        "size": len(data),
        "header": {
            "part_count": part_count,
            "material_count": material_count,
            "texture_count": texture_count,
            "extra_count": extra_count,
            "palette_anim_count": palette_anim_count,
            "counts_raw": {hexo(f): u16(data, f) for f in (0x06, 0x0E, 0x10)},
            "bounds": [[f32(data, 0x14 + i * 4) for i in range(3)], [f32(data, 0x20 + i * 4) for i in range(3)]],
            "part_table_offset": hexo(part_offset),
            "material_table_offset": hexo(material_offset),
            "texture_table_offset": hexo(texture_offset),
            "tables": tables,
        },
        "hierarchy": tree,
        "parts": parts,
        "materials": materials,
        "textures": textures,
        "extras": extras,
        "palette_anims": palette_anims,
    }
    if kind == "CAR":
        result["car"] = {
            "part_count_FC": u8(data, 0xFC),
            "light_fan_count": u8(data, 0xFF),
            "light_strip_count": u8(data, 0x10C),
            "vec_11C": [f32(data, 0x11C + i * 4) for i in range(3)],
            "blocks": read_car_blocks(data, problems),
        }
    result["problems"] = problems
    result["notes"] = [
        "All offsets are file-relative, big-endian values unless marked relative.",
        "Material streams are relative to their own material record (Mesh_Relocate).",
        "Parts form first-child/next-sibling lists; parent_index is derived from them.",
        "CAR header bytes 0x100-0x15F also hold runtime state (colour, fade, damage, matrix) overwritten in RAM.",
        "Geometry command streams use a game-specific compressed format; this report intentionally does not alter them.",
    ]
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="decompressed .CAR or .OBJ model asset")
    parser.add_argument("output", type=Path, help="new JSON report path")
    args = parser.parse_args()
    if args.source.resolve() == args.output.resolve():
        raise SystemExit("error: output must not overwrite the model asset")
    try:
        result = inspect(args.source)
    except (OSError, ValueError, struct.error) as error:
        raise SystemExit(f"error: {error}")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n")
    print(f"model report written to {args.output}")
    if result["problems"]:
        print(f"{len(result['problems'])} problem(s) found; see \"problems\" in the report")


if __name__ == "__main__":
    main()
