"""Identifica funciones de libultra/libaudio que siguen en ensamblador comparándolas con ultralib compilado.

Compila cada .c de tools/ultralib/src con el GCC 2.7.2 del repo y compara las funciones del .o con la ROM,
enmascarando los campos que llevan reubicación. No toca src/. Uso:
  python3 tools/match/ultra_id.py [--version K] [--opt -O3] [--all] > salida.txt
--all compara también las funciones ya escritas en C (sirve para ponerles nombre).
Necesita build/hotwheelsturbo.elf. Escribe una línea por coincidencia: dirección, nombre en el repo, función y archivo."""
import glob, os, re, shutil, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
UL = ROOT / "tools/ultralib"
GCC = ROOT / "tools/gcc_2.7.2/gcc"
ROM = (ROOT / "baserom.z64").read_bytes()
R = lambda v: v - 0x80060400 + 0x1000
LO, HI = 0x800CD600, 0x800E7AA0  # audio, sistema, libultra y libc


def arg(flag, default):
    return sys.argv[sys.argv.index(flag) + 1] if flag in sys.argv else default


VERSION, OPT = arg("--version", "K"), arg("--opt", "-O3")
ALL = "--all" in sys.argv


def pending():
    """Funciones en INCLUDE_ASM dentro del rango, con su tamaño hasta el siguiente símbolo."""
    names = set()
    for f in glob.glob(str(ROOT / "src/*.c")):
        names |= set(re.findall(r'INCLUDE_ASM\("[^"]+", (\w+)\)', Path(f).read_text()))
    elf = Path(tempfile.mkdtemp()) / "a.elf"
    shutil.copy(ROOT / "build/hotwheelsturbo.elf", elf)
    out = subprocess.check_output(["mips-linux-gnu-nm", "-n", str(elf)], text=True)
    syms = sorted({(int(a, 16), n) for a, t, n in (l.split() for l in out.splitlines() if len(l.split()) == 3)
                   if t in "tT" and not n.startswith((".", "L", "jtbl", "gcc2_compiled"))})
    addrs = sorted({a for a, _ in syms})
    res = {}
    for a, n in syms:
        if a in res:
            continue
        if (ALL or n in names) and LO <= a < HI:
            nxt = next((b for b in addrs if b > a), HI)
            res[a] = (n, nxt - a)
    return res


def compile_one(c):
    d = Path(tempfile.mkdtemp())
    o = d / "a.o"
    gbi = "-DF3D_GBI" if c.endswith(("parse_gbi.c", "sprite.c")) else "-DF3DEX_GBI"
    flags = ["-w", "-nostdinc", "-c", "-G", "0", "-mgp32", "-mfp32", "-D_LANGUAGE_C", "-mips3",
             "-D_MIPS_SZLONG=32", "-D__USE_ISOC99", gbi, f"-DBUILD_VERSION=VERSION_{VERSION}",
             f'-DBUILD_VERSION_STRING="2.0{VERSION}"', "-D_FINALROM", "-DNDEBUG", OPT,
             "-I.", f"-I{UL}/include", f"-I{UL}/include/compiler/gcc", f"-I{UL}/include/PR", f"-I{UL}/src"]
    if VERSION in "DEFGHIJ":
        flags.append("-funsigned-char")
    env = dict(os.environ, VR4300MUL="ON")
    r = subprocess.run([str(GCC), f"-B{GCC.parent}/", *flags, Path(c).name, "-o", str(o)],
                       cwd=Path(c).parent, capture_output=True, env=env)
    return (c, o) if r.returncode == 0 and o.exists() else (c, None)


def funcs_of(o):
    """{nombre: (bytes de la función, desplazamientos con reubicación)} de la .text de un .o."""
    d = o.parent
    subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", str(o), str(d / "t.bin")], check=True)
    text = (d / "t.bin").read_bytes()
    syms = []
    for l in subprocess.check_output(["mips-linux-gnu-objdump", "-t", str(o)], text=True).splitlines():
        p = l.split()
        if len(p) >= 6 and p[3] == ".text" and "F" in p[2]:
            syms.append((int(p[0], 16), int(p[4], 16), p[5]))
    rel = set()
    for l in subprocess.check_output(["mips-linux-gnu-objdump", "-r", "-j", ".text", str(o)], text=True).splitlines():
        m = re.match(r"^([0-9a-f]{8})\s+R_MIPS", l)
        if m:
            rel.add(int(m.group(1), 16))
    return {n: (text[a:a + s], {r - a for r in rel if a <= r < a + s}) for a, s, n in syms}


def mask(word, reloc):
    if not reloc:
        return word
    op = word >> 26
    return word & 0xFC000000 if op in (2, 3) else word & 0xFFFF0000


def same(code, rels, addr, size):
    n = len(code)
    if n == 0 or n > size or ROM[R(addr) + n:R(addr) + size].strip(b"\0"):
        return False
    rom = ROM[R(addr):R(addr) + n]
    for i in range(0, n, 4):
        a, b = int.from_bytes(code[i:i + 4], "big"), int.from_bytes(rom[i:i + 4], "big")
        if mask(a, i in rels) != mask(b, i in rels):
            return False
    return True


def main():
    todo = pending()
    print(f"# {len(todo)} pendientes en el rango; ultralib 2.0{VERSION} {OPT}", file=sys.stderr)
    sources = sorted(glob.glob(str(UL / "src/**/*.c"), recursive=True))
    with ThreadPoolExecutor(4) as ex:
        objs = list(ex.map(compile_one, sources))
    print(f"# compilados {sum(o is not None for _, o in objs)}/{len(sources)}", file=sys.stderr)
    found = {}
    for c, o in objs:
        if o is None:
            continue
        for fn, (code, rels) in funcs_of(o).items():
            for a, (n, size) in todo.items():
                if a not in found and same(code, rels, a, size):
                    found[a] = (n, fn, os.path.relpath(c, UL))
    for a in sorted(found):
        n, fn, c = found[a]
        print(f"{a:08X} {n} {fn} {c}")
    print(f"# identificadas {len(found)}/{len(todo)}", file=sys.stderr)


if __name__ == "__main__":
    main()
