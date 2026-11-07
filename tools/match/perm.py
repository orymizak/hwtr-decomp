#!/usr/bin/env python3
"""perm.py [--iters N] [unidades...]: permutador sencillo para las funciones que pass2 no iguala.

Parte del mejor candidato de pass2 y aplica mutaciones al azar (orden y tipo de variables, orden de
sentencias, operandos conmutativos, temporales, casts), aceptando las que no empeoran. Lo que coincide
pasa por el mismo filtro de pass2: commit por unidad solo si la ROM es idéntica. Mismo entorno que pass2."""
import difflib, glob, json, random, re, struct, sys, time
from pathlib import Path
import pass2
from pass2 import ROM, R, TMP, asm_of, compare, compile_to, convert, section

INTS = ["s32", "u32", "s16", "u16", "s8", "u8"]
MAX_SIZE = 1600
MAX_RATIO = 0.85
ITERS = 300


def target_words(v, size, asm):
    words = list(struct.unpack_from(">%dI" % (size // 4), ROM, R(v)))
    lines = [l for l in asm.splitlines() if re.match(r"\s+/\*", l)]
    for k, l in enumerate(lines[:len(words)]):
        if "%hi(" in l or "%lo(" in l:
            words[k] &= 0xFFFF0000
        elif re.search(r"\*/\s+(jal|j)\s", l):
            words[k] &= 0xFC000000
    return words


def score(o, target, d):
    x = section(o, ".text", d)
    n = len(x) // 4
    w = list(struct.unpack(">%dI" % n, x))
    while w and w[-1] == 0:
        w.pop()
    out = pass2.subprocess.run(["mips-linux-gnu-objdump", "-r", "-j", ".text", str(o)], capture_output=True, text=True).stdout
    for m in re.finditer(r"^([0-9a-f]{8}) R_MIPS_(\w+)", out, re.M):
        k = int(m.group(1), 16) // 4
        if k < len(w):
            w[k] &= 0xFC000000 if m.group(2) == "26" else 0xFFFF0000
    t = list(target)
    while t and t[-1] == 0:
        t.pop()
    if w == t:
        return 0
    same = sum(b.size for b in difflib.SequenceMatcher(None, t, w, autojunk=False).get_matching_blocks())
    return max(len(t), len(w)) - same


def body_span(c):
    h = re.search(r"^[\w \*]+\b\w+\(.*\) \{$", c, re.M)
    return h.end() + 1 if h else None


def locals_of(c):
    b = body_span(c)
    return [m for m in re.finditer(r"^    ([\w ]+?) ?(\*?)(\w+);$", c[b:], re.M) if m.group(1).split()[-1] in INTS + ["f32", "f64", "void"]] if b else []


def m_swap_decls(c, r):
    b = body_span(c)
    lines = c[b:].split("\n")
    idx = [i for i, l in enumerate(lines) if re.match(r"^    [\w ]+ \*?\w+;$", l)]
    if len(idx) < 2:
        return None
    i, j = r.sample(idx, 2)
    lines[i], lines[j] = lines[j], lines[i]
    return c[:b] + "\n".join(lines)


def m_retype(c, r):
    ls = [m for m in locals_of(c) if not m.group(2) and m.group(1).split()[-1] in INTS]
    if not ls:
        return None
    m = r.choice(ls)
    b = body_span(c)
    return c[:b + m.start(1)] + r.choice(INTS) + c[b + m.end(1):]


def m_retype_param(c, r):
    h = re.search(r"^[\w \*]+\b\w+\((.*)\) \{$", c, re.M)
    ps = list(re.finditer(r"\b(s32|u32|s16|u16|s8|u8) (arg\d)\b", h.group(1))) if h else []
    if not ps:
        return None
    m = r.choice(ps)
    a = h.start(1)
    return c[:a + m.start(1)] + r.choice(["s32", "u32", "s16", "u16", "s8", "u8"]) + c[a + m.end(1):]


def m_swap_stmts(c, r):
    b = body_span(c)
    lines = c[b:].split("\n")
    idx = [i for i in range(len(lines) - 1)
           if re.match(r"^\s+[^\s{}].*;$", lines[i]) and re.match(r"^\s+[^\s{}].*;$", lines[i + 1])
           and len(lines[i]) - len(lines[i].lstrip()) == len(lines[i + 1]) - len(lines[i + 1].lstrip())
           and "=" in lines[i] + lines[i + 1] and "return" not in lines[i] + lines[i + 1]
           and "goto" not in lines[i] + lines[i + 1] and not re.match(r"^    [\w ]+ \*?\w+;$", lines[i])]
    if not idx:
        return None
    i = r.choice(idx)
    lines[i], lines[i + 1] = lines[i + 1], lines[i]
    return c[:b] + "\n".join(lines)


def operand_left(t, i):
    """Inicio de la expresión primaria que termina justo antes de i."""
    j = i
    if t[j - 1] == ")":
        depth = 0
        while j > 0:
            j -= 1
            depth += (t[j] == ")") - (t[j] == "(")
            if depth == 0:
                break
    else:
        while j > 0 and (t[j - 1].isalnum() or t[j - 1] in "_."):
            j -= 1
    while j > 0 and t[j - 1] in "&*":
        j -= 1
    return j


def operand_right(t, i):
    j = i
    while j < len(t) and t[j] in "&*":
        j += 1
    if t[j] == "(":
        depth = 0
        while j < len(t):
            depth += (t[j] == "(") - (t[j] == ")")
            j += 1
            if depth == 0:
                break
        if t[j:j + 1] == "(" or t[j:j + 1].isalnum() or t[j:j + 1] in ("_", "&", "*"):
            return operand_right(t, j)  # era un cast
    else:
        while j < len(t) and (t[j].isalnum() or t[j] in "_."):
            j += 1
    return j


def m_commute(c, r):
    b = body_span(c)
    body = c[b:]
    ms = list(re.finditer(r" (\+|\*|&|\||\^|==|!=) ", body))
    if not ms:
        return None
    m = r.choice(ms)
    i, j = operand_left(body, m.start()), operand_right(body, m.end())
    left, right = body[i:m.start()], body[m.end():j]
    if not left or not right:
        return None
    return c[:b] + body[:i] + right + m.group(0) + left + body[j:]


def m_field_width(c, r):
    """El ancho de un campo se dedujo por desplazamiento y puede corresponder a otra base."""
    b = body_span(c)
    ms = list(re.finditer(r"\*\((s32|u32|s16|u16|s8|u8|f32) \*\)\(", c[b:]))
    if not ms:
        return None
    m = r.choice(ms)
    return c[:b + m.start(1)] + r.choice(["s32", "u32", "s16", "u16", "s8", "u8"]) + c[b + m.end(1):]


def m_retype_proto(c, r):
    """Los tipos de los argumentos de una llamada deciden el orden en que se cargan."""
    b = body_span(c)
    ps = [m for m in re.finditer(r"^[\w \*]+\b\w+\((.*)\);$", c[:b], re.M) if m.group(1).strip() not in ("", "void")]
    if not ps:
        return None
    m = r.choice(ps)
    args = [a.strip() for a in m.group(1).split(",")]
    args[r.randrange(len(args))] = r.choice(["s32", "u32", "s16", "u16", "s8", "u8", "void *"])
    return c[:m.start(1)] + ", ".join(args) + c[m.end(1):]


def m_inline_temp(c, r):
    b = body_span(c)
    body = c[b:]
    cand = []
    for m in re.finditer(r"^\s+(temp_\w+) = (.*);\n", body, re.M):
        n = m.group(1)
        if len(re.findall(r"\b%s\b" % n, body)) == 3:  # declaración, asignación y un uso
            cand.append(m)
    if not cand:
        return None
    m = r.choice(cand)
    n, e = m.group(1), m.group(2)
    body = body[:m.start()] + body[m.end():]
    body = re.sub(r"^\s+[\w ]+ \*?%s;\n" % n, "", body, flags=re.M)
    body = re.sub(r"\b%s\b" % n, lambda _: f"({e})", body, count=1)
    return c[:b] + body


def m_cast(c, r):
    b = body_span(c)
    ms = list(re.finditer(r"\((s32|u32|s16|u16|s8|u8)\) ?", c[b:]))
    if not ms:
        return None
    m = r.choice(ms)
    return c[:b + m.start()] + c[b + m.end():]


def m_unsigned(c, r):
    b = body_span(c)
    ms = list(re.finditer(r"(0x[0-9A-Fa-f]+|\d+)U\b", c[b:]))
    if not ms:
        return None
    m = r.choice(ms)
    return c[:b + m.start()] + m.group(1) + c[b + m.end():]


def m_register(c, r):
    ls = [m for m in locals_of(c) if not m.group(1).startswith("register")]
    if not ls:
        return None
    m = r.choice(ls)
    b = body_span(c)
    return c[:b + m.start(1)] + "register " + c[b + m.start(1):]


def m_array_index(c, r):
    """Vuelve a la forma original de un bucle sobre un array: quita el índice paralelo en bytes."""
    b = body_span(c)
    body = c[b:]
    counters = [m.group(1) for m in re.finditer(r"^\s+(var_\w+) \+= 1;$", body, re.M) if re.search(r"^\s+%s = 0;$" % m.group(1), body, re.M)]
    steps = re.findall(r"^\s+(var_\w+) \+= (0x[0-9A-Fa-f]+|\d+);$", body, re.M)
    cand = []
    for y, n in steps:
        if int(n, 0) == 1:
            continue
        init = re.findall(r"^\s+%s = (.*);$" % y, body, re.M)
        if len(init) == 1 and len(re.findall(r"^\s+%s [-+*&|]?= " % y, body, re.M)) == 2:
            cand.extend((x, y, n, init[0]) for x in counters if x != y)
    if not cand:
        return None
    x, y, n, init = r.choice(cand)
    body = re.sub(r"^\s+%s = .*;\n" % y, "", body, count=1, flags=re.M)
    body = re.sub(r"^\s+%s \+= .*;\n" % y, "", body, count=1, flags=re.M)
    body = re.sub(r"^\s+[\w ]+ \*?%s;\n" % y, "", body, count=1, flags=re.M)
    idx = f"({x} * {n})" if init == "0" else f"((u8 *)({init}) + ({x} * {n}))"
    return c[:b] + re.sub(r"\b%s\b" % y, lambda _: idx, body)


def m_no_unsigned_counter(c, r):
    """`var += 1; while (var < 8U)` suele ser un s32 en el original."""
    b = body_span(c)
    body = c[b:]
    ms = list(re.finditer(r"^\s+u32 (var_\w+);$", body, re.M))
    if not ms:
        return None
    m = r.choice(ms)
    body = body[:m.start()] + m.group(0).replace("u32", "s32") + body[m.end():]
    body = re.sub(r"(\b%s (?:<|<=|>|>=) (?:0x[0-9A-Fa-f]+|\d+))U\b" % m.group(1), r"\1", body)
    return c[:b] + body


def m_merge_syms(c, r):
    """Dos símbolos cercanos suelen ser campos del mismo struct: un solo símbolo base."""
    b = body_span(c)
    body = c[b:]
    syms = sorted(set(re.findall(r"&(D_[0-9A-F]{8})\b", body)))
    if len(syms) < 2:
        return None
    base = syms[0]
    for y in syms[1:]:
        dist = int(y[2:], 16) - int(base[2:], 16)
        if dist < 0x800:
            body = re.sub(r"(?:\(u8 \*\))?&%s\b" % y, f"((u8 *)&{base} + 0x{dist:X})", body)
    return c[:b] + body


def m_struct_array(c, r):
    t = m_array_index(m_merge_syms(c, r) or c, r)
    # El campo va después del índice, como en a[i].campo: gcc conserva así la base y el desplazamiento.
    return t and re.sub(r"\(\(u8 \*\)&(D_\w+) \+ (0x\w+)\) \+ (\(var_\w+ \* \w+\))", r"(u8 *)&\1 + \3 + \2", t)


IDX = r"\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*?) \* (0x[0-9A-Fa-f]+|\d+)\)"
ACCESS = [
    r"\*\((\w+) \*\)\(\(u8 \*\)&(D_\w+) \+ " + IDX + r"(?: \+ (0x[0-9A-Fa-f]+))?\)",
    r"\*\((\w+) \*\)\(\(u8 \*\)\(+\(u8 \*\)\(?&(D_\w+)\)? \+ " + IDX + r"\)+(?: \+ (0x[0-9A-Fa-f]+))?\)",
]
TSIZE = {"s32": 4, "u32": 4, "f32": 4, "s16": 2, "u16": 2, "s8": 1, "u8": 1, "f64": 8}


def m_true_array(c, r):
    """Un array tipado de verdad (a[i].campo) da `lui at; addu at; sw %lo(a)(at)`; un cast no."""
    t = m_struct_array(c, r) or c
    b = body_span(t)
    body = t[b:]
    found = [m for pat in ACCESS for m in re.finditer(pat, body)]
    if not found:
        return None
    bases = {}
    for m in found:
        ty, sym, _, n, off = m.groups()
        if ty in TSIZE:
            bases.setdefault(sym, {"n": int(n, 0), "f": {}})
            if bases[sym]["n"] == int(n, 0):
                bases[sym]["f"][int(off or "0", 0)] = ty
    decl = []
    for sym, info in bases.items():
        n, fields, pos, parts = info["n"], sorted(info["f"].items()), 0, []
        if any(o % TSIZE[ty] for o, ty in fields) or fields[-1][0] + TSIZE[fields[-1][1]] > n:
            return None
        for o, ty in fields:
            if o > pos:
                parts.append(f"u8 p{pos:X}[0x{o - pos:X}];")
            parts.append(f"{ty} f{o:X};")
            pos = o + TSIZE[ty]
        if n > pos:
            parts.append(f"u8 p{pos:X}[0x{n - pos:X}];")
        decl.append(f"typedef struct {{ {' '.join(parts)} }} S_{sym};\nextern S_{sym} {sym}[];")

    def rep(m):
        ty, sym, x, n, off = m.groups()
        if sym not in bases or bases[sym]["n"] != int(n, 0) or ty not in TSIZE:
            return m.group(0)
        return f"{sym}[{x}].f{int(off or '0', 0):X}"
    for pat in ACCESS:
        body = re.sub(pat, rep, body)
    for sym in bases:
        body = re.sub(r"\(u8 \*\)&%s \+ %s" % (sym, IDX), lambda m: f"(u8 *)&{sym}[{m.group(1)}]", body)
    head = t[:b]
    for sym in bases:
        head = re.sub(r"^extern [\w \*]+\b%s(\[\])?;\n" % sym, "", head, flags=re.M)
    head = "\n".join(decl) + "\n" + head
    for sym in re.findall(r"^extern u8 (D_\w+);$", head, re.M):
        if not re.search(r"\b%s\b" % sym, body):
            head = head.replace(f"extern u8 {sym};\n", "")
    return head + body


MUTS = [m_field_width] * 3 + [m_retype_proto] * 2 + [m_true_array] * 6 + [m_struct_array] * 4 + [m_merge_syms] + [m_array_index] * 3 + [m_no_unsigned_counter] * 2 + [m_swap_decls] * 4 + [m_retype] * 2 + [m_retype_param, m_swap_stmts, m_swap_stmts, m_commute, m_inline_temp, m_cast, m_unsigned, m_register]


def permute(job):
    r_ = convert(job)
    name, cand, best, size, base = r_
    if cand is not None or base is None or size > MAX_SIZE or body_span(base) is None:
        return r_
    i, unit, name, opt = job
    d = TMP / "pm" / str(i)
    d.mkdir(parents=True, exist_ok=True)
    v, size, asm = asm_of(unit, name)
    target = target_words(v, size, asm)
    o = compile_to(base, d, opt)
    if o is None or section(o, ".rodata", d):
        return r_
    cur = score(o, target, d)
    if cur > len(target) * MAX_RATIO:
        return r_
    rng = random.Random(name)
    c = base
    for _ in range(ITERS):
        t = c
        for __ in range(rng.choice((1, 1, 2))):
            if body_span(t) is None:
                t = c
                break
            try:
                t = rng.choice(MUTS)(t, rng) or t
            except (IndexError, ValueError):
                pass  # una mutación de texto puede no aplicar a esta función
        if t == c:
            continue
        o = compile_to(t, d, opt)
        if o is None or section(o, ".rodata", d):
            continue
        s = score(o, target, d)
        if s <= cur:
            c, cur = t, s
            if s == 0:
                dif, ro = compare(o, v, size, d)
                if dif == 0 and not ro:
                    return name, t, 0, size, t
    return name, None, cur, size, c


def main():
    global ITERS
    args = sys.argv[1:]
    if "--iters" in args:
        i = args.index("--iters")
        ITERS = int(args[i + 1])
        del args[i:i + 2]
    units = args or sorted(Path(f).stem for f in glob.glob("src/*.c"))
    total = 0
    t0 = time.time()
    for u in units:
        n = pass2.process(u, True, permute)
        total += n
        print(f"{u}: +{n} (total {total}, {int(time.time() - t0)} s)", flush=True)


main()
