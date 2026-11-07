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
CONTEXT = Path("ghidra_work/context.h")
PROTOS = {}
if CONTEXT.exists():
    for line in CONTEXT.read_text().splitlines():
        m = re.match(r"^[\w \*]+?\b(\w+)\(.*\);$", line)
        if m:
            PROTOS[m.group(1)] = line
    M2C_VARIANTS = [["--context", str(CONTEXT)] + v for v in M2C_VARIANTS] + M2C_VARIANTS
MAX_WIDTH_COMBOS = 8
WIDTH = {"lw": "s32", "sw": "s32", "lb": "s8", "lbu": "u8", "sb": "u8", "lh": "s16", "lhu": "u16", "sh": "s16",
         "lwc1": "f32", "swc1": "f32", "ldc1": "f64", "sdc1": "f64"}
near = []
KNOWN = {m.group(1): int(m.group(2), 16) for m in re.finditer(r"^(\w+) = 0x([0-9A-Fa-f]+);", Path("symbol_addrs.txt").read_text(), re.M)}


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
    for op, off in re.findall(r"\*/\s+(lw|sw|lbu|lb|sb|lhu|lh|sh|lwc1|swc1|ldc1|sdc1)\s+\S+\s+(-?0x[0-9A-F]+)\(\$(?!sp)", asm):
        w.setdefault(int(off, 16), set()).add(WIDTH[op])
    return {k: sorted(v, key=lambda t: (t != "s32", t)) for k, v in w.items()}


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
        m = re.search(r"->unk(-?)([0-9A-Fa-f]+)", t)
        if not m:
            return t
        b = base_start(t, m.start())
        sign, off = m.group(1), m.group(2)
        val = int(sign + off, 16)
        ty = "s32 (**)()" if t[m.end():m.end() + 1] == "(" else f"{choose(val)} *"
        add = f"- 0x{off}" if sign else f"+ 0x{off}"
        t = f"{t[:b]}(*({ty})((u8 *)({t[b:m.start()]}) {add})){t[m.end():]}"


def symbol_widths(asm):
    w = {}
    for op, sym, off in re.findall(r"\*/\s+(lw|sw|lbu|lb|sb|lhu|lh|sh|lwc1|swc1|ldc1|sdc1)\s+\S+\s+%lo\((\w+)(?: \+ 0x([0-9A-F]+))?\)", asm):
        w.setdefault(sym, WIDTH[op])
        w.setdefault((sym, int(off or "0", 16)), WIDTH[op])
    return w


def global_fields(t, syms):
    """D_X.unk4 sobre un símbolo sin tipo: acceso por bytes con el ancho que usa el ensamblador."""
    def rep(m):
        off = int(m.group(2) + m.group(3), 16)
        ty = "s32 (**)()" if m.group(4) else syms.get((m.group(1), off), "s32") + " *"
        add = f"- 0x{m.group(3)}" if m.group(2) else f"+ 0x{m.group(3)}"
        return f"(*({ty})((u8 *)&{m.group(1)} {add})){m.group(4)}"
    return re.sub(r"\b(D_[0-9A-F]+)\.unk(-?)([0-9A-F]+)\b(\(?)", rep, t)


def byte_symbols(t, syms):
    """m2c escala en bytes lo que tipa `?`: esos símbolos pasan a u8 con cast en cada acceso."""
    for sym in re.findall(r"^extern \? (\w+);$", t, re.M):
        t = t.replace(f"extern ? {sym};", f"extern u8 {sym};")
        while True:
            i = t.find(f"*(&{sym}")
            if i < 0:
                break
            j, depth = i + 1, 0
            while True:
                depth += (t[j] == "(") - (t[j] == ")")
                if depth == 0:
                    break
                j += 1
            if t[j + 1:j + 2] == "(":
                t = f"{t[:i]}(*(s32 (**)()){t[i + 1:j + 1]}){t[j + 1:]}"
            else:
                t = f"{t[:i]}*({syms.get(sym, 's32')} *){t[i + 1:]}"
    return t


