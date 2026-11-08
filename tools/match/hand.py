#!/usr/bin/env python3
"""hand.py <lote.c> [--apply]: comprueba funciones escritas a mano y, con --apply, las pasa a src/.

Cada función del lote empieza con `//@ nombre [-O0]`, seguida de sus declaraciones y su cuerpo. Compara con la ROM
incluyendo a dónde llevan las reubicaciones (pass2.compare). --apply une las que coinciden y comprueba
la ROM enlazada; quita las que la cambien. No hace commit. Necesita TMP_DIR."""
import difflib, os, re, struct, subprocess, sys, tempfile
from pathlib import Path

os.environ.setdefault("TMP_DIR", ".scratch/hand")
Path(os.environ["TMP_DIR"]).mkdir(parents=True, exist_ok=True)
sys.path.insert(0, "tools/match")
sys.path.insert(0, "tools")
import pass2

ROM = pass2.ROM


def sections(path):
    parts = re.split(r"^//@ ", Path(path).read_text(), flags=re.M)[1:]
    for part in parts:
        head, _, body = part.partition("\n")
        words = head.split()
        # Sin extern en los prototipos: pass2.reconcile los reescribe como cast si chocan.
        body = re.sub(r"^extern ([\w \*]+\(.*\);)$", r"\1", body, flags=re.M)
        yield words[0], next((w for w in words[1:] if w.startswith("-O")), None), body


def unit_of(name):
    hits = list(Path("asm/nonmatchings").glob(f"*/{name}.s"))
    return hits[0].parent.name if hits else None


