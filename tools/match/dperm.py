#!/usr/bin/env python3
"""dperm.py [--secs N] [--max-ratio R] [unidades...]: pasa las funciones casi iguales por decomp-permuter.

Requiere tools/decomp-permuter y el mismo entorno que pass2 (TMP_DIR, PYTHONPATH=tools/match, m2c en PATH).
Lo que llega a 0 pasa por el filtro de pass2: commit por unidad solo si la ROM es idéntica."""
import glob, os, re, shutil, stat, subprocess, sys
from pathlib import Path
import pass2, perm
from pass2 import TMP, asm_of, compare, compile_to, convert, section

ROOT = Path.cwd().resolve()
PY = sys.executable
SECS = 30
MAX_RATIO = 0.2
MAX_SIZE = 800
TYPES = (ROOT / "include/common.h").read_text()
TYPEDEFS = "\n".join(l for l in TYPES.splitlines() if l.startswith("typedef"))


def setup(d, unit, name, cand, opt, asm):
    if d.exists():
        shutil.rmtree(d)
    d.mkdir(parents=True)
    (d / "base.c").write_text(TYPEDEFS + "\nvoid *memset(void *dst, int value, u32 size);\n" + cand + "\n")
    (d / "target.s").write_text('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n' + asm + "\n")
    subprocess.check_call(["mips-linux-gnu-as", "-EB", "-march=vr4300", "-mtune=vr4300", "-mabi=32", f"-I{ROOT}/include",
                           "-o", str(d / "target.o"), str(d / "target.s")])
    sh = d / "compile.sh"
    sh.write_text(f'#!/bin/bash\nexec "{ROOT}/tools/gcc_2.7.2/gcc" -B"{ROOT}/tools/gcc_2.7.2/" -c {opt} -G0 -mips3 -mgp32 -mfp32 "$@"\n')
    sh.chmod(sh.stat().st_mode | stat.S_IEXEC)
    (d / "settings.toml").write_text(f'func_name = "{name}"\ncompiler_type = "gcc"\n')


def from_output(src, name):
    """Devuelve el candidato en el formato de m2c: sin typedefs y con la llave en la misma línea."""
    t = re.sub(r"\)\s*\n\s*\{", ") {", src)
    t = re.sub(r"^typedef (?:signed |unsigned )?[\w ]+ \w+;\n", "", t, flags=re.M)
    t = re.sub(r"^void \*memset\(.*\);\n", "", t, flags=re.M)
    t = re.sub(r"typedef struct\s*\{(.*?)\}\s*(\w+);", lambda m: "typedef struct { " + " ".join(m.group(1).split()) + f" }} {m.group(2)};", t, flags=re.S)
    return t.strip() + "\n"


def run(job):
    i, unit, name, opt = job
    r = convert(job)
    if r[1] is not None or r[4] is None or r[3] > MAX_SIZE or perm.body_span(r[4]) is None:
        return r
    d = TMP / "dp" / name
    d.mkdir(parents=True, exist_ok=True)
    v, size, asm = asm_of(unit, name)
    target = perm.target_words(v, size, asm)
    o = compile_to(r[4], d, opt)
    if o is None or section(o, ".rodata", d) or perm.score(o, target, d) > len(target) * MAX_RATIO:
        return r
    setup(d, unit, name, r[4], opt, asm)
    try:
        subprocess.run([PY, str(ROOT / "tools/decomp-permuter/permuter.py"), str(d), "--stop-on-zero", "--best-only", "--quiet", "-j", "4"],
                       capture_output=True, text=True, timeout=SECS, cwd=ROOT)
    except subprocess.TimeoutExpired:
        pass
    for src in sorted(glob.glob(str(d / "output-0-*/source.c"))):
        c = from_output(Path(src).read_text(), name)
        o = compile_to(c, d, opt)
        if o is not None and compare(o, v, size, d) == (0, False):
            print(f"  {name}: 0", flush=True)
            return name, c, 0, size, c
    return r


def main():
    global SECS, MAX_RATIO
    args = sys.argv[1:]
    if "--secs" in args:
        i = args.index("--secs")
        SECS = int(args[i + 1])
        del args[i:i + 2]
    if "--max-ratio" in args:
        i = args.index("--max-ratio")
        MAX_RATIO = float(args[i + 1])
        del args[i:i + 2]
    units = args or sorted(Path(f).stem for f in glob.glob("src/*.c"))
    total = 0
    for u in units:
        # Dos a la vez: cada decomp-permuter ya usa cuatro núcleos.
        pass2.ThreadPoolExecutor = lambda n: pass2.ThreadPoolExecutor0(2)
        n = pass2.process(u, True, run)
        total += n
        print(f"{u}: +{n} (total {total})", flush=True)
    pass2.flush()


pass2.ThreadPoolExecutor0 = pass2.ThreadPoolExecutor
if __name__ == "__main__":
    main()