def byte_offsets(t, syms):
    """m2c suma bytes a `&D` también si D tiene tipo: se hace explícito con u8 *."""
    for sym in set(re.findall(r"&(D_[0-9A-F]+)\b", t)):
        ty = syms.get(sym, "s32")
        t = re.sub(r"\*\(&%s\b" % sym, f"*({ty} *)((u8 *)&{sym}", t)
        t = re.sub(r"(?<!\(u8 \*\))&%s( [+-] )" % sym, f"(u8 *)&{sym}\\1", t)
        t = re.sub(r"( \+ )&%s\b" % sym, f"\\1(u8 *)&{sym}", t)
    return t


def void_derefs(t, choice):
    """`*x` sobre `void *x` no compila: usa el ancho visto en el desplazamiento 0."""
    for x in set(re.findall(r"\bvoid \*(\w+)[;,)]", t)):
        ty = choice.get(0, "s32")
        t = re.sub(r"(?<![\w)\]])(?<!void )\*\*%s\b" % x, f"(*({ty} *)(*(s32 *){x}))", t)
        t = re.sub(r"(?<![\w)\]])(?<!void )\*%s\b" % x, f"(*({ty} *){x})", t)
    return t


def declare_stack(t):
    """m2c lee huecos de pila sin declararlos."""
    head = re.search(r"^[\w \*]+\b\w+\(.*\) \{\n", t, re.M)
    if not head:
        return t
    body = t[head.end():]
    names = sorted(n for n in set(re.findall(r"\bsp[0-9A-F]+\b", body)) if not re.search(r"^\s+[\w \*]+\b%s\b[^=]*;$" % n, body, re.M))
    return t[:head.end()] + "".join(f"    s32 {n};\n" for n in names) + body


SIZES = {"s32": 4, "u32": 4, "f32": 4, "s16": 2, "u16": 2, "f64": 8}


def scale_steps(t):
    """m2c da en bytes el paso de un puntero tipado: `s32 *p; p += 4` es avanzar uno."""
    for ty, stars, name in set(re.findall(r"^\s+(\w+) (\*+)(\w+);$", t, re.M)):
        size = 4 if len(stars) > 1 else SIZES.get(ty) or DATA.get("sizes", {}).get(ty)
        if size:
            t = re.sub(r"\b%s ([+-])= (0x[0-9A-Fa-f]+|\d+);" % name,
                       lambda m: f"{name} {m.group(1)}= {int(m.group(2), 0) // size};" if int(m.group(2), 0) % size == 0 else m.group(0), t)
    return t


def fill_params(t):
    """m2c omite los argumentos sin usar anteriores a uno usado, y cambiaría el registro."""
    head = re.search(r"^([\w \*]+\b\w+)\((.*)\) \{$", t, re.M)
    if not head:
        return t
    used = [int(n) for n in re.findall(r"\barg(\d+)\b", head.group(2))]
    if not used or sorted(used) == list(range(max(used) + 1)):
        return t
    parts = dict((int(m.group(1)), p.strip()) for p in head.group(2).split(",") for m in [re.search(r"\barg(\d+)\b", p)] if m)
    sig = ", ".join(parts.get(i, f"s32 arg{i}") for i in range(max(used) + 1))
    return t[:head.start()] + f"{head.group(1)}({sig}) {{" + t[head.end():]


def volatile_io(t):
    """Los registros de hardware son volátiles: gcc no une la dirección con el desplazamiento."""
    t = re.sub(r"\*\(void \*\)(0x[0-9A-Fa-f]{8})\b", r"*(s32 *)\1", t)
    return re.sub(r"\*\((\w+) \*\)(0x[0-9A-Fa-f]{8})\b", r"*(volatile \1 *)\2", t)


def unspecified_protos(t):
    head = re.search(r"^[\w \*]+\b\w+\(.*\) \{$", t, re.M)
    end = head.start() if head else len(t)
    head = re.sub(r"^([\w \*]+\b\w+)\(.*\);$", r"\1();", t[:end], flags=re.M)
    head = re.sub(r"^(extern [\w \*]+\(\*\w+\))\(.*\);$", r"\1();", head, flags=re.M)
    return head + t[end:]


def typed_derefs(t):
    """`*(expr)` sobre un entero no compila: se tipa, y el permutador puede cambiar el ancho."""
    return re.sub(r"(?<![\w)\]])\*\((?!(?:volatile )?\w+ \*+\))", "*(s32 *)(", t)


