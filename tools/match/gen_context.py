#!/usr/bin/env python3
"""gen_context.py: genera ghidra_work/context.h con prototipos a partir de ghidra_work/decomp.jsonl."""
import collections, glob, json, re
TYPES = {"void": "void", "int": "s32", "uint": "u32", "float": "f32", "double": "f64", "bool": "s32", "char": "s8", "byte": "u8", "uchar": "u8",
         "short": "s16", "ushort": "u16", "undefined1": "u8", "undefined2": "s16", "undefined4": "s32", "undefined": "s32", "undefined8": "s64",
         "longlong": "s64", "ulonglong": "u64", "sbyte": "s8"}
SKIP = {"memset", "memcpy", "main"}


def conv(t):
    return "void *" if t.endswith("*") else TYPES.get(t.strip(), "s32")


out = ["typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;",
       "typedef int s32; typedef unsigned int u32; typedef long long s64; typedef unsigned long long u64;",
       "typedef float f32; typedef double f64;"]
seen = set()
for l in open("ghidra_work/decomp.jsonl"):
    r = json.loads(l)
    n = r["name"].replace(".NON_MATCHING", "")
    if n in seen or n in SKIP or not re.fullmatch(r"\w+", n) or "FUN_" in n:
        continue
    seen.add(n)
    args = ", ".join(conv(p) for p in r["params"]) or "void"
    out.append(f"{conv(r['ret'])} {n}({args});")

# Arrays globales: el paso sale del pseudocódigo de Ghidra y los anchos, del ensamblador.
WIDTH = {"lw": "s32", "sw": "s32", "lb": "s8", "lbu": "u8", "sb": "u8", "lh": "s16", "lhu": "u16", "sh": "s16",
         "lwc1": "f32", "swc1": "f32", "ldc1": "f64", "sdc1": "f64"}
SIZE = {"s32": 4, "u32": 4, "f32": 4, "s16": 2, "u16": 2, "s8": 1, "u8": 1, "f64": 8}
GSIZE = {"int": 4, "uint": 4, "float": 4, "undefined4": 4, "short": 2, "ushort": 2, "undefined2": 2, "char": 1, "byte": 1, "undefined1": 1, "double": 8}
widths = collections.defaultdict(collections.Counter)
for f in glob.glob("asm/nonmatchings/*/*.s"):
    for op, sym, off in re.findall(r"\*/\s+(lw|sw|lbu|lb|sb|lhu|lh|sh|lwc1|swc1|ldc1|sdc1)\s+\S+\s+%lo\((D_[0-9A-F]{8})(?: \+ 0x([0-9A-F]+))?\)", open(f).read()):
        widths[int(sym[2:], 16) + int(off or "0", 16)][WIDTH[op]] += 1


def width_of(addr):
    return widths[addr].most_common(1)[0][0] if widths[addr] else None


NUM = r"(0x[0-9a-f]+|\d+)"
SYM = r"(D_[0-9A-F]{8})(?:_NON_MATCHING)?"
votes = collections.defaultdict(collections.Counter)
for l in open("ghidra_work/decomp.jsonl"):
    c = json.loads(l)["c"]
    for sym, k in re.findall(r"\(&%s\)\[[^\]\n]*\* %s\]" % (SYM, NUM), c):
        w = width_of(int(sym[2:], 16))
        if w:
            votes[int(sym[2:], 16)][int(k, 0) * SIZE[w]] += 1
    for sym, k in re.findall(r"&%s \+ [^;\n]*?\* %s\)" % (SYM, NUM), c):
        votes[int(sym[2:], 16)][int(k, 0)] += 1
    # Recorrido con puntero: p = &D; ...; p = p + k
    for ty, var in re.findall(r"^\s*(\w+) \*(\w+);$", c, re.M):
        init = re.search(r"\b%s = \(?[\w \*]*\)?&?%s;" % (var, SYM), c)
        step = re.search(r"\b%s = %s \+ %s;" % (var, var, NUM), c)
        if init and step and ty in GSIZE:
            votes[int(init.group(1)[2:], 16)][int(step.group(1), 0) * GSIZE[ty]] += 1

stride = {a: v.most_common(1)[0][0] for a, v in votes.items()}
decls, rebase, sizes, done, nstruct, nscalar = {}, {}, {}, set(), 0, 0
for a in sorted(stride):
    if a in done or stride[a] < 2:
        continue
    n = stride[a]
    members = [m for m in sorted(stride) if a <= m < a + n and stride[m] == n]
    done.update(members)
    fields = {}
    for addr in sorted(widths):
        if a <= addr < a + n and width_of(addr):
            fields[addr - a] = width_of(addr)
    fields.setdefault(0, width_of(a) or "s32")
    name = f"D_{a:08X}"
    if len(fields) == 1 and SIZE[fields[0]] == n:
        decls[name] = f"extern {fields[0]} {name}[];"
        nscalar += 1
        continue
    parts, pos, ok = [], 0, True
    for off in sorted(fields):
        ty = fields[off]
        if off < pos or off % SIZE[ty] or off + SIZE[ty] > n:
            ok = False
            break
        if off > pos:
            parts.append(f"u8 p{pos:X}[0x{off - pos:X}];")
        parts.append(f"{ty} f{off:X};")
        pos = off + SIZE[ty]
    if not ok:
        continue
    if n > pos:
        parts.append(f"u8 p{pos:X}[0x{n - pos:X}];")
    decls[name] = f"typedef struct {{ {' '.join(parts)} }} S_{name};\nextern S_{name} {name}[];"
    sizes[f"S_{name}"] = n
    nstruct += 1
    for addr in fields:
        if addr != 0 and a + addr != a:
            rebase[f"D_{a + addr:08X}"] = [name, addr]
out.extend(decls.values())
open("ghidra_work/context.h", "w").write("\n".join(out) + "\n")
json.dump({"decls": decls, "rebase": rebase, "sizes": sizes}, open("ghidra_work/data.json", "w"))
print(len(seen), "prototipos;", nstruct, "arrays de structs;", nscalar, "arrays simples;", len(rebase), "símbolos reubicados")
