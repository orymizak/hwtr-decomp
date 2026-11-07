#!/usr/bin/env python3
"""try.py <unidad> <función> [-O2] [archivo.c]: compila un candidato y lo compara con la ROM instrucción a instrucción.
Sin archivo.c imprime lo que da m2c. Se ejecuta desde la raíz del repo, con m2c en el PATH."""
import difflib, re, struct, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, "tools")
import cc

TC = "tools/gcc_2.7.2/"
ROM = Path("baserom.z64").read_bytes()
# Operaciones cuyo inmediato puede ser una reubicación: se comparan sin él.
RELOC_OPS = {2, 3, 9, 15, 32, 33, 35, 36, 37, 40, 41, 43, 49, 53, 57, 61}


def asm_of(unit, name):
    t = Path(f"asm/nonmatchings/{unit}/{name}.s").read_text()
    size = int(re.search(r"nonmatching \S+?, (0x[0-9A-F]+)", t).group(1), 16)
    rom = int(re.search(r"/\* ([0-9A-F]+) [0-9A-F]{8}", t).group(1), 16)
    return t, rom, size


def rom_lines(t):
    res = []
    for line in t.splitlines():
        m = re.match(r"\s*/\* \S+ \S+ [0-9A-F]{8} \*/\s+(\S+)\s*(.*)", line)
        if m:
            res.append(m.group(1) + " " + re.sub(r"\s+", " ", m.group(2)))
    return res


def mask(w):
    op = w >> 26
    if op in (2, 3):
        return w & 0xFC000000
    return w & 0xFFFF0000 if op in RELOC_OPS else w


def main():
    args = sys.argv[1:]
    unit, name = args[0], args[1]
    opt = next((x for x in args if x.startswith("-O")), None)
    if not opt:
        m = re.search(r"\$\(BUILD\)/src/%s\.o: OPTFLAGS := (\S+)" % re.escape(unit), Path("Makefile").read_text())
        opt = m.group(1) if m else "-O2"
    files = [x for x in args[2:] if not x.startswith("-")]
    t, rom, size = asm_of(unit, name)
    with tempfile.TemporaryDirectory() as d:
        d = Path(d)
        if not files:
            (d / "f.s").write_text(re.search(r"(glabel .*)", t, re.S).group(1))
            r = subprocess.run(["m2c", "--target", "mips-gcc-c", str(d / "f.s")], capture_output=True, text=True)
            print(r.stdout)
            print(r.stderr[:500])
            return
        (d / "a.c").write_text('#include "common.h"\n' + Path(files[0]).read_text())
        code, err = cc.compile_c(opt, d / "a.c", d / "a.o")
        if code:
            print(err)
            sys.exit(1)
        subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", str(d / "a.o"), str(d / "t.bin")])
        subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".rodata", str(d / "a.o"), str(d / "r.bin")])
        x = (d / "t.bin").read_bytes()
        rodata = (d / "r.bin").stat().st_size if (d / "r.bin").exists() else 0
        x = x[:len(x) - (len(x) - len(x.rstrip(b"\0"))) // 4 * 4]
        ours = [struct.unpack_from(">I", x, i)[0] for i in range(0, len(x), 4)]
        theirs = [struct.unpack_from(">I", ROM, rom + i)[0] for i in range(0, size, 4)]
        a, b = [f"{mask(w):08X}" for w in theirs], [f"{mask(w):08X}" for w in ours]
        lines = rom_lines(t)
        dump = subprocess.run(["mips-linux-gnu-objdump", "-d", "-r", "-j", ".text", "-M", "reg-names=numeric", str(d / "a.o")],
                              capture_output=True, text=True).stdout
        ins = [l.split("\t", 2) for l in dump.splitlines() if re.match(r"\s+[0-9a-f]+:\t", l)]
        sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
        same = sum(block.size for block in sm.get_matching_blocks())
        print(f"iguales {same}/{max(len(a), len(b))}  tamaño {len(x)}/{size}  rodata {rodata}")
        for op, i1, i2, j1, j2 in sm.get_opcodes():
            if op == "equal":
                continue
            for i in range(i1, i2):
                print(f"- {i * 4:4X} {lines[i] if i < len(lines) else ''}")
            for j in range(j1, j2):
                print(f"+ {j * 4:4X} {ins[j][2] if j < len(ins) and len(ins[j]) > 2 else ''}")
            print()


main()
