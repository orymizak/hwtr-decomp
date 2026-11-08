"""insert.py <lote.c> <unidad>: pasa a src/<unidad>.c las funciones de un lote de hand.py, sin comprobar nada.

Lo que va antes de cada definición (typedefs, prototipos, extern) se añade tras los #include si la unidad
no lo tiene ya; los typedefs, antes que el resto, para que los prototipos los vean. Sirve cuando
pass2.merge no puede unir una función por el orden de los tipos. Después hay que compilar y comprobar la ROM."""
import re, sys
from pathlib import Path

lote, unit = sys.argv[1:3]
parts = re.findall(r"^//@ (\w+)[^\n]*\n(.*?)(?=^//@ |\Z)", Path(lote).read_text(), re.S | re.M)
p = Path(f"src/{unit}.c")
c = p.read_text()
head = '#include "include_asm.h"\n'
for name, body in parts:
    m = re.search(r"^[\w \*]+?\b%s\([^;]*?\)\s*\{" % re.escape(name), body, re.M)
    decls, fn = body[:m.start()].strip("\n").splitlines(), body[m.start():].rstrip()
    marker = f'INCLUDE_ASM("asm/nonmatchings/{unit}", {name});'
    if marker not in c:
        print(f"{name}: no está en {unit}")
        continue
    known = c[:c.index(marker)]
    types = [d for d in decls if d.startswith("typedef")]
    others = [d for d in decls if not d.startswith("typedef")]
    add_t = [d for d in types if not re.search(r"\b%s;" % re.escape(re.search(r"(\w+);\s*$", d).group(1)), known)]
    def declared(d):
        f = re.search(r"(\w+)\(", d)
        if f:
            return re.search(r"\b%s\(" % re.escape(f.group(1)), known)
        v = re.search(r"(\w+)\s*(\[[^\]]*\])?;\s*$", d)
        return v and re.search(r"\b%s\b" % re.escape(v.group(1)), known)
    add_o = [d for d in others if d.strip() and d not in known and not declared(d)]
    i = c.index(head) + len(head)
    c = c[:i] + "".join(d + "\n" for d in add_t) + c[i:]
    j = c.index(marker)
    c = c[:j] + "".join(d + "\n" for d in add_o) + fn + c[j + len(marker):]
    print(f"{name}: insertada")
p.write_text(c)
