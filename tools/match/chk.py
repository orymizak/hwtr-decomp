#!/usr/bin/env python3
"""chk.py <vram_ini> <vram_fin> [--show nombre|all]: compara cada función original con la compilada.
Normaliza inmediatos y destinos; el juez final sigue siendo el SHA-1 de la ROM."""
import json, os, re, struct, subprocess, sys
import rabbitizer
S = os.path.dirname(os.path.abspath(__file__))
MAP = S + "/funcs.json"
if not os.path.exists(MAP):
    F = {}
    for f in os.listdir("asm"):
        if f.endswith(".s"):
            t = open("asm/" + f).read()
            for m in re.finditer(r"nonmatching (\S+?), (0x[0-9A-F]+)\n\nglabel \S+\n(?:\s*\.L\S+:\n)*\s+/\* [0-9A-F]+ ([0-9A-F]{8})", t):
                F[int(m.group(3), 16)] = int(m.group(2), 16)
    json.dump({str(k): v for k, v in F.items()}, open(MAP, "w"))
F = {int(k): v for k, v in json.load(open(MAP)).items()}
lo, hi = int(sys.argv[1], 16), int(sys.argv[2], 16)
show = sys.argv[sys.argv.index("--show") + 1] if "--show" in sys.argv else None
syms = {}
for l in open("symbol_addrs.txt"):
    m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+);", l)
    if m:
        syms[int(m.group(2), 16)] = m.group(1)
a = open("baserom.z64", "rb").read()
b = open("build/hotwheelsturbo.z64", "rb").read()
nm = {l.split()[2]: int(l.split()[0], 16) for l in subprocess.check_output(["mips-linux-gnu-nm", "build/hotwheelsturbo.elf"], text=True).splitlines() if len(l.split()) == 3}
R = lambda v: v - 0x80060400 + 0x1000
BR = {4, 5, 6, 7, 20, 21, 22, 23}
IMM = {0xF, 9, 0x23, 0x2B, 0x20, 0x24, 0x25, 0x21, 0x28, 0x29, 0x31, 0x39, 0x35, 0x3D}
def dis(w):
    return " ".join(rabbitizer.Instruction(w).disassemble().split())
tot = ok = 0
for v in sorted(F):
    if not lo <= v < hi:
        continue
    name = syms.get(v, f"func_{v:08X}")
    n = F[v] // 4
    if name not in nm:
        print(f"{v:08X} {name}: sin símbolo")
        tot += n
        continue
    vb = nm[name]
    same = 0
    out = []
    for k in range(n):
        wa = struct.unpack_from(">I", a, R(v) + k * 4)[0]
        wb = struct.unpack_from(">I", b, R(vb) + k * 4)[0]
        op = wa >> 26
        e = wa == wb or (op in (2, 3) and op == wb >> 26) or ((op in BR or op in IMM) and wa >> 16 == wb >> 16)
        same += e
        out.append(f"{'  ' if e else '!!'} {k*4:3x} {dis(wa):36.36} | {dis(wb)}")
    tot += n
    ok += same
    if same != n or show == name or show == "all":
        print(f"{v:08X} {name}: {same}/{n} (despl {vb-v:+#x})")
        if show == name or show == "all":
            print("\n".join(out))
print(f"total {ok}/{tot}; bytes distintos en ROM: {sum(x != y for x, y in zip(a, b)) + abs(len(a) - len(b))}")