def diff(o, v, size, name, unit, d):
    x = pass2.section(o, ".text", d)
    x = x[:len(x) - (len(x) - len(x.rstrip(b"\0"))) // 4 * 4]
    ours = [struct.unpack_from(">I", x, i)[0] for i in range(0, len(x), 4)]
    theirs = [struct.unpack_from(">I", ROM, pass2.R(v) + i)[0] for i in range(0, size, 4)]
    def mask(w):
        op = w >> 26
        if op in (2, 3):
            return w & 0xFC000000
        return w & 0xFFFF0000 if op in {9, 15, 32, 33, 35, 36, 37, 40, 41, 43, 49, 53, 57, 61} else w
    a, b = [f"{mask(w):08X}" for w in theirs], [f"{mask(w):08X}" for w in ours]
    lines = []
    for line in Path(f"asm/nonmatchings/{unit}/{name}.s").read_text().splitlines():
        m = re.match(r"\s*/\* \S+ \S+ [0-9A-F]{8} \*/\s+(\S+)\s*(.*)", line)
        if m:
            lines.append(m.group(1) + " " + re.sub(r"\s+", " ", m.group(2)))
    dump = subprocess.run(["mips-linux-gnu-objdump", "-d", "-j", ".text", "-M", "reg-names=numeric", str(o)], capture_output=True, text=True).stdout
    ins = [l.split("\t", 2) for l in dump.splitlines() if re.match(r"\s+[0-9a-f]+:\t", l)]
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    same = sum(block.size for block in sm.get_matching_blocks())
    print(f"  iguales {same}/{max(len(a), len(b))}  tamaño {len(x)}/{size}")
    shown = 0
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == "equal":
            continue
        for i in range(i1, i2):
            print(f"  - {i * 4:4X} {lines[i] if i < len(lines) else ''}")
        for j in range(j1, j2):
            print(f"  + {j * 4:4X} {re.sub(chr(9), ' ', ins[j][2]) if j < len(ins) and len(ins[j]) > 2 else ''}")
        shown += 1
        if shown > 8:
            print("  ...")
            break


def check(name, opt, body):
    unit = unit_of(name)
    if not unit:
        print(f"{name}: sin ensamblador")
        return None
    opt = opt or pass2.opt_of(unit)
    v, size, _ = pass2.asm_of(unit, name)
    with tempfile.TemporaryDirectory() as d:
        d = Path(d)
        code, err = pass2.cc.compile_c(opt, _write(d, body), d / "a.o")
        if code:
            print(f"{name}: no compila\n{err[:600]}")
            return None
        dif, rodata = pass2.compare(d / "a.o", v, size, d)
        if dif == 0 and not rodata:
            print(f"{name}: COINCIDE ({size:#x})")
            return unit, opt, size
        print(f"{name}: {dif} palabras distintas{' (rodata)' if rodata else ''}")
        diff(d / "a.o", v, size, name, unit, d)
    return None


def _write(d, body):
    f = d / "a.c"
    f.write_text('#include "common.h"\n' + body)
    return f


def built():
    return Path("build/hotwheelsturbo.z64").read_bytes() if Path("build/hotwheelsturbo.z64").exists() else b""


def explain():
    """Dice por qué falló el enlazado o en qué función cambió la ROM."""
    b = built()
    if len(b) != len(ROM):
        out = subprocess.run("make 2>&1 | grep -E 'undefined|multiple|rror' | head -3", shell=True, capture_output=True, text=True).stdout
        print("  enlazado:", out.strip()[:300])
        return
    first = next((i for i in range(len(ROM)) if ROM[i] != b[i]), None)
    if first is not None:
        v = first - 0x1000 + 0x80060400
        nm = pass2.nm_map()
        near = max((a, n) for n, a in nm.items() if a <= v)
        print(f"  primera diferencia en {v:08X}, dentro de {near[1]}")


def forward_protos(c, name, body):
    """Las funciones que el candidato llama y que la unidad define más abajo necesitan su prototipo exacto
    antes del punto de inserción; si no, pass2.reconcile las convierte en un cast sin declarar."""
    marker = re.search(r'INCLUDE_ASM\("asm/nonmatchings/\w+", %s\);' % re.escape(name), c)
    if not marker:
        return c
    before, after = c[:marker.start()], c[marker.start():]
    protos = []
    for m in re.finditer(r"^([\w \*]+?\b(\w+)\(([^;{\n]*)\)) \{$", after, re.M):
        callee = m.group(2)
        if callee != name and re.search(r"\b%s\(" % re.escape(callee), body) and \
                not re.search(r"\b%s\(" % re.escape(callee), before):
            protos.append(m.group(1) + ";")
    return before + "".join(p + "\n" for p in protos) + after if protos else c


def apply(ok):
    by_unit = {}
    for name, (unit, opt, body) in ok.items():
        by_unit.setdefault(unit, []).append((name, opt, body))
    for unit, items in by_unit.items():
        path = Path(f"src/{unit}.c")
        orig = path.read_text()
        def attempt(names):
            c = orig
            for name, opt, body in items:
                if name in names:
                    merged = pass2.merge(forward_protos(c, name, body), name, body, opt)
                    if merged is None:
                        print(f"{name}: no se puede unir a {unit}")
                        tu = Path(os.environ["TMP_DIR"]).resolve() / "tu" / "tu.c"
                        if tu.exists():
                            err = pass2.cc.compile_c(opt, tu, tu.with_suffix(".o"))[1]
                            print("\n".join(l for l in err.splitlines() if "warning" not in l and "In function" not in l)[:500])
                    else:
                        c = merged
            path.write_text(c)
            if os.environ.get("NO_BUILD") == "1":
                # Sin compilar la ROM: basta con que la unidad compile con todas las funciones unidas.
                return all(f'", {n});' not in c for n in names)
            ok = pass2.make(unit) == ROM
            if not ok:
                explain()
            return ok
        names = [n for n, _, _ in items]
        if attempt(names):
            text = path.read_text()
            left = [n for n in names if f'", {n});' in text]
            done_msg = "compila (ROM sin comprobar)" if os.environ.get("NO_BUILD") == "1" else "ROM idéntica"
            print(f"{unit}: {len(names) - len(left)}/{len(names)} aplicadas, {done_msg}")
            continue
        good = []
        for n in names:
            if attempt(good + [n]):
                good.append(n)
            else:
                print(f"{n}: cambia la ROM enlazada, se deja en ensamblador")
        if not attempt(good):
            path.write_text(orig)
            pass2.make(unit)
            sys.exit(f"{unit}: la ROM no coincide tras restaurar")
        done_msg = "compila (ROM sin comprobar)" if os.environ.get("NO_BUILD") == "1" else "ROM idéntica"
        print(f"{unit}: {len(good)}/{len(names)} aplicadas, {done_msg}")


def main():
    ok = {}
    for name, opt, body in sections(sys.argv[1]):
        r = check(name, opt, body)
        if r:
            ok[name] = (r[0], r[1], body)
    if "--apply" in sys.argv and ok:
        apply(ok)


main()
