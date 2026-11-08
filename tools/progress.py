#!/usr/bin/env python3
"""Muestra cuánto código está en C, en bytes y en funciones. Requiere un build (make)."""
import glob
import re
import subprocess
import sys
from pathlib import Path

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
        # nonmatching añade un segundo símbolo por función; jtbl_L son etiquetas de jump tables
        if len(parts) == 3 and parts[1] == "T" and "NON_MATCHING" not in parts[2] and not parts[2].startswith("jtbl_L") and CODE_START <= int(parts[0], 16) < CODE_END:
            symbols.append((int(parts[0], 16), parts[2]))

    done = c_functions()
    table = ["| Zona | Funciones | Bytes de código | Avance |", "|---|---|---|---|"]
    table_en = ["| Area | Functions | Code bytes | Progress |", "|---|---|---|---|"]
    english = {"juego": "game", "sistema": "system"}
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
        table.append(f"| {name} | {c_funcs}/{funcs} | {c_bytes}/{size} | {c_bytes / size * 100:.2f} % |")
        table_en.append(f"| {english.get(name, name)} | {c_funcs}/{funcs} | {c_bytes}/{size} | {c_bytes / size * 100:.2f}% |")

    if "--readme" in sys.argv:
        # README.md is in English; README.es.md keeps the Spanish table.
        for path, rows_ in (("README.md", table_en), ("README.es.md", table)):
            readme = Path(path)
            if not readme.exists():
                continue
            text = readme.read_text()
            block = "<!-- progress -->\n" + "\n".join(rows_) + "\n<!-- /progress -->"
            readme.write_text(re.sub(r"<!-- progress -->.*?<!-- /progress -->", block, text, flags=re.S))


if __name__ == "__main__":
    main()
