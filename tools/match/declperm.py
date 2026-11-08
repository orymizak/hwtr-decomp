"""declperm.py <lote.c> [func] [--max N]: tries every order of a function's local declarations.

GCC 2.7.2 assigns registers partly by declaration order, so a candidate that only differs in register
allocation often matches once its locals are reordered. Takes the first block of declaration lines at the
start of each `//@` function body in the lote, checks each permutation with hand.py and writes the first
match to <lote>.ok.c. Prints the best result per function."""
import itertools, re, subprocess, sys
from pathlib import Path

DECL = re.compile(r"^\s+(?:register\s+)?(?:const\s+)?(?:unsigned\s+|signed\s+)?[A-Za-z_]\w*(?:\s*\*+\s*|\s+)\**\w+(?:\[[^\]]*\])?(?:\s*=\s*[^;]+)?(?:\s*,\s*\**\w+(?:\[[^\]]*\])?(?:\s*=\s*[^;]+)?)*;\s*$")


def check(text, path):
    path.write_text(text)
    out = subprocess.run([sys.executable, "tools/match/hand.py", str(path)], capture_output=True, text=True).stdout
    first = out.splitlines()[0] if out else "no output"
    if "COINCIDE" in first:
        return 0, first
    m = re.search(r": (\d+) palabras", first)
    return (int(m.group(1)) if m else 10 ** 6), first


def main():
    lote = Path(sys.argv[1])
    only = sys.argv[2] if len(sys.argv) > 2 and not sys.argv[2].startswith("--") else None
    limit = int(sys.argv[sys.argv.index("--max") + 1]) if "--max" in sys.argv else 720
    text = lote.read_text()
    tmp = lote.with_suffix(".perm.c")
    for name in re.findall(r"^//@ (\w+)", text, re.M):
        if only and name != only:
            continue
        part = re.search(r"^//@ %s[^\n]*\n.*?(?=^//@ |\Z)" % re.escape(name), text, re.S | re.M).group(0)
        body_start = re.search(r"^[^\n]*\b%s\([^;]*?\)\s*\{\n" % re.escape(name), part, re.M)
        if not body_start:
            print(f"{name}: no se encontró la definición")
            continue
        lines = part[body_start.end():].split("\n")
        block = []
        for line in lines:
            if DECL.match(line):
                block.append(line)
            else:
                break
        if len(block) < 2:
            print(f"{name}: menos de dos declaraciones")
            continue
        best = None
        for n, perm in enumerate(itertools.permutations(block)):
            if n >= limit:
                break
            new_part = part.replace("\n".join(block), "\n".join(perm), 1)
            only_this = text.replace(part, new_part)
            single = re.search(r"^//@ %s[^\n]*\n.*?(?=^//@ |\Z)" % re.escape(name), only_this, re.S | re.M).group(0)
            score, line = check(single, tmp)
            if best is None or score < best[0]:
                best = (score, perm, line)
            if score == 0:
                lote.with_suffix(".ok.c").write_text(single)
                break
        print(f"{name}: {best[2]}")
        if best[0]:
            print("  mejor orden: " + " | ".join(l.strip() for l in best[1]))
    tmp.unlink(missing_ok=True)


if __name__ == "__main__":
    main()
