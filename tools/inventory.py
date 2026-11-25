#!/usr/bin/env python3
"""Inventario de pendientes: funciones en INCLUDE_ASM / asm puro, unidades incompletas, bytes.
Requiere un build (make). OJO: sus cifras (tamaño por distancia entre símbolos, "hecha" = tiene C) difieren de objdiff, que es la referencia oficial; ver METRICS.md. Uso: tools/inventory.py [--top N]"""
import re, subprocess, sys, collections
from pathlib import Path
ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import progress as P

syms = []
for l in subprocess.check_output([P.NM, "-n", str(ROOT / P.ELF)], text=True).splitlines():
    p = l.split()
    if len(p) == 3 and p[1] == "T" and "NON_MATCHING" not in p[2] and not p[2].startswith("jtbl_L") and P.CODE_START <= int(p[0], 16) < P.CODE_END:
        syms.append((int(p[0], 16), p[2]))
size = {s: (syms[i + 1][0] if i + 1 < len(syms) else P.CODE_END) - a for i, (a, s) in enumerate(syms)}
done = P.c_functions()
pending = {s for _, s in syms} - done

inc = {}  # func -> unit
for f in sorted((ROOT / "src").glob("*.c")):
    for m in re.finditer(r'INCLUDE_ASM\("[^"]*",\s*(\w+)\)', f.read_text()):
        inc[m.group(1)] = f.stem
rest = sorted(pending - set(inc))  # sin INCLUDE_ASM en src (asm puro / libc)
units = collections.Counter(inc.values())
dbt = collections.Counter()
for l in (ROOT / "tools/asm_allowlist.txt").read_text().splitlines():
    if l and not l.startswith("#"):
        dbt[l.split("\t")[0]] += 1

tot = sum(size.values())
pb = sum(size[s] for s in pending)
print(f"funciones: {len(syms)} | en C {len(done & set(size))} | pendientes {len(pending)} "
      f"(INCLUDE_ASM {len(inc)}, sin INCLUDE_ASM {len(rest)})")
print(f"bytes: {tot - pb}/{tot} en C ({(tot - pb) / tot * 100:.2f}%), pendientes {pb}")
print(f"unidades src con INCLUDE_ASM: {len(units)} | match-debt (allowlist): {sum(dbt.values())} hits en {len(dbt)} archivos")
n = int(sys.argv[sys.argv.index("--top") + 1]) if "--top" in sys.argv else 25
print(f"\nmayores pendientes (bytes):")
for s in sorted(pending, key=lambda s: -size[s])[:n]:
    print(f"  {size[s]:5} {s:34} {inc.get(s, '(asm puro)')}")
print("\nsin INCLUDE_ASM:", ", ".join(f"{s}({size[s]})" for s in rest[:30]), "..." if len(rest) > 30 else "")

if "--mid" in sys.argv:
    lo, hi = 200, 1700
    c = sorted(((size[s], s) for s in pending if lo <= size[s] <= hi), reverse=True)
    print(f"\nmedianas {lo}-{hi} B: {len(c)} funciones, {sum(z for z, _ in c)} B")
    for z, s in c:
        print(f"  {z:5} {s:34} {inc.get(s, '(asm puro)')}")
