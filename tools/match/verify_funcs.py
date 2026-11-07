#!/usr/bin/env python3
"""verify_funcs.py: comprueba las funciones en C ya enlazadas, en sus direcciones finales.

Cada función en src/ debe estar en la dirección que dice su nombre y sus bytes enlazados (con las
reubicaciones ya resueltas) deben ser los de baserom.z64. Sale con 1 si alguna no coincide."""
import glob, re, subprocess, sys
from pathlib import Path

ROM = Path("baserom.z64").read_bytes()
BUILT = Path("build/hotwheelsturbo.z64").read_bytes()
CODE = (0x80060450, 0x800E7AA0)
R = lambda v: v - 0x80060400 + 0x1000


def c_functions():
    names = set()
    for path in glob.glob("src/*.c"):
        text = re.sub(r"/\*.*?\*/", "", Path(path).read_text(), flags=re.S)
        names.update(re.findall(r"^[\w* ]*?\b(\w+)\([^;{\n]*\) \{$", text, re.M))
    return names


def main():
    symbols = []
    for line in subprocess.check_output(["mips-linux-gnu-nm", "-n", "build/hotwheelsturbo.elf"], text=True).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in "Tt" and "NON_MATCHING" not in parts[2] and not parts[2].startswith("jtbl_L") and CODE[0] <= int(parts[0], 16) < CODE[1]:
            symbols.append((int(parts[0], 16), parts[2]))
    done, bad, checked, total = c_functions(), [], 0, 0
    for i, (addr, name) in enumerate(symbols):
        if name not in done:
            continue
        end = symbols[i + 1][0] if i + 1 < len(symbols) else CODE[1]
        m = re.fullmatch(r"func_([0-9A-F]{8})", name)
        if m and int(m.group(1), 16) != addr:
            bad.append(f"{name}: enlazada en {addr:08X}")
        if ROM[R(addr):R(end)] != BUILT[R(addr):R(end)]:
            bad.append(f"{name}: bytes distintos en {addr:08X}")
        checked += 1
        total += end - addr
    print(f"{checked} funciones en C, {total} bytes: {'todas coinciden' if not bad else str(len(bad)) + ' no coinciden'} en su dirección final")
    for line in bad[:40]:
        print(" ", line)
    sys.exit(1 if bad else 0)


main()
