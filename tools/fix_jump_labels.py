#!/usr/bin/env python3
"""Prepara las etiquetas de jump tables generadas por splat para binutils 2.6.

binutils 2.6 trata todo símbolo .L como local aunque sea .global, y las tablas en rodata
quedan en otro objeto cuando su función se incluye desde C con INCLUDE_ASM. Por eso se
renombran y jlabel pasa a ser global; splat regenera include/labels.inc en cada split.
"""
import glob
import re

files = glob.glob("asm/**/*.s", recursive=True)
sources = {path: open(path).read() for path in files}

labels = set()
for source in sources.values():
    labels.update(re.findall(r"^\s*jlabel (\.L\w+)", source, re.M))
    # Una función ya en C no tiene jlabel, pero la tabla de la ROM sigue nombrando sus etiquetas.
    labels.update(re.findall(r"\.word (\.L\w+)", source))

pattern = re.compile(r"\.L\w+")
for path, source in sources.items():
    fixed = pattern.sub(lambda m: "jtbl_" + m.group(0)[1:] if m.group(0) in labels else m.group(0), source)
    if fixed != source:
        with open(path, "w") as f:
            f.write(fixed)

with open("include/labels.inc") as f:
    labels_inc = f.read()
with open("include/labels.inc", "w") as f:
    f.write(labels_inc.replace(".macro jlabel label, visibility=local", ".macro jlabel label, visibility=global"))
