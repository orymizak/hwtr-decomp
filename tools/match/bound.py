#!/usr/bin/env python3
"""bound.py <asm.s>: posibles archivos originales: funciones alineadas a 16 con relleno de nop antes."""
import re, struct, sys
rom = open("baserom.z64", "rb").read()
R = lambda v: v - 0x80060400 + 0x1000
txt = open(sys.argv[1]).read()
F = []
for m in re.finditer(r"nonmatching (\S+?), (0x[0-9A-F]+)\n\nglabel \S+\n(?:\s*\.L\S+:\n)*\s+/\* ([0-9A-F]+) ([0-9A-F]{8})", txt):
    F.append((m.group(1), int(m.group(2), 16), int(m.group(4), 16)))
files = [[]]
for i, (n, s, v) in enumerate(F):
    if i and v % 16 == 0:
        pn, ps, pv = F[i - 1]
        if pv + ps < v:  # hueco de relleno
            files.append([])
    files[-1].append((n, s, v))
print(len(F), "funciones;", len(files), "archivos probables")
for fl in files[: int(sys.argv[2]) if len(sys.argv) > 2 else 40]:
    v0 = fl[0][2]
    size = fl[-1][2] + fl[-1][1] - v0
    fp = 0
    lim = R(v0 + size)
    w = rom[R(v0): lim]
    words = struct.unpack(">%dI" % (len(w) // 4), w)
    fp = sum(x == 0x03A0F021 for x in words)  # addu $fp,$sp,$zero
    jt = sum(x >> 26 == 0 and x & 0x3F == 8 and (x >> 21) & 31 != 31 for x in words)
    print(f"ROM {R(v0):06X} VRAM {v0:08X} {len(fl):3d} func {size:#7x} bytes  fp={fp:2d} jr_reg={jt:2d}  {fl[0][0]}..{fl[-1][0]}")
