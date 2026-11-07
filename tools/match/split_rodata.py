#!/usr/bin/env python3
"""split_rodata.py --plan | --apply N: divide .rodata por unidad en hotwheelsturbo.yaml.

La .rodata real empieza en 0x800FE7B0 (ROM 0x9F3B0): lo anterior a la división estaba marcado como .data.
Cada unidad empieza en una dirección alineada a 16, que es como el enlazador coloca cada .rodata.
--plan imprime los cortes; --apply N deja en el yaml los N primeros y el resto sin dividir."""
import glob, re, sys
from pathlib import Path

YAML = Path("hotwheelsturbo.yaml")
CUTS = Path("tools/match/rodata_cuts.txt")
START, END = 0x800FE7B0, 0x80101E70
ROM = lambda v: v - 0x80060400 + 0x1000
SYM = re.compile(r"\b(?:D|jtbl|STR)_([0-9A-F]{8})\b")
# Lo que ya emite su propio C: no se toca.
KEEP = {0x9F880: "file_table"}


def order():
    names = []
    for name in re.findall(r"build/src/(\w+)\.o\(\.text\)", Path("hotwheelsturbo.ld").read_text()):
        if name not in names:
            names.append(name)
    return names


def symbols():
    found = set()
    for path in glob.glob("asm/data/*.s"):
        for m in re.finditer(r"^dlabel \w+\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", Path(path).read_text(), re.M):
            v = int(m.group(1), 16)
            if START <= v < END:
                found.add(v)
    return sorted(found)


def owners():
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


def plan():
    rank = {u: i for i, u in enumerate(order())}
    syms, refs = symbols(), owners()
    runs, last = [], -1
    for v in syms:
        us = [u for u in refs.get(v, ()) if u in rank and rank[u] >= last]
        if not us:
            continue
        u = min(us, key=rank.get)
        if not runs or runs[-1][1] != u:
            runs.append([v, u, v])
        runs[-1][2] = v
        last = rank[u]
    cuts = []
    for i, (first, unit, _) in enumerate(runs):
        prev_last = runs[i - 1][2] if i else START - 1
        aligned = [v for v in syms if prev_last < v <= first and v % 16 == 0]
        if not aligned:
            print(f"sin corte alineado para {unit} en {first:08X}", file=sys.stderr)
            continue
        cuts.append((aligned[-1] if i else START, unit))
    return [(ROM(v), u) for v, u in cuts]


def saved_plan():
    """Los cortes acordados, guardados para que aplicarlos de uno en uno no dependa de lo que haya en asm/."""
    return [(int(a, 16), u) for a, u in (l.split() for l in CUTS.read_text().splitlines())]


def apply(n):
    all_cuts = saved_plan()
    cuts = all_cuts[:n]
    lines = YAML.read_text().splitlines()
    begin = next(i for i, l in enumerate(lines) if "[0x928B0, data]" in l)
    end = next(i for i, l in enumerate(lines) if "type: bss, vram: 0x80101E70" in l)
    entries = {0x928B0: "      - [0x928B0, data]"}
    entries[0x9F3B0] = "      - [0x9F3B0, rodata]"
    for rom, unit in cuts:
        if rom not in KEEP:
            entries[rom] = f"      - [0x{rom:X}, rodata, {unit}]"
    for rom, unit in KEEP.items():
        entries[rom] = f"      - [0x{rom:X}, .rodata, {unit}]"
    # font y screen no emiten .rodata desde su C.
    entries[0xA0390] = "      - [0xA0390, rodata, font]"
    entries[0xA03F0] = "      - [0xA03F0, rodata, screen]"
    # El resto sin dividir sigue en un bloque anónimo tras cada corte confirmado.
    if len(cuts) < len(all_cuts):
        rest = all_cuts[n][0]
        entries.setdefault(rest, f"      - [0x{rest:X}, rodata]")
    for after in (0xA03B0, 0xA0420):
        entries.setdefault(after, f"      - [0x{after:X}, rodata]")
    lines[begin:end] = [entries[k] for k in sorted(entries)]
    YAML.write_text("\n".join(lines) + "\n")


if __name__ == "__main__":
    if sys.argv[1] == "--plan":
        for rom, unit in plan():
            print(f"0x{rom:X} {unit}")
    else:
        apply(int(sys.argv[2]))
