#!/usr/bin/env python3
"""auto.py <nombre> <rom_ini> <rom_fin> [-O2]: convierte un archivo: m2c + compilar + comparar.
Lo que coincide queda en C; lo demás, INCLUDE_ASM. Actualiza el YAML (segmento c)."""
import glob, os, re, struct, subprocess, sys, tempfile
from pathlib import Path

NAME, ROM_S, ROM_E = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
OPT = sys.argv[4] if len(sys.argv) > 4 else "-O2"
TC = "tools/gcc_2.7.2/"
VRAM = lambda r: r - 0x1000 + 0x80060400
rom = Path("baserom.z64").read_bytes()
R = lambda v: v - 0x80060400 + 0x1000
TMP = Path(os.environ["TMP_DIR"])
BR = {4, 5, 6, 7, 20, 21, 22, 23}
IMM = {0xF, 9, 0x23, 0x2B, 0x20, 0x24, 0x25, 0x21, 0x28, 0x29, 0x31, 0x39, 0x35, 0x3D}
FLAGS = [OPT, "-G0", "-mips3", "-mgp32", "-mfp32", "-Iinclude"]

funcs = []
for f in glob.glob("asm/*.s") + glob.glob("asm/nonmatchings/**/*.s", recursive=True):
    t = open(f).read()
    for n, s, b in re.findall(r"nonmatching (\S+?), (0x[0-9A-F]+)\n\n(glabel \S+\n.*?endlabel \S+\n)", t, re.S):
        v = int(re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8})", b).group(1), 16)
        if VRAM(ROM_S) <= v < VRAM(ROM_E):
            funcs.append((v, n, int(s, 16), b))
funcs.sort()
assert funcs, "sin funciones"


def m2c(b):
    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False, dir=TMP) as f:
        f.write(b)
    r = subprocess.run(["m2c", "--target", "mips-gcc-c", f.name], capture_output=True, text=True)
    os.unlink(f.name)
    return r.stdout if r.returncode == 0 else None


W = {"lw": "s32", "sw": "s32", "lb": "s8", "lbu": "u8", "sb": "u8", "lh": "s16", "lhu": "u16", "sh": "s16", "lwc1": "f32", "swc1": "f32", "ldc1": "f64", "sdc1": "f64"}


def widths(b):
    m = {}
    for op, off in re.findall(r"\*/\s+(lw|sw|lbu|lb|sb|lhu|lh|sh|lwc1|swc1|ldc1|sdc1)\s+\S+\s+(-?0x[0-9A-F]+)\(\$", b):
        o = int(off, 16)
        m.setdefault(o, set()).add(W[op])
    return m


def clean(text, derefs, width=None):
    out = []
    for l in text.splitlines():
        l = re.sub(r"\s*/\* extern \*/", "", l)
        l = re.sub(r"\s*/\* static \*/", "", l)
        out.append(l.rstrip())
    t = "\n".join(out)
    if re.search(r"M2C_|saved_reg|unaligned|/\*|\(bitwise|sp[0-9A-F]+\b.*\? |\bphi_", t):
        return None
    t = re.sub(r"\?\s*\(\*", "s32 (*", t)
    t = re.sub(r"(^|[\s(,])\?( \*| )", lambda m: m.group(1) + "s32" + m.group(2), t, flags=re.M)
    t = re.sub(r"\(\?\)", "(s32)", t)
    t = re.sub(r"\? \*", "s32 *", t)
    if "?" in re.sub(r"[^?]\?[^?]*:", "", t) and re.search(r"\?\s*[;,)]", t):
        return None
    if "->unk" in t or re.search(r"\.unk[0-9A-Fa-f]+", t):
        if not derefs:
            return None
        def T(off):
            if derefs != "auto":
                return derefs
            o = width.get(int(off, 16), {"s32"})
            o = sorted(o)
            return o[0] if len(o) == 1 else ("u8" if "u8" in o else "u16" if "u16" in o or "s16" in o else o[0])
        t = re.sub(r"(\b\w+)->unk([0-9A-F]+)", lambda m: f"*({T(m.group(2))} *)((u8 *){m.group(1)} + 0x{m.group(2)})", t)
        t = re.sub(r"\(([^()]*(?:\([^()]*\)[^()]*)*)\)->unk([0-9A-F]+)", lambda m: f"*({T(m.group(2))} *)((u8 *)({m.group(1)}) + 0x{m.group(2)})", t)
        if "unk" in t:
            return None
    return t


