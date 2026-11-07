#!/usr/bin/env python3
"""Extrae y descomprime los archivos del juego usando la tabla de la ROM del usuario.

Uso: python3 tools/extract_files.py [--rom baserom.z64] [--out assets/files] [--list]
"""
import argparse
import hashlib
import struct
import sys
from pathlib import Path

ROM_SHA1 = "94913a07e49005ffd43dfdfa16ff5862bdb93748"

VRAM_BASE = 0x80060400
ROM_BASE = 0x1000
FILE_TABLE_ROM = 0x911F0  # gFileTable
FILE_COUNT = 201  # FileTable_Init
ENTRY_SIZE = 28

TYPE_RAW = 0
TYPE_LZ = 1  # func_8006EE44
TYPE_LZ_CHUNKED = 2  # func_8006EEEC


class Entry:
    def __init__(self, rom, offset):
        name_ptr, type_word, self.stored, self.size, self.first_chunk, self.start, self.end = struct.unpack_from(
            ">7I", rom, offset
        )
        self.type = type_word >> 24
        name_rom = name_ptr - VRAM_BASE + ROM_BASE
        self.name = rom[name_rom : rom.index(b"\0", name_rom)].decode("ascii")


def read_table(rom):
    return [Entry(rom, FILE_TABLE_ROM + i * ENTRY_SIZE) for i in range(FILE_COUNT)]


def lz_decompress(src):
    """Equivalente a func_8007F050. Devuelve (datos, bytes de src consumidos)."""
    size, window_bits, offset_len_extra = struct.unpack_from(">IHH", src)
    length_bits = offset_len_extra & 0xFF
    total_bits = (window_bits & 0xFF) + length_bits
    value_mask = (1 << total_bits) - 1
    length_mask = (1 << length_bits) - 1

    prefix = ((1 << window_bits) + 1) & 0xFFFF
    out = bytearray(src[8 : 8 + prefix])

    # Los accesos de 2-3 bytes del original pueden leer más allá del final del flujo.
    data = bytes(src) + b"\0\0\0"
    bit = (8 + prefix) * 8

    while len(out) < size:
        p = bit >> 3
        shift = bit & 7
        is_match = data[p] >> shift & 1
        bit += 1
        p = bit >> 3
        shift = bit & 7
        if is_match:
            value = ((data[p] | data[p + 1] << 8 | data[p + 2] << 16) >> shift) & value_mask
            bit += total_bits
            distance = (value >> length_bits) + 1
            length = (value & length_mask) + 1
            if distance > len(out):
                raise ValueError(f"referencia fuera de rango: distancia {distance} con {len(out)} bytes")
            for _ in range(length):
                out.append(out[-distance])
        else:
            out.append(((data[p] | data[p + 1] << 8) >> shift) & 0xFF)
            bit += 8

    if len(out) != size:
        raise ValueError(f"tamaño {len(out):#x}, se esperaba {size:#x}")
    return bytes(out), (bit + 7) >> 3


def unpack(rom, entry):
    stored = rom[entry.start : entry.start + entry.stored]
    if entry.type == TYPE_RAW:
        return stored
    if entry.type == TYPE_LZ:
        data, used = lz_decompress(stored)
        if used > len(stored):
            raise ValueError(f"se consumieron {used:#x} de {len(stored):#x} bytes")
        return data
    if entry.type == TYPE_LZ_CHUNKED:
        # Cada trozo: u32 tamaño del siguiente trozo (0 = último), u32 sin usar, flujo LZ.
        parts = []
        pos = 0
        chunk_size = entry.first_chunk
        while chunk_size:
            chunk = stored[pos : pos + chunk_size]
            next_size = struct.unpack_from(">I", chunk)[0]
            data, used = lz_decompress(chunk[8:])
            if used > chunk_size - 8:
                raise ValueError(f"trozo en {pos:#x}: se consumieron {used:#x} de {chunk_size - 8:#x} bytes")
            parts.append(data)
            pos += chunk_size
            chunk_size = next_size
        if pos != entry.stored:
            raise ValueError(f"los trozos suman {pos:#x}, se esperaba {entry.stored:#x}")
        return b"".join(parts)
    raise ValueError(f"tipo desconocido {entry.type}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baserom.z64"))
    parser.add_argument("--out", type=Path, default=Path("assets/files"))
    parser.add_argument("--list", action="store_true", help="solo muestra la tabla")
    args = parser.parse_args()

    rom = args.rom.read_bytes()
    sha1 = hashlib.sha1(rom).hexdigest()
    if sha1 != ROM_SHA1:
        sys.exit(f"{args.rom}: SHA-1 {sha1} no coincide con {ROM_SHA1}")

    entries = read_table(rom)
    if args.list:
        for e in entries:
            print(f"{e.start:08X} {e.end:08X} tipo={e.type} guardado={e.stored:#9x} real={e.size:#9x} {e.name}")
        return

    args.out.mkdir(parents=True, exist_ok=True)
    errors = 0
    for e in entries:
        try:
            data = unpack(rom, e)
            if len(data) != e.size:
                raise ValueError(f"tamaño {len(data):#x}, se esperaba {e.size:#x}")
        except (ValueError, IndexError, struct.error) as err:
            print(f"ERROR {e.name}: {err}", file=sys.stderr)
            errors += 1
            continue
        (args.out / e.name).write_bytes(data)

    print(f"{len(entries) - errors}/{len(entries)} archivos en {args.out}")
    if errors:
        sys.exit(1)


if __name__ == "__main__":
    main()
