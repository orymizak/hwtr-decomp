# REA + Ghidra en el flujo de decomp (evaluado 2026-11-24)

Herramienta de **investigación**; no sustituye a m2c, `try.py`, `hand.py` ni objdiff.

## Preparar
```bash
python3 tools/rea/prepare_elf.py            # -> .scratch/rea/hwtr_rea.elf (copia; ROM y build/ intactos, comprueba SHA-1)
```
Luego, desde Claude: `open_binary(path=<abs>/.scratch/rea/hwtr_rea.elf, provider_id="ghidra")`.
Por qué la copia: el carril MIPS de REA rechaza el ELF del proyecto (e_flags MIPS-III/o32) y los alias
`*.NON_MATCHING` dejaban las funciones pendientes con cuerpo de 1 byte (sin callees ni asm). `raw-binary` no admite MIPS.

## Qué usar
- `procedure_pseudo_code` / `analyze_function` (detail summary; el complete pesa ~130 KB): pseudocódigo tipado.
- `procedure_callers` / `procedure_callees`: grafo de llamadas resuelto.
- `xrefs(address)`: todos los lectores/escritores de un global (p. ej. `gLightsDirty`, 24 sitios).
- `close_binary` al terminar.

## Límites observados
- Tipos de Ghidra sin `context.h`: punteros `undefined4 *` (los pasos de puntero salen en palabras, no en bytes).
- Para decidir tipos/macros GBI hay que reconocer las constantes a mano (p. ej. `0xDC08000A` = `gSPLight`).
- Sin ABI/FP declarado: el modelo de llamadas es una suposición o32.

## Verificación de prepare_elf.py (2026-11-24, integrada en el propio script)
Tras copiar comprueba: (1) `nm -n` idéntico salvo los alias `.NON_MATCHING`; (2) todo alias tenía un símbolo primario en la
misma dirección (no se pierde ningún nombre); (3) cabeceras de sección idénticas (direcciones y tamaños; solo cambian
offsets de symtab/strtab); (4) contenido de las 206 secciones PROGBITS idéntico; (5) el ELF es ya enlazado y no tiene
relocaciones, así que las referencias son direcciones absolutas y se conservan por construcción.

## Limitaciones conocidas del análisis
- Los 4.201 alias `.NON_MATCHING` quitados incluyen funciones en ASM: Ghidra las ve como funciones normales (bien), pero no
  distingue cuáles tienen C ya escrito.
- Ghidra no conoce `context.h`: structs (Car, Node…) salen como `undefined4 *`/offsets; hay que traducir a mano con car.h.
- Referencias por `lui/addiu` + `lw` se resuelven en la mayoría de casos; las tablas de saltos (`jr` + .rodata) y los
  punteros a función en datos pueden faltar en callers/callees (revisar con `xrefs` sobre el símbolo de la tabla).
- El pseudocódigo no refleja el orden de registros ni el scheduling de GCC 2.7.2: sirve para estructura, no para matching fino.
- `complete` pesa ~130 KB por función; usar summary + pseudocódigo.
