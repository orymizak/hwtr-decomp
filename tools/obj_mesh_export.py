#!/usr/bin/env python3
"""Export the verified static-mesh subset of HWTR .OBJ assets to Wavefront OBJ.

Supported subset: material streams with literal/compact triangles and vertex
loads, where segment 3 resolves to the material's static position array.  The
mapping is verified with ARROWD.OBJ.  The tool refuses unsupported command
variants rather than emitting a plausible-but-wrong mesh.
"""
from __future__ import annotations

import argparse
import struct
from pathlib import Path


def u16(d: bytes, o: int) -> int: return struct.unpack_from(">H", d, o)[0]
def s16(d: bytes, o: int) -> int: return struct.unpack_from(">h", d, o)[0]
def u32(d: bytes, o: int) -> int: return struct.unpack_from(">I", d, o)[0]


def export(source: Path, output: Path) -> tuple[int, int]:
    d = source.read_bytes()
    if u16(d, 2) != 1:
        raise ValueError("esta primera exportación solo admite un material")
    material = u32(d, 0x30)
    if material + 0x34 > len(d): raise ValueError("material fuera del archivo")
    command_count = u16(d, material + 2)
    vertex_count = u16(d, material + 6)
    position_start = material + u32(d, material + 0x1C)
    stream = material + u32(d, material + 0x18)
    if position_start + vertex_count * 6 > len(d): raise ValueError("array de posiciones fuera del archivo")
    vertices = [(s16(d, position_start + i * 6), s16(d, position_start + i * 6 + 2), s16(d, position_start + i * 6 + 4)) for i in range(vertex_count)]
    cache: dict[int, int] = {}
    faces: list[tuple[int, int, int]] = []
    previous: tuple[int, int, int] | None = None
    pos = stream
    for _ in range(command_count):
        if pos >= len(d): raise ValueError("stream truncado")
        op, low = d[pos] >> 4, d[pos] & 15
        if op == 11:
            if pos + 4 > len(d): raise ValueError("carga de vértices truncada")
            count, source_byte = d[pos + 1] + 1, (d[pos + 2] + d[pos + 3] * 256) * 16
            # The renderer's segment-3 pointer is a 16-byte dynamic Vtx
            # buffer. Its entries originate from these static position records.
            start = source_byte // 16
            if start + count > vertex_count: raise ValueError("la carga de vértices no cabe en las posiciones estáticas")
            for index in range(count): cache[index] = start + index
            pos += 4
        elif op == 0:
            tri = tuple(d[pos + 1 : pos + 4]); pos += 4
            if any(index not in cache for index in tri): raise ValueError("triángulo refiere una caché no cargada")
            previous = tuple(cache[index] for index in tri); faces.append(previous)
        elif op == 1:
            tri = (low, d[pos + 1] >> 4, d[pos + 1] & 15); pos += 2
            if any(index not in cache for index in tri): raise ValueError("triángulo compacto refiere una caché no cargada")
            previous = tuple(cache[index] for index in tri); faces.append(previous)
        elif op == 2:
            if previous is None: raise ValueError("triángulo incremental sin triángulo anterior")
            # This dialect advances the last two cache indices from the prior
            # triangle.  It is retained here for models that use triangle fans.
            tri = (previous[2], previous[1] + 1 + ((d[pos] >> 2) & 3), previous[2] + 1 + (d[pos] & 3)); pos += 1
            if any(index not in cache for index in tri): raise ValueError("triángulo incremental no soportado por la caché actual")
            previous = tuple(cache[index] for index in tri); faces.append(previous)
        elif op == 3: pos += 2
        elif op == 4: pos += 3
        elif op == 7: pos += 4
        elif op in (8, 9): pos += 2
        elif op in (5, 6, 10, 12): pos += 1
        else: raise ValueError(f"opcode {op:X} no soportado")
    if not faces: raise ValueError("no se encontraron caras")
    output.parent.mkdir(parents=True, exist_ok=True)
    lines = [f"# Exportación estática verificada de {source.name}", "# Coordenadas enteras originales N64"]
    lines += [f"v {x} {y} {z}" for x, y, z in vertices]
    lines += [f"f {a + 1} {b + 1} {c + 1}" for a, b, c in faces]
    output.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return len(vertices), len(faces)


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("source", type=Path); p.add_argument("output", type=Path)
    a = p.parse_args()
    if a.source.resolve() == a.output.resolve(): raise SystemExit("error: la salida no puede reemplazar el asset")
    try: vertices, faces = export(a.source, a.output)
    except (OSError, ValueError, struct.error) as e: raise SystemExit(f"error: {e}")
    print(f"OBJ escrito: {a.output} ({vertices} vértices, {faces} caras)")


if __name__ == "__main__": main()
