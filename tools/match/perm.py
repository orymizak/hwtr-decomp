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
MAX_RATIO = 0.5
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
    return h.end() if h else None


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


def m_commute(c, r):
    b = body_span(c)
    ms = list(re.finditer(r"([\w\.]+|\([^()]*\)) (\+|\*|&|\||\^|==|!=) ([\w\.]+|\([^()]*\))", c[b:]))
    if not ms:
        return None
    m = r.choice(ms)
    return c[:b + m.start()] + f"{m.group(3)} {m.group(2)} {m.group(1)}" + c[b + m.end():]


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


MUTS = [m_swap_decls] * 4 + [m_retype] * 2 + [m_retype_param, m_swap_stmts, m_swap_stmts, m_commute, m_inline_temp, m_cast, m_unsigned, m_register]


def permute(job):
    r_ = convert(job)
    name, cand, best, size, base = r_
    if cand is not None or base is None or size > MAX_SIZE:
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
            nt = rng.choice(MUTS)(t, rng)
            t = nt or t
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
