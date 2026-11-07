#!/usr/bin/env python3
"""Genera rodata_refs.txt: qué dobles y tablas de saltos de .rodata usa cada función, en orden.

Se ejecuta con el código aún en asm/nonmatchings. Las entradas existentes se conservan, porque una
función ya convertida a C no tiene ensamblador del que volver a sacarlas."""
import glob, re
from pathlib import Path

OUT = Path("rodata_refs.txt")
RODATA = (0x800FE7B0, 0x80101E70)
PATTERN = re.compile(r"\*/\s+(ldc1|lw)\s+\S+\s+%lo\(((?:D|jtbl)_([0-9A-F]{8}))(?: \+ 0x[0-9A-F]+)?\)")

refs = {}
if OUT.exists():
    for line in OUT.read_text().splitlines():
        parts = line.split()
        if parts and not parts[0].startswith("#"):
            refs[parts[0]] = parts[1:]
for path in glob.glob("asm/nonmatchings/*/*.s"):
    found = []
    for op, sym, addr in PATTERN.findall(Path(path).read_text()):
        if sym.startswith("jtbl_") and op == "lw":
            found.append(sym)
        elif op == "ldc1" and RODATA[0] <= int(addr, 16) < RODATA[1]:
            found.append(sym)
    if found:
        refs[Path(path).stem] = found
OUT.write_text("# función: símbolos de .rodata que usa, en orden. Lo genera tools/match/gen_rodata_refs.py\n"
               + "".join(f"{name} {' '.join(syms)}\n" for name, syms in sorted(refs.items())))
print(len(refs), "funciones")
