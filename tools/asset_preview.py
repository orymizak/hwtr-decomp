#!/usr/bin/env python3
"""Create safe PNG previews and source manifests for HWTR binary assets.

This tool never changes the ROM asset.  It creates a PNG alongside a JSON
manifest that records the original filename, SHA-1, size, byte layout and the
confidence of the decoder used.  Some formats are fully understood only as a
diagnostic byte map; those PNGs are explicitly labelled as such instead of
pretending they are game-accurate textures.

Examples:
  python3 tools/asset_preview.py export /path/to/assets/files previews
  python3 tools/asset_preview.py export /path/to/ENVMAP1.TEX previews

Supported decoders:
  CLOUD*.TEX  512 bytes, visualized as 32x32 packed 4-bit values
  ENVMAP1.TEX 8-byte header followed by a 32x32 RGBA5551 image
  ACTNFNT.OVL game font: glyph atlas plus glyph/source manifest
  EFFECTS.TEX 30 sequential RGBA32/RGBA5551 effect textures

Unknown formats are skipped by default.  Pass ``--diagnostic`` to create a
raw-byte map for them.  That map is useful to locate embedded regions and
compare revisions, but it is not an editable reconstruction of game art.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import sys
import zlib
from pathlib import Path
from typing import Any


def fail(message: str) -> None:
    raise SystemExit(f"error: {message}")


def sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def png_chunk(kind: bytes, payload: bytes) -> bytes:
    return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", zlib.crc32(kind + payload) & 0xFFFFFFFF)


def write_png(path: Path, rgba: bytes, width: int, height: int) -> None:
    if len(rgba) != width * height * 4:
        fail("buffer RGBA con tamaño inválido")
    rows = b"".join(b"\0" + rgba[row * width * 4 : (row + 1) * width * 4] for row in range(height))
    content = (
        b"\x89PNG\r\n\x1a\n"
        + png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0))
        + png_chunk(b"IDAT", zlib.compress(rows, 9))
        + png_chunk(b"IEND", b"")
    )
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(content)


def rgba5551(value: int) -> tuple[int, int, int, int]:
    return (
        ((value >> 11) & 0x1F) * 255 // 31,
        ((value >> 6) & 0x1F) * 255 // 31,
        ((value >> 1) & 0x1F) * 255 // 31,
        255 if value & 1 else 0,
    )


def cloud_preview(data: bytes) -> tuple[bytes, int, int, dict[str, Any]]:
    if len(data) != 0x200:
        fail("un CLOUD*.TEX debe medir exactamente 0x200 bytes")
    rgba = bytearray()
    for packed in data:
        for nibble in (packed >> 4, packed & 0x0F):
            # This retains the two values stored in every source byte.  It is a
            # grayscale interpretation, deliberately not a claim about the
            # game's combiner/alpha mode.
            intensity = nibble * 17
            rgba.extend((intensity, intensity, intensity, 255))
    return bytes(rgba), 32, 32, {
        "kind": "candidate-packed-4bpp-grayscale",
        "confidence": "candidate",
        "source_layout": "512 bytes; high nibble then low nibble, 32x32 samples",
        "warning": "El motor puede tratar estos 4 bits como intensidad, alfa u otro formato; esta PNG conserva los valores, pero no afirma ser el color final en pantalla.",
    }


def envmap_preview(data: bytes) -> tuple[bytes, int, int, dict[str, Any]]:
    if len(data) != 0x808:
        fail("ENVMAP1.TEX debe medir exactamente 0x808 bytes")
    width, height = data[0], data[1]
    if (width, height) != (32, 32):
        fail(f"cabecera ENVMAP inesperada: {width}x{height}")
    rgba = bytearray()
    for offset in range(8, len(data), 2):
        rgba.extend(rgba5551(struct.unpack_from(">H", data, offset)[0]))
    return bytes(rgba), width, height, {
        "kind": "candidate-rgba5551",
        "confidence": "high",
        "source_layout": "8-byte header (primeros bytes: ancho, alto), seguido de 32x32 valores big-endian RGBA5551",
        "header_hex": data[:8].hex(),
        "warning": "La PNG representa los píxeles RGBA5551; los últimos seis bytes de cabecera aún no tienen nombre confirmado.",
    }


def byte_map_preview(data: bytes) -> tuple[bytes, int, int, dict[str, Any]]:
    width = 256
    height = max(1, (len(data) + width - 1) // width)
    padded = data + b"\0" * (width * height - len(data))
    rgba = bytearray()
    for value in padded:
        rgba.extend((value, value, value, 255))
    return bytes(rgba), width, height, {
        "kind": "diagnostic-raw-byte-map",
        "confidence": "diagnostic-only",
        "source_layout": "un píxel gris por byte, 256 bytes por fila; el negro de relleno solo completa la última fila",
        "warning": "Esto no es una textura, fuente, edificio o escenario decodificado. Sirve para localizar regiones y comparar binarios sin alterar el archivo de origen.",
    }


def font_preview(data: bytes) -> tuple[bytes, int, int, dict[str, Any]]:
    """Decode the font container loaded by Font_Load in src/font.c.

    Its first 256 bytes map character codes to glyph indices.  At 0x500 the
    executable walks sequential 8-byte glyph headers followed by RGBA5551
    bitmap data aligned to 8 bytes.  The in-memory pointer table at 0x100 is
    zero in the file and is filled by the loader, so it is deliberately not
    treated as source image data.
    """
    if len(data) < 0x508:
        fail("el archivo de fuente es demasiado pequeño")
    glyph_index = data[:0x100]
    used = [value for value in glyph_index if value != 0xFF]
    if not used:
        fail("no se encontró una tabla de glifos")
    glyph_count = max(used) + 1
    offset = 0x500
    glyphs: list[dict[str, Any]] = []
    for index in range(glyph_count):
        if offset + 8 > len(data):
            fail(f"cabecera incompleta del glifo {index}")
        width, height, pixel_format = data[offset], data[offset + 1], data[offset + 2]
        if pixel_format != 0:
            fail(f"formato de píxel {pixel_format} no reconocido en glifo {index}")
        pixel_bytes = width * height * 2
        data_start = offset + 8
        data_end = data_start + pixel_bytes
        if data_end > len(data):
            fail(f"datos incompletos del glifo {index}")
        glyphs.append({"index": index, "width": width, "height": height, "data_start": data_start, "data_end": data_end})
        offset = (data_end + 7) & ~7

    cell_width = max(glyph["width"] for glyph in glyphs) + 2
    cell_height = max(glyph["height"] for glyph in glyphs) + 2
    columns = 16
    rows = (glyph_count + columns - 1) // columns
    width, height = columns * cell_width, rows * cell_height
    rgba = bytearray(width * height * 4)
    # Checkerboard-like dark backing makes transparent pixels visible without
    # changing the source RGBA5551 values of the actual glyph bitmap.
    for y in range(height):
        for x in range(width):
            value = 46 if ((x // 4) + (y // 4)) & 1 else 68
            position = (y * width + x) * 4
            rgba[position : position + 4] = bytes((value, value, value, 255))
    for glyph in glyphs:
        col, row = glyph["index"] % columns, glyph["index"] // columns
        for y in range(glyph["height"]):
            for x in range(glyph["width"]):
                value = struct.unpack_from(">H", data, glyph["data_start"] + (y * glyph["width"] + x) * 2)[0]
                r, g, b, alpha = rgba5551(value)
                if alpha:
                    position = ((row * cell_height + 1 + y) * width + col * cell_width + 1 + x) * 4
                    rgba[position : position + 4] = bytes((r, g, b, alpha))
    chars = {
        chr(code): index for code, index in enumerate(glyph_index)
        if index != 0xFF and 32 <= code <= 126
    }
    return bytes(rgba), width, height, {
        "kind": "hwtr-font-rgba5551-glyph-atlas",
        "confidence": "high",
        "source_layout": "glyphIndex[256] en 0x000; tabla de punteros temporal en 0x100..0x4FF; glifos consecutivos desde 0x500: width u8, height u8, format u8, flags u8, pointer temporal u32, bitmap RGBA5551 big-endian, alineado a 8 bytes",
        "glyph_count": glyph_count,
        "atlas": {"columns": columns, "cell_dimensions": [cell_width, cell_height], "glyphs": glyphs},
        "character_to_glyph": chars,
    }


def effects_textures(data: bytes) -> list[dict[str, Any]]:
    """Parse the 30 entries traversed by the game's EFFECTS.TEX loader."""
    offset = 0
    textures: list[dict[str, Any]] = []
    for index in range(30):
        if offset + 8 > len(data):
            fail(f"EFFECTS.TEX termina antes de la entrada {index}")
        width, height, pixel_format, flags = data[offset : offset + 4]
        if not width or not height or pixel_format not in (0, 3):
            fail(f"cabecera inválida en EFFECTS.TEX, entrada {index} @ {offset:#x}")
        bytes_per_pixel = 2 if pixel_format == 0 else 4
        pixel_start = offset + 8
        pixel_end = pixel_start + width * height * bytes_per_pixel
        if pixel_end > len(data):
            fail(f"píxeles incompletos en EFFECTS.TEX, entrada {index}")
        textures.append({
            "index": index, "offset": offset, "width": width, "height": height,
            "pixel_format": "RGBA5551" if pixel_format == 0 else "RGBA32",
            "flags": flags, "pixel_start": pixel_start, "pixel_end": pixel_end,
        })
        offset = (pixel_end + 7) & ~7
    if offset != len(data):
        fail(f"EFFECTS.TEX tiene {len(data) - offset} bytes no descritos al final")
    return textures


