#!/usr/bin/env python3
"""Reject inline assembly in decompiled C.

A function only counts as decompiled when it is plain C. Pasting the original
disassembly into asm() bodies, pinning variables to registers with
`register ... asm("$n")`, injecting nops or emitting .space/.word padding makes
the ROM match without decompiling anything. Those functions belong in
INCLUDE_ASM (with a NONMATCHING note holding the closest C), not in src/ as C.

Hits that predate this check are listed in tools/asm_allowlist.txt as match
debt. The allowlist only shrinks: fixing one of them means deleting its line.

Usage:
  tools/check_no_asm.py            check src/ and include/ in the working tree
  tools/check_no_asm.py --staged   check the staged version of staged files (pre-commit)
  tools/check_no_asm.py --update   rewrite the allowlist from the working tree (needs review!)
"""
import collections
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
ALLOWLIST = ROOT / "tools" / "asm_allowlist.txt"
DIRS = ("src", "include")
EXEMPT = {"include/include_asm.h"}  # defines INCLUDE_ASM / INCLUDE_RODATA themselves

PATTERNS = [
    (re.compile(r"(?<![A-Za-z0-9_])(?:asm|__asm__|__asm)\s*(?:volatile\s*|__volatile__\s*)?\("),
     "inline asm / register pinned with asm(\"$n\")"),
    (re.compile(r"""["']\s*\.(?:space|skip|word|half|byte|fill|set|section|text)\b"""),
     "assembler directive inside a string"),
    (re.compile(r"^\s*\.(?:space|skip|word|half|byte|fill)\b"),
     "raw assembler directive"),
]

COMMENT_RE = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)


def strip_comments(text):
    # Keep newlines so line numbers stay right.
    return COMMENT_RE.sub(lambda m: "\n" * m.group(0).count("\n"), text)


def norm(line):
    return " ".join(line.split())


def hits(path, text):
    out = []
    for no, line in enumerate(strip_comments(text).splitlines(), 1):
        for rx, why in PATTERNS:
            if rx.search(line):
                out.append((path, no, norm(line), why))
                break
    return out


def is_source(path):
    return (path.startswith(tuple(d + "/" for d in DIRS))
            and path.endswith((".c", ".h")) and path not in EXEMPT)


def worktree_files():
    for d in DIRS:
        for p in sorted((ROOT / d).rglob("*")):
            rel = p.relative_to(ROOT).as_posix()
            if p.is_file() and is_source(rel):
                yield rel, p.read_text(errors="replace")


def staged_files():
    names = subprocess.run(
        ["git", "diff", "--cached", "--name-only", "--diff-filter=ACMR"],
        cwd=ROOT, check=True, capture_output=True, text=True).stdout.split()
    for rel in names:
        if is_source(rel):
            blob = subprocess.run(["git", "show", ":" + rel], cwd=ROOT, check=True,
                                  capture_output=True).stdout
            yield rel, blob.decode(errors="replace")


def load_allowlist():
    allowed = collections.Counter()
    if ALLOWLIST.exists():
        for line in ALLOWLIST.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                path, _, code = line.partition("\t")
                allowed[(path, code)] += 1
    return allowed


def main(argv):
    if "--update" in argv:
        found = [h for rel, text in worktree_files() for h in hits(rel, text)]
        lines = ["# Match debt: inline asm that predates tools/check_no_asm.py.",
                 "# One line per hit: <path><TAB><normalized code>. Only ever delete lines."]
        lines += [f"{p}\t{c}" for p, _, c, _ in found]
        ALLOWLIST.write_text("\n".join(lines) + "\n")
        print(f"{ALLOWLIST.relative_to(ROOT)}: {len(found)} entries")
        return 0

    files = staged_files() if "--staged" in argv else worktree_files()
    allowed = load_allowlist()
    used = collections.Counter()
    bad = []
    for rel, text in files:
        for path, no, code, why in hits(rel, text):
            key = (path, code)
            used[key] += 1
            if used[key] > allowed[key]:
                bad.append(f"{path}:{no}: {why}\n    {code}")

    if bad:
        print("check_no_asm: inline assembly is not decompilation.\n", file=sys.stderr)
        print("\n".join(bad), file=sys.stderr)
        print("\nWrite the function as plain C. If it does not match without these tricks,\n"
              "leave it as INCLUDE_ASM and put the closest C in a NONMATCHING comment.\n"
              "Do not add these lines to tools/asm_allowlist.txt.", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
