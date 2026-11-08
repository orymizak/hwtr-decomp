#!/usr/bin/env python3
"""unit_cmp.py <unidad>...: compila cada unidad sola y compara su .text con la ROM, sin compilar la ROM entera.

Las reubicaciones se comparan sin su valor (no hay enlazado), así que no sustituye a la comprobación del
SHA-1, pero detecta cualquier diferencia de código. Sale con 1 si alguna unidad no coincide."""
import re, struct, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, "tools")
import cc

ROM = Path("baserom.z64").read_bytes()
R = lambda v: v - 0x80060400 + 0x1000
RELOC_OPS = {9, 15, 32, 33, 35, 36, 37, 40, 41, 43, 49, 53, 57, 61}


def mask(w):
    op = w >> 26
    if op in (2, 3):
        return w & 0xFC000000
    return w & 0xFFFF0000 if op in RELOC_OPS else w


def unit_start(unit):
    m = re.search(r"- \[0x([0-9A-F]+), c, %s\]" % re.escape(unit), Path("hotwheelsturbo.yaml").read_text())
    return int(m.group(1), 16) - 0x1000 + 0x80060400


def opt_of(unit):
    m = re.search(r"\$\(BUILD\)/src/%s\.o: OPTFLAGS := (\S+)" % re.escape(unit), Path("Makefile").read_text())
    return m.group(1) if m else "-O2"


def main():
    bad = 0
    db = cc.Refs()
    for unit in sys.argv[1:]:
        with tempfile.TemporaryDirectory(dir=".scratch") as d:
            o = Path(d) / "u.o"
            code, err = cc.compile_c(opt_of(unit), f"src/{unit}.c", str(o), db)
            if code:
                print(f"{unit}: no compila\n{err[:400]}")
                bad += 1
                continue
            subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", str(o), str(o) + ".bin"])
            x = Path(str(o) + ".bin").read_bytes()
            n = len(x.rstrip(b"\0")) // 4 * 4
            start = R(unit_start(unit))
            diff = [i for i in range(0, n, 4)
                    if mask(struct.unpack_from(">I", ROM, start + i)[0]) != mask(struct.unpack_from(">I", x, i)[0])]
            print(f"{unit}: {n // 4 - len(diff)}/{n // 4} palabras iguales" + ("" if not diff else f", primera diferencia en +0x{diff[0]:X}"))
            bad += bool(diff)
    sys.exit(1 if bad else 0)


main()
