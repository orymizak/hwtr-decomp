#!/usr/bin/env python3
"""pass2.py [--no-commit] [unidades...]: segunda pasada sobre las funciones en INCLUDE_ASM de cada unidad.

Prueba variantes de m2c y de anchos de acceso, compara cada una con la ROM (exacto salvo
reubicaciones), une lo que coincide resolviendo prototipos en conflicto y hace commit solo si la
ROM queda idéntica. Se ejecuta desde la raíz del repo, con TMP_DIR apuntando a un directorio temporal
y `m2c` en el PATH. Escribe en $TMP_DIR/near.json las que casi coinciden."""
import glob, itertools, json, os, re, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

TC = "tools/gcc_2.7.2/"
TMP = Path(os.environ["TMP_DIR"]).resolve()
ROM = Path("baserom.z64").read_bytes()
R = lambda v: v - 0x80060400 + 0x1000
CFLAGS = ["-G0", "-mips3", "-mgp32", "-mfp32", "-Iinclude"]
M2C_VARIANTS = [[], ["--no-andor"], ["--reg-vars", "saved"], ["--no-andor", "--reg-vars", "saved"]]
MAX_WIDTH_COMBOS = 8
WIDTH = {"lw": "s32", "sw": "s32", "lb": "s8", "lbu": "u8", "sb": "u8", "lh": "s16", "lhu": "u16", "sh": "s16",
         "lwc1": "f32", "swc1": "f32", "ldc1": "f64", "sdc1": "f64"}
near = []


def opt_of(unit):
    m = re.search(r"\$\(BUILD\)/src/%s\.o: OPTFLAGS := (\S+)" % re.escape(unit), Path("Makefile").read_text())
    return m.group(1) if m else "-O2"


def asm_of(unit, name):
    t = Path(f"asm/nonmatchings/{unit}/{name}.s").read_text()
    m = re.search(r"nonmatching \S+?, (0x[0-9A-F]+)\n\n(glabel \S+\n.*?endlabel \S+\n)", t, re.S)
    v = int(re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8})", m.group(2)).group(1), 16)
    return v, int(m.group(1), 16), m.group(2)


def width_options(asm):
    w = {}
    for op, off in re.findall(r"\*/\s+(lw|sw|lbu|lb|sb|lhu|lh|sh|lwc1|swc1|ldc1|sdc1)\s+\S+\s+(-?0x[0-9A-F]+)\(\$", asm):
        w.setdefault(int(off, 16), set()).add(WIDTH[op])
    return {k: sorted(v) for k, v in w.items()}


def base_start(s, end):
    """Devuelve dónde empieza la expresión primaria que termina en s[end]."""
    i = end
    while True:
        while i > 0 and s[i - 1] == " ":
            i -= 1
        if i and s[i - 1] in ")]":
            op, cl = ("(", ")") if s[i - 1] == ")" else ("[", "]")
            depth, i = 0, i - 1
            while i >= 0:
                depth += (s[i] == cl) - (s[i] == op)
                if depth == 0:
                    break
                i -= 1
            if s[i] == "[" or (i and (s[i - 1].isalnum() or s[i - 1] == "_")):
                continue
            return i
        j = i
        while j and (s[j - 1].isalnum() or s[j - 1] == "_"):
            j -= 1
        if j >= 2 and s[j - 2:j] == "->":
            i = j - 2
            continue
        return j


def expand_fields(t, choose):
    """Convierte a->unk8 en (*(T *)((u8 *)(a) + 0x8)), de dentro hacia fuera."""
    while True:
        m = re.search(r"->unk([0-9A-Fa-f]+)", t)
        if not m:
            return t
        b = base_start(t, m.start())
        off = m.group(1)
        t = f"{t[:b]}(*({choose(int(off, 16))} *)((u8 *)({t[b:m.start()]}) + 0x{off})){t[m.end():]}"


def symbol_widths(asm):
    w = {}
    for op, sym in re.findall(r"\*/\s+(lw|sw|lbu|lb|sb|lhu|lh|sh|lwc1|swc1|ldc1|sdc1)\s+\S+\s+%lo\((\w+)", asm):
        w.setdefault(sym, WIDTH[op])
    return w


