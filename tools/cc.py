#!/usr/bin/env python3
"""cc.py <-O2|-O0|...> <src.c> <out.o>: compila con GCC 2.7.2 sin duplicar .rodata.

La ROM enlaza su .rodata desde asm/data/*.rodata.s, de modo que lo que el compilador genera por su
cuenta (dobles como 2^32 de u32 -> f64 y las tablas de saltos de un switch) no debe volver a emitirse:
cada función convertida apunta a los símbolos que ya existen, según rodata_refs.txt.

- `li.d $f,VALOR` pasa a `l.d $f,D_xxx`, tras comprobar que la ROM guarda ese valor en D_xxx.
- La tabla de un switch se quita y sus etiquetas toman el nombre global (jtbl_Lxxx) que espera la tabla de la ROM.

Una función sin entrada en rodata_refs.txt se deja tal cual."""
import os, re, struct, subprocess, sys, tempfile
from pathlib import Path

TC = "tools/gcc_2.7.2/"
FLAGS = ["-G0", "-mips3", "-mgp32", "-mfp32", "-Iinclude"]
REFS = Path("rodata_refs.txt")
ROM_PATH = Path("baserom.z64")
R = lambda v: v - 0x80060400 + 0x1000


class Refs:
    def __init__(self):
        self.refs = {}
        if REFS.exists():
            for line in REFS.read_text().splitlines():
                p = line.split()
                if p and not p[0].startswith("#"):
                    self.refs[p[0]] = p[1:]
        self.rom = None
        self.tables = None

    def double_at(self, sym):
        if self.rom is None:
            self.rom = ROM_PATH.read_bytes()
        return struct.unpack_from(">d", self.rom, R(int(sym[2:], 16)))[0]

    def table(self, sym):
        if self.tables is None:
            self.tables = {}
            for path in Path("asm/data").glob("*.rodata.s"):
                current = None
                for line in path.read_text().splitlines():
                    m = re.match(r"dlabel (\S+)", line)
                    if m:
                        current = self.tables.setdefault(m.group(1), [])
                        continue
                    if line.startswith("enddlabel"):
                        current = None
                    m = re.search(r"\.word (\S+)", line)
                    if m and current is not None:
                        current.append(m.group(1))
        return self.tables.get(sym, [])


def rewrite_function(lines, name, refs, db):
    """Devuelve las líneas de la función con sus referencias a .rodata resueltas."""
    out, i, aliases, tables, k = [], 0, {}, {}, 0
    # Primero, las tablas de switch.
    body = []
    while k < len(lines):
        line = lines[k]
        if line.strip().startswith(".section") and ".rodata" in line:
            j = k + 1
            while lines[j].strip() != ".text":
                j += 1
            block = lines[k + 1:j]
            words = [re.match(r"\s*\.word\s+(\.L\w+)", l) for l in block]
            words = [w.group(1) for w in words if w]
            label = next((re.match(r"(\.L\w+):", l).group(1) for l in block if re.match(r"\.L\w+:", l)), None)
            if words and label:
                tables[label] = words
                k = j + 1
                continue
        body.append(line)
        k += 1
    for line in body:
        m = re.match(r"\s*li\.d\s+(\$f\d+),(\S+)", line)
        if m and float(m.group(2)) != 0.0:
            if i >= len(refs):
                raise SystemExit(f"{name}: li.d {m.group(2)} sin referencia en rodata_refs.txt")
            sym = refs[i]
            if not sym.startswith("D_") or db.double_at(sym) != float(m.group(2)):
                raise SystemExit(f"{name}: li.d {m.group(2)} no coincide con {sym}")
            out.append(f"\tl.d\t{m.group(1)},{sym}")
            i += 1
            continue
        m = re.match(r"\s*l\.d\s+\$f\d+,(D_[0-9A-F]{8})\b", line)
        if m and i < len(refs) and refs[i] == m.group(1):
            i += 1
        m = re.match(r"(\s*lw\s+\$\d+,)(\.L\w+)(\(\$\d+\))", line)
        if m and m.group(2) in tables:
            if i >= len(refs) or not refs[i].startswith("jtbl_"):
                raise SystemExit(f"{name}: tabla de saltos sin referencia en rodata_refs.txt")
            sym = refs[i]
            i += 1
            rom_table = db.table(sym)
            words = tables[m.group(2)]
            if len(rom_table) < len(words):
                raise SystemExit(f"{name}: la tabla {sym} tiene {len(rom_table)} entradas, el switch {len(words)}")
            for ours, theirs in zip(words, rom_table):
                if aliases.setdefault(ours, theirs) != theirs:
                    raise SystemExit(f"{name}: la tabla {sym} no tiene la forma del switch")
            out.append(f"{m.group(1)}{sym}{m.group(3)}")
            continue
        out.append(line)
    result = []
    for line in out:
        result.append(line)
        m = re.match(r"(\.L\w+):", line)
        if m and m.group(1) in aliases:
            result += [f"\t.globl\t{aliases[m.group(1)]}", f"{aliases[m.group(1)]}:"]
    return result


def rewrite(text, db):
    lines, out, k = text.split("\n"), [], 0
    while k < len(lines):
        m = re.match(r"\s*\.ent\s+(\w+)", lines[k])
        if not m or m.group(1) not in db.refs:
            out.append(lines[k])
            k += 1
            continue
        end = k
        while not re.match(r"\s*\.end\s+%s\b" % m.group(1), lines[end]):
            end += 1
        out += rewrite_function(lines[k:end], m.group(1), db.refs[m.group(1)], db)
        k = end
    return "\n".join(out)


def compile_c(opt, src, obj, db=None):
    """Devuelve (código de salida, stderr)."""
    db = db or Refs()
    fd, tmp = tempfile.mkstemp(suffix=".s", dir=os.path.dirname(os.path.abspath(obj)))
    os.close(fd)
    try:
        cmd = [TC + "gcc", "-B" + TC, opt, *FLAGS]
        r = subprocess.run(cmd + ["-S", "-o", tmp, str(src)], capture_output=True, text=True)
        if r.returncode:
            return r.returncode, r.stderr
        try:
            Path(tmp).write_text(rewrite(Path(tmp).read_text(), db))
        except SystemExit as e:
            return 1, str(e)
        r = subprocess.run(cmd + ["-c", "-o", str(obj), tmp], capture_output=True, text=True)
        return r.returncode, r.stderr
    finally:
        # gcc borra su salida si falla
        if os.path.exists(tmp):
            os.unlink(tmp)


def main():
    opt, src, obj = sys.argv[1:4]
    code, err = compile_c(opt, src, obj)
    sys.stderr.write(err + ("\n" if err and not err.endswith("\n") else ""))
    sys.exit(code)


if __name__ == "__main__":
    main()