def compile_c(src):
    f = TMP / "a.c"
    o = TMP / "a.o"
    b = TMP / "a.bin"
    rd = TMP / "a.rd"
    if o.exists():
        o.unlink()
    f.write_text('#include "common.h"\n' + src)
    r = subprocess.run([TC + "gcc", "-B" + TC, "-c", *FLAGS, "-o", str(o), str(f)], capture_output=True, text=True)
    if r.returncode or not o.exists():
        return None, r.stderr
    subprocess.check_call(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".text", str(o), str(b)])
    if rd.exists():
        rd.unlink()
    subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", ".rodata", str(o), str(rd)], capture_output=True)
    if rd.exists() and rd.stat().st_size:
        return None, "rodata"
    return b.read_bytes(), ""


def same(text, v, size):
    if len(text) < size:
        return False
    for k in range(0, size, 4):
        wa = struct.unpack_from(">I", rom, R(v) + k)[0]
        wb = struct.unpack_from(">I", text, k)[0]
        op = wa >> 26
        if wa == wb or (op in (2, 3) and op == wb >> 26) or ((op in BR or op in IMM) and wa >> 16 == wb >> 16):
            continue
        return False
    return not any(text[size:size + 16].rstrip(b"\0"))


TYPES = [None, "auto", "s32", "u8", "u16", "f32"]
accepted = {}
for v, n, s, b in funcs:
    text = m2c(b)
    if not text:
        continue
    for ty in TYPES:
        c = clean(text, ty, widths(b))
        if c is None:
            continue
        bin_, err = compile_c(c)
        if bin_ is not None and same(bin_, v, s):
            accepted[n] = c
            break

print(f"{NAME}: {len(accepted)}/{len(funcs)} candidatas")


def split_decl(c):
    decls, body = [], []
    inbody = False
    for l in c.splitlines():
        if not inbody and re.match(r"^[\w ]+\*?.*\) \{$", l):
            inbody = True
        if inbody:
            body.append(l)
        elif l.strip():
            decls.append(l)
    return decls, "\n".join(body)


def build(names):
    decl, seen = [], set()
    parts = []
    fn = [n for _, n, _, _ in funcs]
    defined = set(names)
    for v, n, s, b in funcs:
        if n in names:
            d, body = split_decl(names[n])
            for x in d:
                if x not in seen:
                    seen.add(x)
                    decl.append(x)
            parts.append(body)
        else:
            parts.append(f'INCLUDE_ASM("asm/nonmatchings/{NAME}", {n});')
    return '#include "common.h"\n#include "include_asm.h"\n\n' + "\n".join(decl) + "\n\n" + "\n\n".join(parts) + "\n"


def tu_errors(src):
    f = TMP / "tu.c"
    f.write_text(src.replace('#include "include_asm.h"\n', '#define INCLUDE_ASM(a, b)\n'))
    r = subprocess.run([TC + "gcc", "-B" + TC, "-c", *FLAGS, "-o", str(TMP / "tu.o"), str(f)], capture_output=True, text=True)
    return r.stderr if r.returncode else ""


names = dict(accepted)
for _ in range(60):
    src = build(names)
    err = tu_errors(src)
    if not err:
        break
    bad = set(re.findall(r"`(\w+)'", err)) | set(re.findall(r"conflicting types for `(\w+)", err))
    drop = [n for n, c in names.items() if any(re.search(r"\b%s\b" % re.escape(x), c) for x in bad)]
    if not drop:
        drop = list(names)[:1]
    for n in drop[:3]:
        names.pop(n, None)
print(f"{NAME}: {len(names)} tras resolver conflictos")
Path(f"src/{NAME}.c").write_text(build(names))
