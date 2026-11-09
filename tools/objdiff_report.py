#!/usr/bin/env python3
"""Generates the objdiff progress report that ByteMatch reads (make report). Requires a matching build (make).

- Target objects: the objects of the matching build, copied to build/expected/. Since the ROM is
  byte-identical, they hold every function exactly as in the original game.
- Base objects: each src/*.c compiled again with INCLUDE_ASM empty, in build/base/, so only the
  functions written in C are present.
- Units are grouped by zone (game, audio, system, libultra, libc) using their address in the map file.

Writes objdiff.json, runs objdiff-cli and copies the result to report/us/report.json, which the
CI workflow uploads as the `us_report` artifact."""
import json, re, shutil, subprocess, sys
from pathlib import Path

sys.path.insert(0, "tools")
import cc

VERSION = "us"
OBJDIFF = "tools/objdiff-cli"
MAP = Path("build/hotwheelsturbo.map")
ZONES = [
    ("game", 0x80060400, 0x800CD600),
    ("audio", 0x800CD600, 0x800D5560),
    ("system", 0x800D5560, 0x800D7CB0),
    ("libultra", 0x800D7CB0, 0x800E6990),
    ("libc", 0x800E6990, 0x800E7AA0),
]


def text_objects():
    """(address, object path) of every object with code, in link order."""
    found = re.findall(r"^ \.text\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (build/(?:src|asm)/\S+\.o)$", MAP.read_text(), re.M)
    return [(int(a, 16), o) for a, size, o in found if int(size, 16)]


def opt_flags():
    flags = dict(re.findall(r"^\$\(BUILD\)/src/(\S+)\.o: OPTFLAGS := (\S+)", Path("Makefile").read_text(), re.M))
    return lambda stem: flags.get(stem, "-O2")


def main():
    if not MAP.exists():
        sys.exit("build/hotwheelsturbo.map not found: run make first")
    opt = opt_flags()
    cc.FLAGS = cc.FLAGS + ["-DINCLUDE_ASM(a,b)="]
    db = cc.Refs()
    units = []
    for addr, obj in text_objects():
        zone = next(z for z, lo, hi in ZONES if lo <= addr < hi)
        stem = Path(obj).stem
        target = Path("build/expected") / Path(obj).relative_to("build")
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(obj, target)
        unit = {"name": f"{zone}/{stem}", "target_path": str(target), "metadata": {"progress_categories": [zone]}}
        if obj.startswith("build/src/"):
            base = Path("build/base") / Path(obj).relative_to("build")
            base.parent.mkdir(parents=True, exist_ok=True)
            code, err = cc.compile_c(opt(stem), f"src/{stem}.c", base, db)
            if code:
                sys.exit(f"src/{stem}.c: {err[:400]}")
            unit["base_path"] = str(base)
            unit["metadata"]["source_path"] = f"src/{stem}.c"
            unit["metadata"]["complete"] = "INCLUDE_ASM(" not in Path(f"src/{stem}.c").read_text()
        else:
            # Segment kept as handwritten assembly: counted, never matched.
            unit["metadata"]["source_path"] = obj.replace("build/", "", 1).replace(".o", ".s")
        units.append(unit)
    config = {
        "min_version": "2.0.0",
        "custom_make": "make",
        "build_target": False,
        "build_base": False,
        "units": units,
        "progress_categories": [{"id": z, "name": z} for z, _, _ in ZONES],
    }
    Path("objdiff.json").write_text(json.dumps(config, indent=2) + "\n")
    out = Path(f"build/{VERSION}/report.json")
    out.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run([OBJDIFF, "report", "generate", "-o", str(out)], check=True)
    dest = Path(f"report/{VERSION}/report.json")
    dest.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(out, dest)
    subprocess.run([sys.executable, "tools/treemap.py", str(dest), str(dest.with_name("units.svg"))], check=True)
    measures = json.loads(out.read_text()).get("measures", {})
    print(f"{dest}: {measures.get('matched_code_percent', 0):.2f}% code, "
          f"{measures.get('matched_functions', 0)}/{measures.get('total_functions', 0)} functions")


if __name__ == "__main__":
    main()
