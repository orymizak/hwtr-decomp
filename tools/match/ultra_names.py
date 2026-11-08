"""Pone a las funciones identificadas por ultra_id.py su nombre de libultra.

Entrada: líneas "DIRECCIÓN nombre_actual nombre_nuevo archivo". Añade los nombres a symbol_addrs.txt y renombra
las referencias en src/, include/ y rodata_refs.txt. Después hay que regenerar con `make split` y comprobar
con `make clean && make`. Uso: python3 tools/match/ultra_names.py renombres.txt"""
import glob, re, sys
from pathlib import Path

rows = [l.split() for l in Path(sys.argv[1]).read_text().splitlines() if l.strip()]
ren = {old: new for _, old, new, *_ in rows if old.startswith("func_")}
sym = Path("symbol_addrs.txt")
text = sym.read_text()
add = [f"{new} = 0x{addr};  // type:func" for addr, old, new, *_ in rows if old in ren and f"{new} = " not in text]
sym.write_text(text.rstrip("\n") + "\n" + "\n".join(add) + "\n")
pat = re.compile(r"\b(" + "|".join(map(re.escape, ren)) + r")\b")
changed = 0
for f in glob.glob("src/*.c") + glob.glob("include/*.h") + ["rodata_refs.txt"]:
    p = Path(f)
    if not p.exists():
        continue
    s = p.read_text()
    t = pat.sub(lambda m: ren[m.group(1)], s)
    if t != s:
        p.write_text(t)
        changed += 1
print(f"{len(add)} nombres añadidos, {changed} archivos con referencias renombradas")
