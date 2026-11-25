#!/usr/bin/env python3
"""prepare_elf.py [salida.elf]: copia de build/hotwheelsturbo.elf apta para REA/Ghidra.

REA (Ghidra, carril MIPS) solo admite ELF con e_flags sin ABI ni ISA declarados (MIPS-I "recordless"); el ELF
del proyecto declara MIPS-III/o32 y se rechaza. Esto escribe una COPIA con e_flags = 0 y sin los alias
`*.NON_MATCHING` (Ghidra los tomaba como nombre primario y la funcion quedaba con cuerpo de 1 byte).
Ni la ROM ni build/ se modifican; se comprueba el SHA-1 de baserom.z64 antes y despues.
Por defecto escribe en .scratch/rea/hwtr_rea.elf (ignorado por git)."""
import hashlib, subprocess, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SHA1 = "94913a07e49005ffd43dfdfa16ff5862bdb93748"


def sha1(p):
    return hashlib.sha1(Path(p).read_bytes()).hexdigest()


def verify(src, out, strip):
    """Comprueba que la copia conserva simbolos, direcciones y contenido de todas las secciones."""
    import tempfile
    run = lambda *c: subprocess.run(c, capture_output=True, text=True, check=True).stdout.splitlines()
    keep = lambda p: [l for l in run("mips-linux-gnu-nm", "-n", str(p)) if l.split()[-1] not in strip]
    assert keep(src) == keep(out), "simbolos/direcciones distintos tras objcopy"
    primarios = {l.split()[0] for l in keep(src) if len(l.split()) == 3}
    huerf = [l for l in run("mips-linux-gnu-nm", str(src)) if l.split()[-1] in strip and l.split()[0] not in primarios]
    assert not huerf, f"alias sin simbolo primario: {huerf[:3]}"
    secs = lambda p: [" ".join(l.split()) for l in run("mips-linux-gnu-readelf", "-SW", str(p))
                      if "[" in l and "symtab" not in l and "strtab" not in l and "Nr]" not in l]
    assert secs(src) == secs(out), "cabeceras de seccion distintas"
    names = [l.split()[1] for l in secs(src) if " PROGBITS " in l]
    with tempfile.TemporaryDirectory() as t:
        for n in names:
            for tag, p in (("a", src), ("b", out)):
                subprocess.run(["mips-linux-gnu-objcopy", "-O", "binary", "-j", n, str(p), f"{t}/{tag}"], check=True)
            assert Path(f"{t}/a").read_bytes() == Path(f"{t}/b").read_bytes(), f"contenido distinto en {n}"
    print(f"verificado: simbolos y direcciones identicos, {len(names)} secciones PROGBITS identicas, ELF sin relocs")


def main():
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / ".scratch/rea/hwtr_rea.elf"
    out.parent.mkdir(parents=True, exist_ok=True)
    assert sha1(ROOT / "baserom.z64") == SHA1, "baserom.z64 no coincide con el SHA-1 original"
    src = ROOT / "build/hotwheelsturbo.elf"
    syms = subprocess.run(["mips-linux-gnu-nm", str(src)], capture_output=True, text=True, check=True).stdout.split()
    strip = [s for s in syms if s.endswith(".NON_MATCHING")]
    cmd = ["mips-linux-gnu-objcopy"] + [f"--strip-symbol={s}" for s in strip] + [str(src), str(out)]
    subprocess.run(cmd, check=True)
    d = bytearray(out.read_bytes())
    d[0x24:0x28] = b"\0\0\0\0"
    out.write_bytes(d)
    assert sha1(ROOT / "baserom.z64") == SHA1
    verify(src, out, set(strip))
    print(f"{out} listo ({len(strip)} alias NON_MATCHING eliminados, e_flags=0); ROM intacta")


main()