def clean(text, choice, syms, ptr):
    t = re.sub(r"\s*/\*.*?\*/", "", text)
    t = re.sub(r"^.*\bmemset\(.*\);\n", "", t, flags=re.M)
    t = re.sub(r"\(unaligned (\w+)\)", r"(\1)", t)
    t = re.sub(r"\(bitwise \w+\) ?", "", t)
    t = re.sub(r"\?(8|16|32|64)\b", lambda m: {"8": "s8", "16": "s16", "32": "s32", "64": "f64"}[m.group(1)], t)
    t = struct_index(global_fields(t, syms))
    if re.search(r"M2C_|saved_reg|\.unk[0-9A-Fa-f]", t):
        return None
    if ptr == "void":
        t = byte_symbols(t, syms)
        t = byte_offsets(t, syms)
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
    t = declare_stack(void_derefs(t, choice))
    t = volatile_io(fill_params(scale_steps(t)))
    return "\n".join(l.rstrip() for l in t.splitlines())


DATA = json.loads(Path("ghidra_work/data.json").read_text()) if Path("ghidra_work/data.json").exists() else {"decls": {}, "rebase": {}}


def rebase_asm(asm):
    """Un campo de un array global aparece como símbolo propio: se expresa como base + desplazamiento."""
    def rep(m):
        base = DATA["rebase"].get(m.group(2))
        if not base:
            return m.group(0)
        off = base[1] + int(m.group(3) or "0", 16)
        return f"%{m.group(1)}({base[0]} + 0x{off:X})"
    return re.sub(r"%(hi|lo)\((D_[0-9A-F]{8})(?: \+ 0x([0-9A-F]+))?\)", rep, asm)


INDEX = r"\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*?) \* (0x[0-9A-Fa-f]+|\d+)\)"


def struct_index(t):
    """m2c no siempre indexa: *(&D->f1C + (i * 0x4C)) es D[i].f1C, que es lo que da el código original."""
    def rep(m):
        sym, field, x, n = m.groups()
        if DATA.get("sizes", {}).get(f"S_{sym}") != int(n, 0):
            return m.group(0)
        return f"{sym}[{x}].f{field}"
    t = re.sub(r"\*\(&(D_[0-9A-F]{8})->f([0-9A-F]+) \+ " + INDEX + r"\)", rep, t)
    return t


def add_protos(text, name):
    """Con contexto m2c no repite lo ya conocido, pero el candidato necesita declararlo."""
    used = [n for n in set(re.findall(r"\b(\w+)\(", text)) if n in PROTOS and n != name]
    missing = [PROTOS[n] for n in sorted(used) if not re.search(r"^[\w \*]+\b%s\(.*\);$" % n, text, re.M)]
    for sym in sorted(set(re.findall(r"\bD_[0-9A-F]{8}\b", text))):
        if sym in DATA["decls"] and not re.search(r"^extern .*\b%s\b" % sym, text, re.M):
            missing.append(DATA["decls"][sym])
    return "\n".join(missing) + "\n" + text if missing else text


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
    relocs = [(int(m.group(1), 16), m.group(2), m.group(3)) for m in re.finditer(r"^([0-9a-f]{8}) R_MIPS_(\w+)\s+(\S+)", out, re.M)]
    rel = {r[0]: r for r in relocs}
    dif = 0
    for k in range(0, size, 4):
        a, b = struct.unpack_from(">I", ROM, R(v) + k)[0], struct.unpack_from(">I", x, k)[0]
        if a == b and k not in rel:
            continue
        op = a >> 26
        mask = 0xFC000000 if op in (2, 3) else 0xFFFF0000
        if k not in rel or (a & mask) != (b & mask) or not reloc_ok(k, rel[k], a, b, relocs, x, v):
            dif += 1
    return dif, False


def symbol_addr(sym):
    m = re.fullmatch(r"(?:func|D)_([0-9A-F]{8})", sym)
    return int(m.group(1), 16) if m else KNOWN.get(sym)


