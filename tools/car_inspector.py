#!/usr/bin/env python3
"""Read-only structural inspection for HWTR N64 .CAR vehicle assets.

This emits the parts, texture descriptors, and geometry streams used by the
game's own loader.  It does not modify game data and is the input contract for
the in-progress 3D decoder.
"""
from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path


HEADER_SIZE = 0x160
PART_SIZE = 0x54
MATERIAL_SIZE = 0x34


def u16(data: bytes, offset: int) -> int:
    return struct.unpack_from(">H", data, offset)[0]


def u32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def f32(data: bytes, offset: int) -> float:
    return struct.unpack_from(">f", data, offset)[0]


def inside(value: int, data: bytes) -> bool:
    return 0 <= value < len(data)


def inspect(path: Path) -> dict:
    data = path.read_bytes()
    if len(data) < HEADER_SIZE:
        raise ValueError(f"{path} is too small to be a CAR asset")
    part_count, material_count = u16(data, 0), u16(data, 2)
    part_offset = u32(data, 0x2C)
    material_offset = u32(data, 0x30)
    texture_count = u16(data, 4)
    texture_offset = u32(data, 0x34)
    if not inside(part_offset, data) or not inside(material_offset, data) or not inside(texture_offset, data):
        raise ValueError("CAR header points outside the file")
    parts = []
    for index in range(part_count):
        offset = part_offset + index * PART_SIZE
        if offset + PART_SIZE > len(data):
            break
        parts.append({
            "index": index,
            "offset": f"0x{offset:X}",
            "translation": [f32(data, offset + 0x0C), f32(data, offset + 0x1C), f32(data, offset + 0x2C)],
            "radius": f32(data, offset + 0x38),
            "material_index": u32(data, offset + 0x3C),
            "parent_index": u32(data, offset + 0x4C),
            "child_index": u32(data, offset + 0x50),
        })
    materials = []
    for index in range(material_count):
        offset = material_offset + index * MATERIAL_SIZE
        if offset + MATERIAL_SIZE > len(data):
            break
        stream_offset = u32(data, offset + 0x18)
        materials.append({
            "index": index,
            "offset": f"0x{offset:X}",
            "command_count": u16(data, offset + 2),
            "stream_offset": f"0x{stream_offset:X}",
            "stream_in_file": inside(stream_offset, data),
            "fields": [f"0x{u32(data, offset + field):08X}" for field in range(0, MATERIAL_SIZE, 4)],
        })
    textures = []
    for index in range(texture_count):
        offset = texture_offset + index * 8
        if offset + 8 > len(data):
            break
        descriptor, pixels = u32(data, offset), u32(data, offset + 4)
        textures.append({
            "index": index,
            "offset": f"0x{offset:X}",
            "width": descriptor >> 24,
            "height": (descriptor >> 16) & 0xFF,
            "format_code": (descriptor >> 8) & 0xFF,
            "flags": descriptor & 0xFF,
            "pixels_offset": f"0x{pixels:X}",
            "pixels_in_file": inside(pixels, data),
        })
    return {
        "format": "hwtr-car-inspection-v1",
        "source": path.name,
        "size": len(data),
        "header": {
            "part_count": part_count,
            "material_count": material_count,
            "texture_count": texture_count,
            "part_table_offset": f"0x{part_offset:X}",
            "material_table_offset": f"0x{material_offset:X}",
            "texture_table_offset": f"0x{texture_offset:X}",
        },
        "parts": parts,
        "materials": materials,
        "textures": textures,
        "notes": [
            "All offsets are file-relative, big-endian values.",
            "Geometry command streams use a game-specific compressed format; this report intentionally does not alter them.",
        ],
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="decompressed .CAR asset")
    parser.add_argument("output", type=Path, help="new JSON report path")
    args = parser.parse_args()
    if args.source.resolve() == args.output.resolve():
        raise SystemExit("error: output must not overwrite the CAR asset")
    try:
        result = inspect(args.source)
    except (OSError, ValueError, struct.error) as error:
        raise SystemExit(f"error: {error}")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n")
    print(f"CAR report written to {args.output}")


if __name__ == "__main__":
    main()