def byte_symbols(t, syms):
    """m2c escala en bytes lo que tipa `?`: esos símbolos pasan a u8 con cast en cada acceso."""
    for sym in re.findall(r"^extern \? (\w+);$", t, re.M):
        t = t.replace(f"extern ? {sym};", f"extern u8 {sym};")
        t = t.replace(f"*(&{sym}", f"*({syms.get(sym, 's32')} *)(&{sym}")
    return t


def clean(text, choice, syms, ptr):
    t = re.sub(r"\s*/\*.*?\*/", "", text)
    if re.search(r"M2C_|saved_reg|\.unk[0-9A-Fa-f]", t):
        return None
    if ptr == "void":
        t = byte_symbols(t, syms)
        t = re.sub(r"\? \*", "void *", t)
    t = re.sub(r"\(\?\)", "()", t)
    t = re.sub(r"\?\s*\(\*", "s32 (*", t)
    t = re.sub(r"(^|[\s(,])\?( \*| )", lambda m: m.group(1) + "s32" + m.group(2), t, flags=re.M)
    t = re.sub(r"\? \*", "s32 *", t)
    t = re.sub(r"(?<=[(,])\s*\?(?=[,)])", "s32", t)
    if re.search(r"(^|[\s(,])\?\s*[;,)]", t, re.M):
        return None
    try:
        t = expand_fields(t, lambda o: choice.get(o, "s32"))
    except IndexError:
        return None
    return "\n".join(l.rstrip() for l in t.splitlines())


def compile_to(src, d, opt):
    f, o = d / "a.c", d / "a.o"
    o.unlink(missing_ok=True)
    f.write_text('#include "common.h"\n' + src)
    r = subprocess.run([TC + "gcc", "-B" + TC, "-c", opt, *CFLAGS, "-o", str(o), str(f)], capture_output=True, text=True)
    return o if r.returncode == 0 and o.exists() else None


def section(o, name, d):
    b = d / "sec.bin"
    b.unlink(missing_ok=True)
    subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", name, str(o), str(b)], capture_output=True)
    return b.read_bytes() if b.exists() else b""


def compare(o, v, size, d):
    """Devuelve (palabras distintas, tiene .rodata). Solo se toleran diferencias en reubicaciones."""
    x = section(o, ".text", d)
    if section(o, ".rodata", d):
        return size, True
    if len(x) < size or any(x[size:size + 16]):
        return size, False
    out = subprocess.run(["mips-linux-gnu-objdump", "-r", "-j", ".text", str(o)], capture_output=True, text=True).stdout
    rel = {int(m.group(1), 16) for m in re.finditer(r"^([0-9a-f]{8}) R_MIPS", out, re.M)}
    dif = 0
    for k in range(0, size, 4):
        a, b = struct.unpack_from(">I", ROM, R(v) + k)[0], struct.unpack_from(">I", x, k)[0]
        if a == b:
            continue
        op = a >> 26
        mask = 0xFC000000 if op in (2, 3) else 0xFFFF0000
        if k not in rel or (a & mask) != (b & mask):
            dif += 1
    return dif, False


def convert(job):
    i, unit, name, opt = job
    d = TMP / "p2" / str(i)
    d.mkdir(parents=True, exist_ok=True)
    v, size, asm = asm_of(unit, name)
    (d / "f.s").write_text(asm)
    opts, syms = width_options(asm), symbol_widths(asm)
    multi = [k for k, w in opts.items() if len(w) > 1]
    combos = [dict(zip(multi, c)) for c in itertools.islice(itertools.product(*(opts[k] for k in multi)), MAX_WIDTH_COMBOS)]
    best, seen = None, set()
    for flags in M2C_VARIANTS:
        r = subprocess.run(["m2c", "--target", "mips-gcc-c", *flags, str(d / "f.s")], capture_output=True, text=True)
        if r.returncode or r.stdout in seen:
            continue
        seen.add(r.stdout)
        for combo in combos:
            choice = {k: w[0] for k, w in opts.items()}
            choice.update(combo)
            for ptr in ("void", "s32"):
                c = clean(r.stdout, choice, syms, ptr)
                if c is None:
                    continue
                o = compile_to(c, d, opt)
                if o is None:
                    continue
                dif, rodata = compare(o, v, size, d)
                if not rodata and dif == 0:
                    return name, c, 0, size
                if not rodata and (best is None or dif < best):
                    best = dif
    return name, None, best, size