def effects_export(source: Path, data: bytes, output_dir: Path) -> Path:
    textures = effects_textures(data)
    atlas_width, atlas_height = 6 * 64, 5 * 64
    atlas = bytearray(atlas_width * atlas_height * 4)
    # Transparent checkerboard background distinguishes alpha-zero effects.
    for y in range(atlas_height):
        for x in range(atlas_width):
            value = 48 if ((x // 8) + (y // 8)) & 1 else 72
            where = (y * atlas_width + x) * 4
            atlas[where : where + 4] = bytes((value, value, value, 255))
    for texture in textures:
        pixels = bytearray()
        if texture["pixel_format"] == "RGBA32":
            pixels.extend(data[texture["pixel_start"] : texture["pixel_end"]])
        else:
            for pos in range(texture["pixel_start"], texture["pixel_end"], 2):
                pixels.extend(rgba5551(struct.unpack_from(">H", data, pos)[0]))
        sprite = output_dir / f"{source.name}.effect{texture['index']:02d}.png"
        write_png(sprite, bytes(pixels), texture["width"], texture["height"])
        col, row = texture["index"] % 6, texture["index"] // 6
        for y in range(texture["height"]):
            for x in range(texture["width"]):
                pixel = (y * texture["width"] + x) * 4
                if pixels[pixel + 3]:
                    target = ((row * 64 + y) * atlas_width + col * 64 + x) * 4
                    atlas[target : target + 4] = pixels[pixel : pixel + 4]
    png = output_dir / f"{source.name}.preview.png"
    write_png(png, bytes(atlas), atlas_width, atlas_height)
    manifest = output_dir / f"{source.name}.preview.json"
    manifest.write_text(json.dumps({
        "format": "hwtr-effects-textures-v1", "source": source.name,
        "source_sha1": sha1(data), "size": len(data), "png": png.name,
        "png_dimensions": [atlas_width, atlas_height],
        "conversion": {
            "kind": "hwtr-effects-texture-container", "confidence": "high",
            "source_layout": "30 entradas consecutivas: width u8, height u8, format u8 (0=RGBA5551 big-endian, 3=RGBA32), flags u8, pointer temporal u32, píxeles; cada entrada alineada a 8 bytes.",
            "atlas": "6 columnas por 5 filas de 64x64; los sprites individuales se exportan junto al atlas.",
            "textures": textures,
        },
        "original_binary": "El archivo de origen no se modifica. La reimportación todavía no está implementada.",
    }, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    return png


def choose_decoder(source: Path, data: bytes) -> tuple[bytes, int, int, dict[str, Any]]:
    name = source.name.upper()
    if name == "ACTNFNT.OVL":
        return font_preview(data)
    if name.startswith("CLOUD") and name.endswith(".TEX"):
        return cloud_preview(data)
    if name == "ENVMAP1.TEX":
        return envmap_preview(data)
    return byte_map_preview(data)


def export_one(source: Path, output_dir: Path, diagnostic: bool = False) -> Path | None:
    if not source.is_file():
        fail(f"no existe el archivo: {source}")
    data = source.read_bytes()
    name = source.name.upper()
    known = name.startswith("CLOUD") and source.suffix.upper() == ".TEX" or name in {"ENVMAP1.TEX", "ACTNFNT.OVL", "EFFECTS.TEX"}
    if not known and not diagnostic:
        return None
    if name == "EFFECTS.TEX":
        output_dir.mkdir(parents=True, exist_ok=True)
        return effects_export(source, data, output_dir)
    rgba, width, height, conversion = choose_decoder(source, data)
    png = output_dir / f"{source.name}.preview.png"
    manifest = output_dir / f"{source.name}.preview.json"
    write_png(png, rgba, width, height)
    manifest.parent.mkdir(parents=True, exist_ok=True)
    manifest.write_text(json.dumps({
        "format": "hwtr-asset-preview-v1",
        "source": source.name,
        "source_sha1": sha1(data),
        "size": len(data),
        "png": png.name,
        "png_dimensions": [width, height],
        "conversion": conversion,
        "original_binary": "El archivo de origen no se modifica ni se puede reconstruir desde esta previsualización sin un importador específico.",
    }, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    return png


def asset_files(directory: Path) -> list[Path]:
    # Restrict bulk export to non-car resources requested for inspection.  CAR
    # files already have their own structural inspector and cannot yet be
    # faithfully rasterized.
    suffixes = {".TEX", ".OVL", ".BLD", ".WLD", ".SCN", ".SKY", ".OBJ"}
    return sorted(path for path in directory.iterdir() if path.is_file() and path.suffix.upper() in suffixes)


def main() -> None:
    parser = argparse.ArgumentParser(description="Exporta previsualizaciones PNG no destructivas de assets HWTR")
    sub = parser.add_subparsers(dest="command", required=True)
    export = sub.add_parser("export", help="convierte un archivo o grupo de assets a PNG + manifiesto JSON")
    export.add_argument("input", type=Path, help="archivo fuente o directorio assets/files")
    export.add_argument("output", type=Path, help="directorio de salida (nunca se toca el input)")
    export.add_argument("--diagnostic", action="store_true", help="incluye formatos desconocidos como mapas de bytes; no son arte final")
    args = parser.parse_args()

    if args.command == "export":
        sources = asset_files(args.input) if args.input.is_dir() else [args.input]
        if not sources:
            fail("no se encontraron assets compatibles")
        for source in sources:
            png = export_one(source, args.output, args.diagnostic)
            if png:
                print(png)


if __name__ == "__main__":
    try:
        main()
    except (OSError, ValueError, struct.error) as error:
        fail(str(error))
