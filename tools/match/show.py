#!/usr/bin/env python3
"""show.py <función>...: lo necesario para escribir una función a mano: tamaño, su .rodata y lo que da m2c."""
import glob, re, subprocess, sys, tempfile
from pathlib import Path


def find(name):
    hits = glob.glob(f"asm/nonmatchings/*/{name}.s")
    return Path(hits[0]) if hits else None


for name in sys.argv[1:]:
    path = find(name)
    if not path:
        print(f"// {name}: sin ensamblador")
        continue
    unit = path.parent.name
    text = path.read_text()
    size = int(re.search(r"nonmatching \S+?, (0x[0-9A-F]+)", text).group(1), 16)
    print(f"// ===== {unit} {name} tamaño {size:#x}")
    with tempfile.TemporaryDirectory() as d:
        f = Path(d) / "f.s"
        f.write_text(re.search(r"(glabel .*)", text, re.S).group(1))
        args = ["m2c", "--target", "mips-gcc-c", "--context", "ghidra_work/context.h.m2c", str(f)]
        ro = Path(f"asm/data/{unit}.rodata.s")
        if ro.exists():
            r = Path(d) / "r.s"
            r.write_text(re.sub(r'^\.include.*$|^nonmatching.*$|^enddlabel.*$', "", ro.read_text(), flags=re.M).replace("dlabel", "glabel"))
            args.append(str(r))
        r = subprocess.run(args, capture_output=True, text=True)
        if r.returncode:
            args.remove("--context")
            args.remove("ghidra_work/context.h.m2c")
            r = subprocess.run(args, capture_output=True, text=True)
        print(r.stdout if r.returncode == 0 else "// m2c falló: " + r.stderr[:300])