def split(c):
    """Separa declaraciones de cuerpo en la salida de m2c."""
    decls, body, inbody = [], [], False
    for l in c.splitlines():
        if not inbody and re.match(r"^[\w \*]+\b\w+\(.*\) \{$", l):
            inbody = True
        if inbody:
            body.append(l)
        elif l.strip():
            decls.append(l)
    return decls, "\n".join(body)


def decl_name(d):
    m = re.search(r"\(\*\s*(\w+)\)|\b(\w+)\s*\(.*\)\s*;$|\b(\w+)\s*(?:\[[^\]]*\])*\s*;$", d)
    return (m.group(1) or m.group(2) or m.group(3)) if m else None


def reconcile(decls, body, known):
    """Quita declaraciones ya presentes; si chocan, reescribe los usos con un cast."""
    new = []
    for d in decls:
        n = decl_name(d)
        if n is None or known.get(n) == d:
            continue
        if n not in known:
            new.append((n, d))
            continue
        fn = re.match(r"^(.*?)\b%s\s*\((.*)\)\s*;$" % re.escape(n), d)
        if fn:
            body = re.sub(r"\b%s\(" % re.escape(n), f"(({fn.group(1).strip()} (*)({fn.group(2)})){n})(", body)
            continue
        arr = re.match(r"^(?:extern\s+)?(.*?)\s*\b%s\s*\[[^\]]*\]\s*;$" % re.escape(n), d)
        var = re.match(r"^(?:extern\s+)?(.*?)\s*\b%s\s*;$" % re.escape(n), d)
        if arr:
            body = re.sub(r"\b%s\b" % re.escape(n), f"(({arr.group(1)} *)&{n})", body)
        elif var:
            body = re.sub(r"\b%s\b" % re.escape(n), f"(*({var.group(1)} *)&{n})", body)
        else:
            return None, None
    return new, body


def compile_tu(src, opt):
    d = TMP / "tu"
    d.mkdir(exist_ok=True)
    f = d / "tu.c"
    f.write_text(src.replace('#include "include_asm.h"\n', "#define INCLUDE_ASM(a, b)\n"))
    r = subprocess.run([TC + "gcc", "-B" + TC, "-c", opt, *CFLAGS, "-o", str(d / "tu.o"), str(f)], capture_output=True, text=True)
    return r.returncode == 0


def known_decls(c):
    k = {}
    for l in c.splitlines():
        if l.startswith(("#", "INCLUDE_ASM", " ", "}")) or not l.strip():
            continue
        if l.endswith(") {"):
            m = re.match(r"^(.*?)\b(\w+)\((.*)\) \{$", l)
            if m:
                k[m.group(2)] = f"{m.group(1)}{m.group(2)}({re.sub(r' (arg|sp|var|temp)[^,]*', '', m.group(3))});"
        else:
            n = decl_name(l)
            if n:
                k[n] = l
    return k


def merge(c, name, cand, opt):
    decls, body = split(cand)
    known = known_decls(c)
    new, body = reconcile(decls, body, known)
    if new is None:
        return None
    head = re.match(r"^(.*?)\b%s\((.*)\) \{" % re.escape(name), body)
    if not head:
        return None
    src = c
    if name in known:
        src = re.sub(r"^.*\b%s\(.*\);\n" % re.escape(name), "", src, count=1, flags=re.M)
        new.append((name, f"{head.group(1)}{name}({head.group(2)});"))
    marker = f'INCLUDE_ASM("asm/nonmatchings/{unit_name(c, name)}", {name});'
    if marker not in src:
        return None
    src = src.replace(marker, body)
    if new:
        idx = src.index('#include "include_asm.h"\n') + len('#include "include_asm.h"\n')
        src = src[:idx] + "\n".join(d for _, d in new) + "\n" + src[idx:]
    return src if compile_tu(src, opt) else None


