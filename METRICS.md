# Progress metrics

**Official reference: objdiff (ByteMatch).** `make report` (tools/objdiff_report.py) writes
`report/us/report.json`; its `measures.matched_code_percent` is the number to quote.
Do not compare figures across metrics: only compare two objdiff runs with each other.

| Tool | What it counts | Total | Matched | % |
|------|----------------|-------|---------|---|
| objdiff (official) | bytes of functions whose bytes equal the original, exact symbol sizes | 552,760 B / 1,910 fn | 434,864 B / 1,791 fn | 78.67% |
| `tools/inventory.py` | bytes of functions that have C source (not `INCLUDE_ASM`), size = distance to the next symbol | 554,576 B / 1,915 fn | 436,424 B / 1,797 fn | 78.70% |

(Figures measured on 2026-11-25, end of session 44, by re-running `make` + `python3 tools/objdiff_report.py`
on the working tree (HEAD 8f28b9e + uncommitted changes). ROM SHA-1 `94913a07…` OK; `tools/match/verify_funcs.py`:
1,765 C functions, 403,604 B, all at their final address. `NON_MATCHING` is not defined in the build, so reconstructions
guarded by `#ifdef NON_MATCHING` fall back to `INCLUDE_ASM` and contribute 0 B to either row. Complete units: 173/221.)

### Progress to the goal (objdiff)
- Matched: **434,864 B** of 552,760 B = **78.67 %** (1,791 / 1,910 functions, 93.77 %).
- 80 % = 442,208 B, so **7,344 B** remain to reach it.
- Previous objdiff points (do not mix with inventory.py): 74.77 % (413,280 B, HEAD 31d1fab) → 74.97 % (414,416 B,
  after `Car_ComputeMass`) → 76.74 % (424,184 B, session 41) → 78.00 % (431,168 B) → 78.67 % (434,864 B, now). Net since 31d1fab: +21,584 B, +17 functions.

## Why they differ (measured, not assumed)

1. **Size basis.** inventory.py sizes a function as the gap to the next `nm` symbol, so alignment padding is
   attributed to the previous function. objdiff uses the exact symbol size. 228 functions differ, net 2,256 B
   (e.g. `Debug_DrawChars` 316 vs 304).
2. **Symbols counted.** inventory.py includes 8 linker marker symbols (`entry_*`, `main_TEXT_START`, `main_VRAM`);
   objdiff does not, and objdiff has 3 symbols inventory.py lacks (`entrypoint`, `__osPackReadData`,
   `__osPfsCheckRamArea`).
3. **Meaning of "done".** inventory.py: "has C source". objdiff: "bytes match". With a byte-identical ROM they
   agree in practice, but a function listed as done in C that did not match would only show up in objdiff.

## Rules

- Quote objdiff for progress and for the 80 % goal. Use inventory.py only to list what is pending
  (sizes, units, `INCLUDE_ASM` names, match-debt hits).
- Never write "went from X (inventory) to Y (objdiff)". Re-run the same tool before and after.
- Older numbers in SESSION.md / commit messages (e.g. 74.77 %) are objdiff figures; the 72.9x / 74.87 % values
  in older notes may come from `tools/progress.py` (its own base, 554,576 B) and are not comparable.
- Baseline objdiff at HEAD 31d1fab: 413,280 / 552,760 B (74.77 %), 1,774 functions.
- 80 % of 552,760 B = 442,208 B, i.e. about 27,800 B still to match by objdiff.
