#!/usr/bin/env python3
"""rodata_owners.py [inicio_vram] [fin_vram]: a qué unidad pertenece cada símbolo de datos de solo lectura.

Busca quién referencia cada símbolo en asm/nonmatchings y en src/, y comprueba que el orden en la ROM
siga el orden de enlazado de las unidades, que es lo que permite dividir .rodata por unidad."""
import glob, re, sys
from pathlib import Path

START = int(sys.argv[1], 16) if len(sys.argv) > 1 else 0x800FC000
END = int(sys.argv[2], 16) if len(sys.argv) > 2 else 0x80101E70
SYM = re.compile(r"\b(?:D|jtbl|STR)_([0-9A-F]{8})\b")


def unit_order():
    text = Path("hotwheelsturbo.ld").read_text()
    order = []
    for name in re.findall(r"build/src/(\w+)\.o\(\.text\)", text):
        if name not in order:
            order.append(name)
    return order


def data_symbols():
    syms = {}
    for path in sorted(glob.glob("asm/data/*.s")):
        for m in re.finditer(r"^dlabel (\w+)\n(.*?)^enddlabel", Path(path).read_text(), re.M | re.S):
            addr = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8})", m.group(2))
            if not addr:
                continue
            v = int(addr.group(1), 16)
            if START <= v < END:
                syms[v] = dict(name=m.group(1), file=Path(path).name, head=m.group(2).split("\n")[0].split("*/")[-1].strip()[:40])
    return syms


def references():
    refs = {}
    for path in glob.glob("asm/nonmatchings/*/*.s"):
        unit = Path(path).parent.name
        for a in SYM.findall(Path(path).read_text()):
            refs.setdefault(int(a, 16), set()).add(unit)
    for path in glob.glob("src/*.c"):
        unit = Path(path).stem
        for a in SYM.findall(Path(path).read_text()):
            refs.setdefault(int(a, 16), set()).add(unit)
    return refs


def main():
    order = unit_order()
    rank = {u: i for i, u in enumerate(order)}
    syms, refs = data_symbols(), references()
    last = -1
    runs = []
    for v in sorted(syms):
        owners = sorted(refs.get(v, ()), key=lambda u: rank.get(u, -1))
        r = [rank[u] for u in owners if u in rank]
        flag = ""
        if r and min(r) < last:
            flag = "FUERA DE ORDEN"
        if r:
            last = max(last, min(r))
        runs.append((v, syms[v], owners, flag))
    for v, s, owners, flag in runs:
        print(f"{v:08X} {s['file'][:14]:14} {s['name'][:22]:22} {','.join(owners)[:40]:40} {s['head']} {flag}")


main()