def unit_name(c, name):
    return re.search(r'INCLUDE_ASM\("asm/nonmatchings/(\w+)", %s\)' % re.escape(name), c).group(1)


def nm_map():
    out = subprocess.check_output(["mips-linux-gnu-nm", "-n", "build/hotwheelsturbo.elf"], text=True)
    return {l.split()[2]: int(l.split()[0], 16) for l in out.splitlines() if len(l.split()) == 3}


def make(unit):
    # El make de macOS compara mtimes con resolución de 1 s: sin esto el .o o el ELF pueden quedar obsoletos.
    for f in (f"build/src/{unit}.o", "build/hotwheelsturbo.elf", "build/hotwheelsturbo.z64"):
        Path(f).unlink(missing_ok=True)
    subprocess.run("make -j8 >/dev/null 2>&1", shell=True)
    p = Path("build/hotwheelsturbo.z64")
    return p.read_bytes() if p.exists() else b""


def degrade(c, unit, names):
    for n in names:
        m = re.search(r"^[\w \*]*?\b%s\([^;{\n]*\) \{\n.*?^\}\n" % re.escape(n), c, re.M | re.S)
        if m:
            c = c[:m.start()] + f'INCLUDE_ASM("asm/nonmatchings/{unit}", {n});\n' + c[m.end():]
    return c


def process(unit, commit):
    path = Path(f"src/{unit}.c")
    orig = path.read_text()
    names = re.findall(r'INCLUDE_ASM\("asm/nonmatchings/\w+", (\w+)\)', orig)
    if not names:
        return 0
    opt = opt_of(unit)
    with ThreadPoolExecutor(8) as ex:
        res = list(ex.map(convert, [(i, unit, n, opt) for i, n in enumerate(names)]))
    c, added = orig, []
    for name, cand, best, size in res:
        if cand is None:
            if best is not None and best * 4 <= size * 0.15:
                near.append(dict(unit=unit, name=name, dif=best, size=size))
            continue
        m = merge(c, name, cand, opt)
        if m:
            c = m
            added.append(name)
    if not added:
        return 0
    for _ in range(10):
        path.write_text(c)
        b = make(unit)
        if b == ROM:
            break
        if not b or not added:
            added = []
            break
        nm = nm_map()
        allsym = sorted(set(nm.values()))
        drop = []
        for n in added:
            v = nm.get(n)
            if v is None:
                drop.append(n)
                continue
            nxt = next((x for x in allsym if x > v), v + 4)
            if ROM[R(v):R(nxt)] != b[R(v):R(nxt)]:
                drop.append(n)
        drop = drop or added[-1:]
        c = degrade(c, unit, drop)
        added = [n for n in added if n not in drop]
    else:
        added = []
    if not added or make(unit) != ROM:
        path.write_text(orig)
        if make(unit) != ROM:
            sys.exit(f"{unit}: la ROM no coincide tras restaurar")
        return 0
    if commit:
        subprocess.run(["python3", "tools/progress.py", "--readme"], capture_output=True)
        subprocess.run(["git", "add", "src", "README.md"])
        left = c.count("INCLUDE_ASM(")
        subprocess.run(["git", "commit", "-q", "-m", f"Convierte {len(added)} funciones de {unit}.c\n\nQuedan {left} en ensamblador. La ROM sigue siendo idéntica."])
    return len(added)


def main():
    args = sys.argv[1:]
    commit = "--no-commit" not in args
    units = [a for a in args if not a.startswith("--")] or sorted(Path(f).stem for f in glob.glob("src/*.c"))
    total = 0
    for u in units:
        n = process(u, commit)
        total += n
        json.dump(near, open(TMP / "near.json", "w"))
        print(f"{u}: +{n} (total {total})", flush=True)


main()
