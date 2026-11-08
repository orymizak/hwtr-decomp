#!/usr/bin/env python3
"""apply.py <nombre> <rom_ini> <rom_fin> [-O2]: auto.py + YAML + make; degrada lo que no coincida exacto; commit si la ROM es idéntica."""
import os, re, subprocess, sys
from pathlib import Path
S = os.path.dirname(os.path.abspath(__file__))
NAME, A, B = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
OPT = sys.argv[4] if len(sys.argv) > 4 else "-O2"
env = dict(os.environ, TMP_DIR=S)


def sh(*a, **k):
    return subprocess.run(a, capture_output=True, text=True, **k)


def make():
    subprocess.run("rm -rf asm/nonmatchings build; make split >/dev/null 2>&1; make -j8 >/dev/null 2>&1", shell=True)


y = Path("hotwheelsturbo.yaml")
orig_y = y.read_text()
if f"c, {NAME}]" not in orig_y:
    # busca el segmento asm que contiene A y parte en A y B
    entries = [(int(m.group(1), 16), m.group(0)) for m in re.finditer(r"      - \[0x([0-9A-F]+), (?:asm|c)[^\n]*\n", orig_y)]
    prev = [e for e in entries if e[0] <= A][-1]
    nxt = [e for e in entries if e[0] > A][0]
    assert ", asm" in prev[1], prev
    t = orig_y
    new = ""
    if prev[0] == A:
        t = t.replace(prev[1], "")
    ins = f"      - [0x{A:X}, c, {NAME}]\n"
    if B < nxt[0]:
        ins += f"      - [0x{B:X}, asm]\n"
    t = t.replace(nxt[1], ins + nxt[1]) if prev[0] != A else t.replace(nxt[1], ins + nxt[1])
    y.write_text(t)
    # asm parent se regenera
    if prev[0] != A:
        pass
subprocess.run("rm -f asm/[0-9A-F]*.s asm/game.s asm/audio_driver.s asm/system.s asm/libultra.s asm/libc.s", shell=True)
subprocess.run("make split >/dev/null 2>&1", shell=True)
# primera pasada: todo INCLUDE_ASM para generar los .s
Path(f"src/{NAME}.c").unlink(missing_ok=True)
subprocess.run("rm -rf asm/nonmatchings; make split >/dev/null 2>&1", shell=True)
r = subprocess.run([sys.executable, S + "/auto.py", NAME, hex(A), hex(B), OPT], env=env, capture_output=True, text=True)
print(r.stdout.strip()[-200:])
if r.returncode:
    print(r.stderr[-400:])
    y.write_text(orig_y)
    Path(f"src/{NAME}.c").unlink(missing_ok=True)
    sys.exit(1)
if OPT != "-O2":
    m = Path("Makefile")
    s = m.read_text()
    if f"src/{NAME}.o" not in s:
        s = s.replace("$(BUILD)/src/lz.o: OPTFLAGS := -O0\n", f"$(BUILD)/src/lz.o: OPTFLAGS := -O0\n$(BUILD)/src/{NAME}.o: OPTFLAGS := {OPT}\n")
        m.write_text(s)
romA = Path("baserom.z64").read_bytes()
for attempt in range(12):
    make()
    p = Path("build/hotwheelsturbo.z64")
    if not p.exists():
        print("sin ROM")
        err = subprocess.run("make -j8 2>&1 | tail -5", shell=True, capture_output=True, text=True).stdout
        print(err[-500:])
        break
    b = p.read_bytes()
    if b == romA:
        break
    # degradar funciones que difieren
    src = Path(f"src/{NAME}.c")
    c = src.read_text()
    nm = {l.split()[2]: int(l.split()[0], 16) for l in subprocess.check_output(["mips-linux-gnu-nm", "-n", "build/hotwheelsturbo.elf"], text=True).splitlines() if len(l.split()) == 3}
    defs = re.findall(r"^[\w \*]*?\b(\w+)\([^;{\n]*\) \{$", c, re.M)
    addrs = sorted((nm[d], d) for d in defs if d in nm)
    R = lambda v: v - 0x80060400 + 0x1000
    drop = []
    allsym = sorted(v for k, v in nm.items() if 0x80060400 <= v < 0x800E7AA0)
    for v, d in addrs:
        nxt = next((x for x in allsym if x > v), v + 4)
        if romA[R(v):R(nxt)] != b[R(v):R(nxt)]:
            drop.append(d)
    if not drop and len(b) != len(romA):
        drop = [d for _, d in addrs]
    if not drop:
        drop = [d for _, d in addrs][:1]
    for d in drop:
        m = re.search(r"^[\w \*]*?\b%s\([^;{\n]*\) \{\n.*?^\}\n" % d, c, re.M | re.S)
        if m:
            c = c[:m.start()] + f'INCLUDE_ASM("asm/nonmatchings/{NAME}", {d});\n' + c[m.end():]
    src.write_text(c)
    print("degradadas", len(drop))
ok = Path("build/hotwheelsturbo.z64").exists() and Path("build/hotwheelsturbo.z64").read_bytes() == romA
c = Path(f"src/{NAME}.c").read_text() if Path(f"src/{NAME}.c").exists() else ""
n = len(re.findall(r"^[\w \*]*?\b\w+\([^;{\n]*\) \{$", c, re.M))
print("OK" if ok else "FALLO", NAME, "en C:", n, "asm:", c.count("INCLUDE_ASM("))
if ok:
    subprocess.run(["python3", "tools/progress.py", "--readme"], capture_output=True)
    subprocess.run("git add -A src hotwheelsturbo.yaml Makefile README.md", shell=True)
    subprocess.run(["git", "commit", "-q", "-m", f"Add {NAME}.c\n\n{n} functions in C and {c.count('INCLUDE_ASM(')} in assembly. The ROM is still identical.\n\nCo-Authored-By: Claude <noreply@anthropic.com>"])
else:
    subprocess.run("git checkout -- hotwheelsturbo.yaml Makefile; rm -f src/%s.c" % NAME, shell=True)
    subprocess.run("rm -rf asm/nonmatchings build; make split >/dev/null 2>&1", shell=True)
