#!/usr/bin/env python3
"""targets.py: lista lo que sigue en INCLUDE_ASM y usa tablas de saltos, dobles de .rodata o argumentos en pila."""
import glob, re
from pathlib import Path

JTBL = re.compile(r"%lo\((jtbl_[0-9A-F]{8})\)")
FRAME = re.compile(r"addiu\s+\$sp, \$sp, -(0x[0-9A-F]+)")
STACK_ARG = re.compile(r"\*/\s+(lw|lh|lhu|lb|lbu|lwc1|ldc1)\s+\S+,\s+(0x[0-9A-F]+)\(\$sp\)")
STACK_OUT = re.compile(r"\*/\s+(sw|swc1|sdc1)\s+\S+,\s+(0x[0-9A-F]+)\(\$sp\)")
RODATA_DOUBLE = re.compile(r"ldc1\s+\S+\s+%lo\(D_(800F[E-F]|8010[01])")


def scan():
    rows = []
    for src in sorted(glob.glob("src/*.c")):
        unit = Path(src).stem
        for name in re.findall(r'INCLUDE_ASM\("asm/nonmatchings/\w+", (\w+)\)', Path(src).read_text()):
            text = Path(f"asm/nonmatchings/{unit}/{name}.s").read_text()
            size = int(re.search(r"nonmatching \S+?, (0x[0-9A-F]+)", text).group(1), 16)
            frame = FRAME.search(text)
            frame = int(frame.group(1), 16) if frame else 0
            incoming = sorted({int(o, 16) - frame for _, o in STACK_ARG.findall(text) if int(o, 16) - frame >= 0x10})
            outgoing = sorted({int(o, 16) for _, o in STACK_OUT.findall(text) if 0x10 <= int(o, 16) < 0x30 and frame})
            home = sorted({int(o, 16) - frame for o in re.findall(r"sw\s+\$a[0-3], (0x[0-9A-F]+)\(\$sp\)", text) if 0 <= int(o, 16) - frame < 0x10})
            rows.append(dict(unit=unit, name=name, size=size, frame=frame, jtbl=sorted(set(JTBL.findall(text))),
                             incoming=incoming, home=home, outgoing=outgoing, doubles=len(RODATA_DOUBLE.findall(text))))
    return rows


def main():
    rows = scan()
    for title, key in (("tablas de saltos", "jtbl"), ("argumentos en pila", "incoming"), ("escribe en su zona de argumentos", "home"), ("dobles de .rodata", "doubles")):
        sel = sorted((r for r in rows if r[key]), key=lambda r: r["size"])
        print(f"== {title}: {len(sel)} funciones, {sum(r['size'] for r in sel)} bytes")
        for r in sel:
            print(f"  {r['unit']:14} {r['name']:16} {r['size']:6} marco={r['frame']:#x} {r[key]}")


if __name__ == "__main__":
    main()
