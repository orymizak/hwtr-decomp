#!/usr/bin/env python3
"""Renombra las etiquetas de jump tables generadas por splat.

binutils 2.6 trata todo símbolo .L como local aunque sea .global, y las tablas en rodata
quedan en otro objeto cuando su función se incluye desde C con INCLUDE_ASM.
"""
import glob
import re

files = glob.glob("asm/**/*.s", recursive=True)
sources = {path: open(path).read() for path in files}

labels = set()
for source in sources.values():
    labels.update(re.findall(r"^\s*jlabel (\.L\w+)", source, re.M))

pattern = re.compile(r"\.L\w+")
for path, source in sources.items():
    fixed = pattern.sub(lambda m: "jtbl_" + m.group(0)[1:] if m.group(0) in labels else m.group(0), source)
    if fixed != source:
        with open(path, "w") as f:
            f.write(fixed)
