#!/usr/bin/env python3
"""Muestra cuánto código está en C, en bytes y en funciones. Requiere un build (make)."""
import glob
import re
import subprocess

ELF = "build/hotwheelsturbo.elf"
NM = "mips-linux-gnu-nm"

ZONES = [
    ("juego", 0x80060450, 0x800CD600),
    ("audio", 0x800CD600, 0x800D5560),
    ("sistema", 0x800D5560, 0x800D7CB0),
    ("libultra", 0x800D7CB0, 0x800E6990),
    ("libc", 0x800E6990, 0x800E7AA0),
]
CODE_START = ZONES[0][1]
CODE_END = ZONES[-1][2]


def c_functions():
    names = set()
    for path in glob.glob("src/*.c"):
        with open(path) as f:
            source = re.sub(r"/\*.*?\*/", "", f.read(), flags=re.S)
        names.update(re.findall(r"^[\w* ]*?\b(\w+)\([^;{\n]*\) \{$", source, re.M))
    return names


def main():
    symbols = []
    for line in subprocess.check_output([NM, "-n", ELF], text=True).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in "Tt" and CODE_START <= int(parts[0], 16) < CODE_END:
            symbols.append((int(parts[0], 16), parts[2]))

    done = c_functions()
    print(f"{'zona':10} {'funciones':>13} {'bytes':>17} {'avance':>8}")
    rows = [("total", CODE_START, CODE_END)] + ZONES
    for name, start, end in rows:
        funcs = c_funcs = c_bytes = 0
        for i, (addr, symbol) in enumerate(symbols):
            if start <= addr < end:
                funcs += 1
                if symbol in done:
                    c_funcs += 1
                    next_addr = symbols[i + 1][0] if i + 1 < len(symbols) else CODE_END
                    c_bytes += next_addr - addr
        size = end - start
        print(f"{name:10} {c_funcs:5}/{funcs:<7} {c_bytes:8}/{size:<8} {c_bytes / size * 100:7.2f}%")


if __name__ == "__main__":
    main()
