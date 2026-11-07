#!/usr/bin/env python3
"""gen_mechanics.py: escribe mechanics.json con las constantes de juego que están en el código.

Cada constante se busca en el ensamblador de su función como f32 inmediato (lui/ori) y el script falla si
no está, así que el archivo solo recoge lo que el código usa tal cual. Los valores que el juego lee de sus
archivos (Tuning.prm, .CWH, .DUP) no son constantes del código: están en las tablas de MECHANICS.md."""
import json, struct, sys
from pathlib import Path

CONSTANTS = [
    ("gravedad", "física", 386.0, "pulgadas/s²", "func_80095970"),
    ("conversión de mph", "física", 17.6, "pulgadas/s por mph", "func_8008D060"),
    ("velocidad de referencia de la dirección", "controles", 2304.0, "pulgadas/s", "func_8008D060"),
    ("límite base de choque", "física", 2112.0, "pulgadas/s", "func_800A66C0"),
    ("pendiente del límite de choque por debajo de 1", "física", 938.6667, "pulgadas/s", "func_800A66C0"),
    ("pendiente del límite de choque por encima de 1", "física", 821.3333, "pulgadas/s", "func_800A66C0"),
    ("resistencia máxima", "física", 4.0, "factor", "func_80091210"),
    ("densidad del aire", "física", 2.377, "milésimas de slug/pie³", "func_800946DC"),
    ("pulgadas a pies", "física", 0.083333336, "factor", "func_800946DC"),
    ("escala de la dificultad", "ia", 255.0, "pasos", "func_800853F4"),
    ("velocidad mínima del ajuste en el aire", "física", 264.0, "pulgadas/s", "func_80092534"),
    ("zona muerta del eje en el aire", "controles", 0.2, "fracción", "func_80092534"),
    ("límite de la dirección de la IA al reaccionar", "ia", 80.0, "unidades", "func_80084B9C"),
    ("límite de la corrección de la dirección de la IA", "ia", 200.0, "unidades", "func_80081840"),
    ("desplazamiento fijo en la referencia de la IA", "ia", 36.0, "pulgadas", "func_80081840"),
]

values = []
for name, group, value, unit, function in CONSTANTS:
    word = struct.unpack(">I", struct.pack(">f", value))[0]
    path = next(Path("asm/nonmatchings").glob(f"*/{function}.s"), None)
    if path is None or f"(0x{word:08X} >> 16)" not in path.read_text():
        sys.exit(f"{name}: no se encuentra 0x{word:08X} en {function}")
    values.append({"name": name, "group": group, "value": value, "unit": unit,
                   "source": f"{function}, f32 inmediato 0x{word:08X}"})

Path("mechanics.json").write_text(json.dumps({"version": 1, "values": values}, ensure_ascii=False, indent=2) + "\n")
print(len(values), "constantes")