def reloc_ok(k, r, a, b, relocs, x, v):
    """Comprueba que la reubicación lleve a la misma dirección que la ROM; un símbolo desconocido no se puede comprobar."""
    _, kind, sym = r
    if sym.startswith("."):
        return kind != "26" or (a & 0x3FFFFFF) == ((v + ((b & 0x3FFFFFF) << 2)) >> 2 & 0x3FFFFFF)
    base = symbol_addr(sym)
    if base is None:
        return True
    if kind == "26":
        return (a & 0x3FFFFFF) == (base + ((b & 0x3FFFFFF) << 2)) >> 2 & 0x3FFFFFF
    if kind == "LO16":
        return (a & 0xFFFF) == (base + sext(b)) & 0xFFFF
    if kind == "HI16":
        lo = next((rr for rr in relocs if rr[0] > k and rr[1] == "LO16" and rr[2] == sym), None)
        if lo is None:
            return True
        add = ((b & 0xFFFF) << 16) + sext(struct.unpack_from(">I", x, lo[0])[0])
        return (a & 0xFFFF) == ((base + add + 0x8000) >> 16) & 0xFFFF
    return True


def sext(w):
    w &= 0xFFFF
    return w - 0x10000 if w & 0x8000 else w


def convert(job):
    i, unit, name, opt = job
    d = TMP / "p2" / str(i)
    d.mkdir(parents=True, exist_ok=True)
    v, size, asm = asm_of(unit, name)
    # m2c no acepta saltos a la propia función: les da una etiqueta local.
    selfb = re.sub(r"^(\s+/\*.*?\*/\s+(?!jal\b)\w+\s.*\b)%s$" % re.escape(name), r"\1.Lself", asm, flags=re.M)
    if selfb != asm:
        asm = selfb.replace(f"glabel {name}\n", f"glabel {name}\n.Lself:\n", 1)
    (d / "f.s").write_text(asm)
    opts, syms = width_options(asm), symbol_widths(asm)
    multi = [k for k, w in opts.items() if len(w) > 1]
    combos = [dict(zip(multi, c)) for c in itertools.islice(itertools.product(*(opts[k] for k in multi)), MAX_WIDTH_COMBOS)]
    best, best_c, seen = None, None, set()
    (d / "r.s").write_text(rebase_asm(asm))
    for flags in M2C_VARIANTS:
        src_s = d / ("r.s" if "--context" in flags else "f.s")
        r = subprocess.run(["m2c", "--target", "mips-gcc-c", *flags, str(src_s)], capture_output=True, text=True)
        if r.returncode or r.stdout in seen:
            continue
        seen.add(r.stdout)
        text = add_protos(r.stdout, name) if "--context" in flags else r.stdout
        for combo in combos:
            choice = {k: w[0] for k, w in opts.items()}
            choice.update(combo)
            for ptr in ("void", "s32"):
                c = clean(text, choice, syms, ptr)
                if c is None:
                    continue
                o = compile_to(c, d, opt)
                if o is None:
                    c = unspecified_protos(c)
                    o = compile_to(c, d, opt)
                if o is None:
                    c = typed_derefs(c)
                    o = compile_to(c, d, opt)
                if o is None:
                    continue
                dif, rodata = compare(o, v, size, d)
                if not rodata and dif == 0:
                    return name, c, 0, size, c
                if not rodata and (best is None or dif < best or best_c is None):
                    best, best_c = dif, c
    return name, None, best, size, best_c


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


def unify_structs(c, decls):
    """Un mismo array en varias funciones: une sus campos en un solo struct, si el tamaño coincide."""
    def fields(line):
        size, out = 0, {}
        for ty, kind, off, n in re.findall(r"(\w+) (f|p)([0-9A-F]+)(?:\[0x([0-9A-F]+)\])?;", line):
            w = int(n, 16) if n else {"s8": 1, "u8": 1, "s16": 2, "u16": 2, "f64": 8}.get(ty, 4)
            size = max(size, int(off, 16) + w)
            if kind == "f":
                out[int(off, 16)] = ty
        return size, out

    keep = []
    for d in decls:
        m = re.match(r"typedef struct \{.*\} (S_\w+);$", d)
        old = re.search(r"^typedef struct \{.*\} %s;$" % m.group(1), c, re.M) if m else None
        if not old:
            keep.append(d)
            continue
        (sa, fa), (sb, fb) = fields(old.group(0)), fields(d)
        if sa != sb or any(fa.get(o, t) != t for o, t in fb.items()):
            return None, None
        merged, pos, parts = {**fa, **fb}, 0, []
        for o in sorted(merged):
            w = {"s8": 1, "u8": 1, "s16": 2, "u16": 2, "f64": 8}.get(merged[o], 4)
            if o < pos:
                return None, None
            if o > pos:
                parts.append(f"u8 p{pos:X}[0x{o - pos:X}];")
            parts.append(f"{merged[o]} f{o:X};")
            pos = o + w
        if sa > pos:
            parts.append(f"u8 p{pos:X}[0x{sa - pos:X}];")
        c = c.replace(old.group(0), f"typedef struct {{ {' '.join(parts)} }} {m.group(1)};")
    return c, keep


