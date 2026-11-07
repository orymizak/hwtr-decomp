#!/usr/bin/env python3
"""gen_context.py: genera ghidra_work/context.h con prototipos a partir de ghidra_work/decomp.jsonl."""
import json, re
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
open("ghidra_work/context.h", "w").write("\n".join(out) + "\n")
print(len(seen), "prototipos")
