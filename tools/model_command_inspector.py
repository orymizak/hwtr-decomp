#!/usr/bin/env python3
"""Inspect the compact material command streams used by HWTR CAR/OBJ assets.

This is a read-only decoder of the command dialect interpreted by
func_80079140.  It deliberately reports vertex-cache triangle indices rather
than producing a mesh until the segment-3 vertex source is fully established.
"""
from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path


MATERIAL_SIZE = 0x34


def u16(data: bytes, at: int) -> int:
    return struct.unpack_from(">H", data, at)[0]


def u32(data: bytes, at: int) -> int:
    return struct.unpack_from(">I", data, at)[0]


def parse_stream(data: bytes, start: int, count: int) -> list[dict]:
    """Decode command lengths and the topology opcodes 0/1/2/B."""
    pos = start
    commands: list[dict] = []
    for number in range(count):
        if pos >= len(data):
            raise ValueError(f"stream ended at command {number}")
        opcode = data[pos] >> 4
        low = data[pos] & 15
        command = {"number": number, "offset": f"0x{pos:X}", "opcode": opcode}
        if opcode == 0:
            if pos + 4 > len(data): raise ValueError("truncated triangle command")
            command["triangle_cache_indices"] = list(data[pos + 1 : pos + 4]); step = 4
        elif opcode == 1:
            if pos + 2 > len(data): raise ValueError("truncated compact triangle command")
            command["triangle_cache_indices"] = [low, data[pos + 1] >> 4, data[pos + 1] & 15]; step = 2
        elif opcode == 2:
            command["triangle_cache_indices"] = ["previous", f"previous+{1 + ((data[pos] >> 2) & 3)}", f"previous+{1 + (data[pos] & 3)}"]; step = 1
        elif opcode == 3:
            if pos + 2 > len(data): raise ValueError("truncated branch command")
            command["display_list_offset"] = (data[pos + 1] * 16) + low; step = 2
        elif opcode == 4:
            if pos + 3 > len(data): raise ValueError("truncated texture-size command")
            command["texture_dimensions"] = [data[pos + 1], data[pos + 2]]; step = 3
        elif opcode in (5, 6, 7, 10, 12):
            step = 4 if opcode == 7 else 1
        elif opcode in (8, 9):
            step = 2
        elif opcode == 11:
            if pos + 4 > len(data): raise ValueError("truncated vertex-load command")
            command["vertex_load"] = {
                "count": data[pos + 1] + 1,
                "segment3_byte_offset": (data[pos + 2] + data[pos + 3] * 256) * 16,
            }
            step = 4
        else:
            raise ValueError(f"unsupported opcode {opcode:X} at {pos:#x}")
        command["encoded_bytes"] = data[pos : pos + step].hex()
        commands.append(command)
        pos += step
    return commands


def inspect(source: Path) -> dict:
    data = source.read_bytes()
    if len(data) < 0x38:
        raise ValueError("asset too small")
    material_count = u16(data, 2)
    material_table = u32(data, 0x30)
    materials = []
    for index in range(material_count):
        base = material_table + index * MATERIAL_SIZE
        if base + MATERIAL_SIZE > len(data):
            raise ValueError(f"material {index} outside file")
        # func_80079DBC fixes offsets relative to the material record itself.
        stream = base + u32(data, base + 0x18)
        command_count = u16(data, base + 2)
        commands = parse_stream(data, stream, command_count)
        materials.append({"index": index, "record_offset": f"0x{base:X}", "stream_offset": f"0x{stream:X}", "command_count": command_count, "commands": commands})
    return {"format": "hwtr-model-command-report-v1", "source": source.name, "size": len(data), "materials": materials,
            "notes": ["Offsets inside material records are relative to that material record.", "Triangle indices address the RSP vertex cache. Segment-3 vertex binding is still under reconstruction; this report does not claim to contain final positions."]}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    if args.source.resolve() == args.output.resolve():
        raise SystemExit("error: output must not overwrite source")
    try:
        result = inspect(args.source)
    except (OSError, ValueError, struct.error) as error:
        raise SystemExit(f"error: {error}")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n")
    print(f"command report written to {args.output}")


if __name__ == "__main__":
    main()