def merge(c, name, cand, opt):
    decls, body = split(cand)
    c, decls = unify_structs(c, decls)
    if c is None:
        return None
    for d in decls:
        # La declaración de array manda: la escalar anterior se quita y compilar decide si vale.
        m = re.match(r"extern S_\w+ (\w+)\[\];$", d)
        if m:
            c = re.sub(r"^extern (?!S_)[\w \*]+\b%s;\n" % m.group(1), "", c, flags=re.M)
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


COMMIT_EVERY = 5
PENDING = []


def flush():
    """Agrupa los cambios: un commit cada COMMIT_EVERY unidades modificadas."""
    if not PENDING:
        return
    subprocess.run(["python3", "tools/progress.py", "--readme"], capture_output=True)
    subprocess.run(["git", "add", "src", "README.md"])
    n = sum(k for _, k in PENDING)
    detail = "\n".join(f"- {u}: {k}" for u, k in PENDING)
    subprocess.run(["git", "commit", "-q", "-m", f"Convierte {n} funciones de {len(PENDING)} unidades\n\n{detail}\n\nLa ROM sigue siendo idéntica."])
    PENDING.clear()


def process(unit, commit, converter=None):
    path = Path(f"src/{unit}.c")
    orig = path.read_text()
    names = re.findall(r'INCLUDE_ASM\("asm/nonmatchings/\w+", (\w+)\)', orig)
    if not names:
        return 0
    opt = opt_of(unit)
    with ThreadPoolExecutor(8) as ex:
        res = list(ex.map(converter or convert, [(i, unit, n, opt) for i, n in enumerate(names)]))
    cands = {r[0]: r[1] for r in res if r[1] is not None}
    if not cands:
        return 0

    def build(names):
        c = orig
        for n in sorted(names, key=names_order.index):
            c = merge(c, n, cands[n], opt) or c
        return c

    names_order = list(names)

    def rom_ok(c):
        path.write_text(c)
        return make(unit) == ROM

    # Primero lo que el enlazado permita descartar de golpe; luego bisección: uno malo no debe tumbar la unidad.
    todo = [n for n in names_order if n in cands]
    c = build(todo)
    todo = [n for n in todo if f'", {n});' not in c]
    print(f"{unit}: {len(cands)} candidatas, {len(todo)} unidas", flush=True)
    if not todo:
        path.write_text(orig)
        return 0
    good = []
    if rom_ok(c):
        good = todo
    else:
        b = Path("build/hotwheelsturbo.z64").read_bytes() if Path("build/hotwheelsturbo.z64").exists() else b""
        if b and len(b) == len(ROM):
            nm = nm_map()
            allsym = sorted(set(nm.values()))
            def same(n):
                v = nm.get(n)
                if v is None:
                    return False
                nxt = next((x for x in allsym if x > v), v + 4)
                return ROM[R(v):R(nxt)] == b[R(v):R(nxt)]
            todo = [n for n in todo if same(n)]

        def solve(ns):
            if not ns:
                return
            if rom_ok(build(good + ns)):
                good.extend(ns)
            elif len(ns) > 1:
                mid = len(ns) // 2
                solve(ns[:mid])
                solve(ns[mid:])
        solve(todo)
    if not good or not rom_ok(build(good)):
        path.write_text(orig)
        if make(unit) != ROM:
            sys.exit(f"{unit}: la ROM no coincide tras restaurar")
        return 0
    c = build(good)
    added = good
    if commit:
        PENDING.append((unit, len(added)))
        if len(PENDING) >= COMMIT_EVERY:
            flush()
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
    flush()


if __name__ == "__main__":
    main()
