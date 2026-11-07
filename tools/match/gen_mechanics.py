#!/usr/bin/env python3
"""gen_mechanics.py: escribe mechanics.json leyendo los valores de los archivos del juego, para que no haya erratas.

Los usos están explicados en MECHANICS.md. Se ejecuta desde la raíz del repo, tras `make extract`."""
import json, struct
from pathlib import Path

FILES = Path("assets/files")
TUNING = (FILES / "TUNING.PRM").read_bytes()
values = []


def add(name, group, value, unit, source):
    values.append({"name": name, "group": group, "value": value, "unit": unit, "source": source})


def tuning(name, group, offset, unit, source):
    add(name, group, TUNING[offset], unit, f"TUNING.PRM +0x{offset:02X}, {source}")


add("gravedad", "física", 386, "pulgadas/s²", "func_80095970")
add("densidad del aire", "física", 0.002377, "slug/pie³", "func_800946DC")
add("conversión de mph", "física", 17.6, "pulgadas/s por mph", "func_8008D060")
add("velocidad de referencia de dirección", "controles", 2304, "pulgadas/s", "func_8008D060")
add("límite base de choque", "física", 2112, "pulgadas/s", "func_800A66C0")
add("resistencia máxima", "física", 4.0, "factor", "func_80091210")

for name, group, offset, unit, source in [
    ("velocidad de la IA, dificultad mínima", "ia", 0x00, "%", "func_800853F4"),
    ("velocidad de la IA, dificultad máxima", "ia", 0x01, "%", "func_800853F4"),
    ("mínimo de corrección, dificultad mínima", "ia", 0x02, "%", "func_800853F4"),
    ("mínimo de corrección, dificultad máxima", "ia", 0x03, "%", "func_800853F4"),
    ("separación objetivo, dificultad mínima", "ia", 0x04, "décimas", "func_800853F4"),
    ("separación objetivo, dificultad máxima", "ia", 0x05, "décimas", "func_800853F4"),
    ("máximo de corrección, dificultad mínima", "ia", 0x0C, "%", "func_800853F4"),
    ("máximo de corrección, dificultad máxima", "ia", 0x0D, "%", "func_800853F4"),
    ("margen de separación, dificultad mínima", "ia", 0x2E, "décimas", "func_800853F4"),
    ("margen de separación, dificultad máxima", "ia", 0x2F, "décimas", "func_800853F4"),
    ("ajuste en la última vuelta", "ia", 0x34, "décimas", "func_800853F4"),
    ("periodo de corrección", "ia", 0x1B, "×10 ms", "func_80081840"),
    ("paso al alejarse la IA", "ia", 0x1C, "centésimas", "func_80081840"),
    ("paso dentro del margen, hacia arriba", "ia", 0x1D, "centésimas", "func_80081840"),
    ("paso dentro del margen, hacia abajo", "ia", 0x1E, "centésimas", "func_80081840"),
    ("paso al quedarse atrás la IA", "ia", 0x1F, "centésimas", "func_80081840"),
    ("velocidad de inicio de reducción de dirección", "controles", 0x08, "mph", "func_8008D060"),
    ("dirección a velocidad máxima", "controles", 0x09, "%", "func_8008D060"),
    ("dirección al empezar la reducción", "controles", 0x3A, "%", "func_8008D060"),
    ("viento aerodinámico", "física", 0x0A, "mph", "func_800946DC"),
    ("resistencia de la IA, dificultad mínima", "física", 0x0E, "%", "func_80091210"),
    ("resistencia de la IA, dificultad máxima", "física", 0x0F, "%", "func_80091210"),
    ("resistencia del jugador, dificultad mínima", "física", 0x10, "%", "func_80091210"),
    ("resistencia del jugador, dificultad máxima", "física", 0x11, "%", "func_80091210"),
]:
    tuning(name, group, offset, unit, source)

FIELDS = [
    ("dirección máxima", 0x4CC), ("masa", 0x4D0), ("empuje", 0x4DC), ("reparto del empuje", 0x4E0),
    ("arrastre", 0x4E4), ("agarre eje 1", 0x4FC), ("agarre eje 2", 0x514), ("ritmo de la IA", 0x534),
    ("resistencia a choques", 0x53C),
]
for car in ("DEORA", "TWINMILL"):
    data = (FILES / f"{car}.CWH").read_bytes()
    for name, field in FIELDS:
        offset = field - 0x4CC + 8
        value = round(struct.unpack_from(">f", data, offset)[0], 4)
        add(f"{car} {name}", "coches", value, "", f"{car}.CWH +0x{offset:02X} (campo +0x{field:X})")

Path("mechanics.json").write_text(json.dumps({"version": 1, "values": values}, ensure_ascii=False, indent=2) + "\n")
print(len(values), "valores")
