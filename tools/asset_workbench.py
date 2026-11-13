#!/usr/bin/env python3
"""Export and safely re-import editable Hot Wheels Turbo Racing assets.

The tool deliberately never overwrites an input.  Work from the exported PNG or
JSON files and send rebuilt binaries to a separate directory; copying a result
into ``assets/files`` is an explicit, later decision.

Examples:
  python3 tools/asset_workbench.py bg-export /path/to/assets/files exports/bg
  python3 tools/asset_workbench.py bg-import exports/bg/N64MAIN.png \
      /path/to/assets/files/N64MAIN.BG rebuilt/N64MAIN.BG
  python3 tools/asset_workbench.py data-export /path/to/assets/files exports/data
  python3 tools/asset_workbench.py data-import exports/data/TWINMILL.CWH.json \
      /path/to/assets/files/TWINMILL.CWH rebuilt/TWINMILL.CWH

``bg-import`` keeps the original 256-colour palette and maps altered pixels to
their nearest palette entry.  Palette index 0 is preserved as transparent;
all other entries are rendered opaque. Consequently, export followed by import
is byte-identical and palette edits are intentionally not supported.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import struct
import subprocess
import sys
from pathlib import Path
from typing import Any


BG_WIDTH = 640
BG_HEIGHT = 480
BG_PALETTE_BYTES = 512
BG_SIZE = BG_PALETTE_BYTES + BG_WIDTH * BG_HEIGHT
CWH_SIZE = 0x158
DUP_SIZE = 0xAD
TUNING_SIZE = 0x100
# These are the fields whose meaning is established in MECHANICS.md.  Keeping
# the rest in raw_hex prevents a JSON round trip from silently changing unknown
# integers, pointers, or IEEE-754 NaNs.
CWH_EDITABLE_FLOAT_OFFSETS = (
    0x008, 0x00C, 0x010, 0x014, 0x018, 0x01C, 0x020, 0x024,
    0x028, 0x02C, 0x030, 0x034, 0x038, 0x04C, 0x03C, 0x050,
    0x070, 0x078,
)
DUP_MULTIPLIERS = {
    "mass": 0x38,
    "center_of_mass_x": 0x3C,
    "center_of_mass_y": 0x40,
    "drive_force": 0x44,
    "drive_split": 0x48,
    "drag_coefficient": 0x4C,
    "front_downforce_coefficient": 0x50,
    "rear_downforce_coefficient": 0x54,
    "front_downforce_area": 0x58,
    "rear_downforce_area": 0x5C,
    "vehicle_parameter_530": 0x60,
    "vehicle_parameter_528": 0x64,
    "vehicle_parameter_52c": 0x68,
    "front_suspension_travel": 0x6C,
    "front_grip": 0x70,
    "vehicle_parameter_504": 0x74,
    "vehicle_parameter_508": 0x78,
    "rear_suspension_travel": 0x7C,
    "rear_grip": 0x80,
    "vehicle_parameter_51c": 0x84,
    "vehicle_parameter_520": 0x88,
    "vehicle_parameter_400": 0x8C,
    "vehicle_parameter_404": 0x90,
    "vehicle_parameter_42c": 0x94,
}


def fail(message: str) -> None:
    raise SystemExit(f"error: {message}")


def sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def require_distinct(input_path: Path, output_path: Path) -> None:
    if input_path.resolve() == output_path.resolve():
        fail("la salida debe ser una ruta distinta del archivo original")


def rgba5551_to_rgba(value: int) -> tuple[int, int, int, int]:
    """Decode the BG palette.

    The shipped menu backgrounds have bit 0 clear in every RGBA5551 entry, but
    their draw path does not use it as alpha.  Treating it literally would turn
    every exported PNG transparent, so BGs are exported as opaque images.
    """
    return (
        ((value >> 11) & 0x1F) * 255 // 31,
        ((value >> 6) & 0x1F) * 255 // 31,
        ((value >> 1) & 0x1F) * 255 // 31,
        255,
    )


def rgba_to_raw_png(path: Path, width: int, height: int) -> bytes:
    magick = shutil.which("magick") or shutil.which("convert")
    if not magick:
        fail("se requiere ImageMagick (comando magick o convert)")
    command = [magick, str(path), "-alpha", "on", "-depth", "8", "rgba:-"]
    result = subprocess.run(command, check=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode:
        fail(f"ImageMagick no pudo leer {path}: {result.stderr.decode(errors='replace').strip()}")
    expected = width * height * 4
    if len(result.stdout) != expected:
        fail(f"{path} debe medir {width}x{height} píxeles (se obtuvieron {len(result.stdout)} bytes RGBA)")
    return result.stdout


def write_png(path: Path, rgba: bytes, width: int, height: int) -> None:
    magick = shutil.which("magick") or shutil.which("convert")
    if not magick:
        fail("se requiere ImageMagick (comando magick o convert)")
    path.parent.mkdir(parents=True, exist_ok=True)
    command = [magick, "-size", f"{width}x{height}", "-depth", "8", "rgba:-", str(path)]
    result = subprocess.run(command, input=rgba, check=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode:
        fail(f"ImageMagick no pudo escribir {path}: {result.stderr.decode(errors='replace').strip()}")


def export_background(source: Path, output: Path) -> None:
    data = source.read_bytes()
    if len(data) != BG_SIZE:
        fail(f"{source.name} mide {len(data):#x}; un fondo BG debe medir {BG_SIZE:#x}")
    palette = [rgba5551_to_rgba(struct.unpack_from(">H", data, i)[0]) for i in range(0, BG_PALETTE_BYTES, 2)]
    rgba = bytearray()
    for index in data[BG_PALETTE_BYTES:]:
        r, g, b, _ = palette[index]
        # The game uses index zero as the transparent/clear entry for these
        # menu images.  Its RGBA5551 alpha bit is clear for every palette entry,
        # so it cannot be used directly when exporting a useful PNG.
        rgba.extend((r, g, b, 0 if index == 0 else 255))
    write_png(output, bytes(rgba), BG_WIDTH, BG_HEIGHT)


def import_background(png: Path, original: Path, output: Path) -> None:
    require_distinct(original, output)
    data = original.read_bytes()
    if len(data) != BG_SIZE:
        fail(f"{original.name} no es un fondo BG de {BG_SIZE:#x} bytes")
    rgba = rgba_to_raw_png(png, BG_WIDTH, BG_HEIGHT)
    palette_raw = data[:BG_PALETTE_BYTES]
    palette = [rgba5551_to_rgba(struct.unpack_from(">H", palette_raw, i)[0]) for i in range(0, BG_PALETTE_BYTES, 2)]
    indices = bytearray(BG_WIDTH * BG_HEIGHT)
    exact = {colour[:3]: index for index, colour in enumerate(palette) if index != 0}
    for pixel in range(BG_WIDTH * BG_HEIGHT):
        colour = tuple(rgba[pixel * 4 : pixel * 4 + 4])
        if colour[3] == 0:
            indices[pixel] = 0
            continue
        index = exact.get(colour[:3])
        if index is None:
            index = min(range(1, 256), key=lambda n: sum((colour[c] - palette[n][c]) ** 2 for c in range(3)))
        indices[pixel] = index
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(palette_raw + indices)


def cwh_document(data: bytes, name: str) -> dict[str, Any]:
    if len(data) != CWH_SIZE:
        fail(f"{name} debe medir {CWH_SIZE:#x} bytes")
    floats = {
        f"0x{offset:03X}": struct.unpack_from(">f", data, offset)[0]
        for offset in CWH_EDITABLE_FLOAT_OFFSETS
    }
    return {
        "format": "hwtr-cwh-v1",
        "source": name,
        "source_sha1": sha1(data),
        "size": len(data),
        "header_u32_be": f"0x{struct.unpack_from('>I', data)[0]:08X}",
        "bytes_0x004_0x007": list(data[4:8]),
        "f32_be": floats,
        "notes": {
            "0x008": "steering maximum",
            "0x00C": "mass",
            "0x018": "drive force",
            "0x01C": "drive split",
            "0x020": "drag coefficient",
            "0x024": "front downforce coefficient",
            "0x028": "rear downforce coefficient",
            "0x02C": "front downforce area",
            "0x030": "rear downforce area",
            "0x070": "AI pace",
            "0x078": "crash resistance",
            "0x07C": "flags (view/edit via raw_hex instead of f32_be)",
        },
        "raw_hex": data.hex(),
    }


def raw_document(data: bytes, name: str, kind: str) -> dict[str, Any]:
    return {
        "format": f"hwtr-{kind}-v1",
        "source": name,
        "source_sha1": sha1(data),
        "size": len(data),
        "raw_hex": data.hex(),
    }


def read_c_string(data: bytes, offset: int, size: int) -> str:
    return data[offset : offset + size].split(b"\0", 1)[0].decode("ascii")


def write_c_string(data: bytearray, offset: int, size: int, value: Any, label: str) -> None:
    if not isinstance(value, str):
        fail(f"{label} debe ser texto")
    try:
        encoded = value.encode("ascii")
    except UnicodeEncodeError:
        fail(f"{label} solo admite ASCII")
    if len(encoded) >= size:
        fail(f"{label} debe ocupar como máximo {size - 1} bytes ASCII")
    # Some string slots have opaque metadata after their NUL terminator.  A
    # terminator is sufficient for the game, and preserving the remaining
    # bytes makes an unedited export/import lossless.
    data[offset : offset + len(encoded) + 1] = encoded + b"\0"


def write_dup_identity(data: bytearray, internal_name: Any, asset_type: Any) -> None:
    if not isinstance(internal_name, str) or not isinstance(asset_type, str):
        fail("internal_name y asset_type deben ser texto")
    try:
        packed = internal_name.encode("ascii") + b"\0" + asset_type.encode("ascii") + b"\0"
    except UnicodeEncodeError:
        fail("internal_name y asset_type solo admiten ASCII")
    if len(packed) > 12:
        fail("internal_name y asset_type juntos deben ocupar como máximo 12 bytes ASCII, incluyendo terminadores")
    data[0x00:0x0C] = packed.ljust(12, b"\0")


def dup_document(data: bytes, name: str) -> dict[str, Any]:
    if len(data) != DUP_SIZE:
        fail(f"{name} debe medir {DUP_SIZE:#x} bytes")
    return {
        "format": "hwtr-dup-v1",
        "source": name,
        "source_sha1": sha1(data),
        "size": len(data),
        "internal_name": read_c_string(data, 0x00, 12),
        "asset_type": read_c_string(data, len(read_c_string(data, 0x00, 12).encode("ascii")) + 1, 12),
        "display_name": read_c_string(data, 0x0C, 20),
        "duration_seconds": struct.unpack_from(">I", data, 0x2C)[0],
        "stacking_mode": struct.unpack_from(">I", data, 0x30)[0],
        "flags": f"0x{struct.unpack_from('>I', data, 0x34)[0]:08X}",
        "multipliers": {
            label: struct.unpack_from(">f", data, offset)[0]
            for label, offset in DUP_MULTIPLIERS.items()
        },
        "velocity_impulse": {
            axis: struct.unpack_from(">f", data, offset)[0]
            for axis, offset in (("x", 0x98), ("y", 0x9C), ("z", 0xA0))
        },
        "steering_multiplier": struct.unpack_from(">f", data, 0xA4)[0],
        "color_delta_u8": list(data[0xA8:0xAC]),
        "notes": {
            "flags": "0x01 brakes; 0x04 turbo; 0x08 4x4; 0x10 steel; 0x20 rubber; 0x40 gyro.",
            "multipliers": "1.0 leaves that vehicle property unchanged. Values are multiplied while the pickup is active.",
            "duration_seconds": "The game compares this value as seconds (value × 1000 ms).",
            "stacking_mode": "Internal pickup-stack behavior; preserve 1 unless testing.",
        },
        "raw_hex": data.hex(),
    }


def export_data(source_dir: Path, output_dir: Path) -> int:
    output_dir.mkdir(parents=True, exist_ok=True)
    files = sorted(list(source_dir.glob("*.CWH")) + list(source_dir.glob("*.DUP")) + list(source_dir.glob("TUNING.PRM")))
    if not files:
        fail(f"no se encontraron .CWH, .DUP ni TUNING.PRM en {source_dir}")
    for source in files:
        data = source.read_bytes()
        if source.suffix == ".CWH":
            document = cwh_document(data, source.name)
        elif source.suffix == ".DUP":
            document = dup_document(data, source.name)
        else:
            if len(data) != TUNING_SIZE:
                fail(f"{source.name} debe medir {TUNING_SIZE:#x} bytes")
            document = raw_document(data, source.name, "tuning")
        (output_dir / f"{source.name}.json").write_text(json.dumps(document, indent=2, allow_nan=False) + "\n")
    return len(files)


def import_data(document_path: Path, original: Path, output: Path) -> None:
    require_distinct(original, output)
    document = json.loads(document_path.read_text())
    if not isinstance(document, dict) or "raw_hex" not in document:
        fail(f"{document_path} no es un documento de asset_workbench")
    old = original.read_bytes()
    expected = document.get("size")
    if not isinstance(expected, int) or len(old) != expected:
        fail("el original no coincide con el tamaño que espera el JSON")
    source_digest = document.get("source_sha1")
    if not isinstance(source_digest, str) or sha1(old) != source_digest:
        fail("el original no coincide con el SHA-1 del que se exportó el JSON")
    try:
        rebuilt = bytearray.fromhex(document["raw_hex"])
    except (TypeError, ValueError) as error:
        fail(f"raw_hex inválido: {error}")
    if len(rebuilt) != len(old):
        fail("raw_hex tiene un tamaño distinto al original")
    if document.get("format") == "hwtr-cwh-v1":
        floats = document.get("f32_be")
        if not isinstance(floats, dict):
            fail("f32_be debe ser un objeto JSON")
        for key, value in floats.items():
            try:
                offset = int(key, 0)
            except (TypeError, ValueError):
                fail(f"offset inválido en f32_be: {key!r}")
            if offset < 8 or offset + 4 > len(rebuilt) or offset % 4:
                fail(f"offset f32 fuera de rango: {key}")
            if not isinstance(value, (int, float)):
                fail(f"{key} debe ser un número")
            struct.pack_into(">f", rebuilt, offset, float(value))
    elif document.get("format") == "hwtr-dup-v1":
        # The first 12 bytes pack two variable-length strings alongside two
        # unknown bytes.  Preserve that region rather than risking a layout
        # change; display_name below is the player-facing editable label.
        write_c_string(rebuilt, 0x0C, 20, document.get("display_name"), "display_name")
        for field, offset in (("duration_seconds", 0x2C), ("stacking_mode", 0x30)):
            value = document.get(field)
            if not isinstance(value, int) or not 0 <= value <= 0xFFFFFFFF:
                fail(f"{field} debe ser un entero sin signo de 32 bits")
            struct.pack_into(">I", rebuilt, offset, value)
        try:
            flags = int(document.get("flags"), 0) if isinstance(document.get("flags"), str) else document.get("flags")
        except ValueError:
            fail("flags debe ser hexadecimal, por ejemplo 0x00000008")
        if not isinstance(flags, int) or not 0 <= flags <= 0xFFFFFFFF:
            fail("flags debe ser un entero sin signo de 32 bits")
        struct.pack_into(">I", rebuilt, 0x34, flags)
        multipliers = document.get("multipliers")
        if not isinstance(multipliers, dict):
            fail("multipliers debe ser un objeto JSON")
        for label, offset in DUP_MULTIPLIERS.items():
            value = multipliers.get(label)
            if not isinstance(value, (int, float)):
                fail(f"multipliers.{label} debe ser un número")
            struct.pack_into(">f", rebuilt, offset, float(value))
        impulse = document.get("velocity_impulse")
        if not isinstance(impulse, dict):
            fail("velocity_impulse debe ser un objeto JSON")
        for axis, offset in (("x", 0x98), ("y", 0x9C), ("z", 0xA0)):
            value = impulse.get(axis)
            if not isinstance(value, (int, float)):
                fail(f"velocity_impulse.{axis} debe ser un número")
            struct.pack_into(">f", rebuilt, offset, float(value))
        steering = document.get("steering_multiplier")
        if not isinstance(steering, (int, float)):
            fail("steering_multiplier debe ser un número")
        struct.pack_into(">f", rebuilt, 0xA4, float(steering))
        color_delta = document.get("color_delta_u8")
        if not isinstance(color_delta, list) or len(color_delta) != 4 or any(not isinstance(n, int) or not 0 <= n <= 255 for n in color_delta):
            fail("color_delta_u8 debe contener cuatro enteros de 0 a 255")
        rebuilt[0xA8:0xAC] = bytes(color_delta)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(rebuilt)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    subparsers = parser.add_subparsers(dest="command", required=True)
    export_bg = subparsers.add_parser("bg-export", help="exporta todos los N64*.BG a PNG")
    export_bg.add_argument("source_dir", type=Path)
    export_bg.add_argument("output_dir", type=Path)
    import_bg = subparsers.add_parser("bg-import", help="convierte un PNG editado a BG usando la paleta original")
    import_bg.add_argument("png", type=Path)
    import_bg.add_argument("original", type=Path)
    import_bg.add_argument("output", type=Path)
    export_data_parser = subparsers.add_parser("data-export", help="exporta física y power-ups a JSON")
    export_data_parser.add_argument("source_dir", type=Path)
    export_data_parser.add_argument("output_dir", type=Path)
    import_data_parser = subparsers.add_parser("data-import", help="reconstruye un CWH, DUP o PRM desde JSON")
    import_data_parser.add_argument("document", type=Path)
    import_data_parser.add_argument("original", type=Path)
    import_data_parser.add_argument("output", type=Path)
    args = parser.parse_args()
    if args.command == "bg-export":
        backgrounds = sorted(args.source_dir.glob("N64*.BG"))
        if not backgrounds:
            fail(f"no se encontraron N64*.BG en {args.source_dir}")
        for source in backgrounds:
            export_background(source, args.output_dir / f"{source.name}.png")
        print(f"{len(backgrounds)} PNG exportados en {args.output_dir}")
    elif args.command == "bg-import":
        import_background(args.png, args.original, args.output)
        print(f"BG reconstruido: {args.output}")
    elif args.command == "data-export":
        count = export_data(args.source_dir, args.output_dir)
        print(f"{count} JSON exportados en {args.output_dir}")
    else:
        import_data(args.document, args.original, args.output)
        print(f"asset reconstruido: {args.output}")


if __name__ == "__main__":
    main()
