# HWTR Decompilation Workflow & Rules

## Trigger: "continúa la decomp de HWTR" (o similar)
Cuando el usuario indique "continúa la decomp de HWTR" o pida continuar con el proceso de decompilación:

### 1. Metodología de Análisis Obligatoria (4 Pasos)
1. **Analizar el ASM objetivo (y REA/Ghidra cuando aporte contexto):** Examinar stack frame (tamaño, registros callee-saved preservados), argumentos de entrada en registros y stack del llamador, llamadas a funciones y estructura de saltos/bloques. Para obtener pseudocódigo tipado, callers/callees y referencias cruzadas (`xrefs`) de globales/structs, usar `python3 tools/rea/rea_query.py <Func> [--xrefs <sym/addr>]` (tras `python3 tools/rea/prepare_elf.py`; ver `tools/rea/README.md`).
2. **Comparar contra ASM actual / Diff:** Identificar la causa raíz más probable de las diferencias (tipos con/sin signo, layout de structs/offsets, orden de expresiones, ramas, o patrones de asignación de registros de GCC 2.7.2). Contrastar siempre el pseudocódigo de Ghidra con el ASM original (Ghidra fusiona colas de bloques que en GCC 2.7.2 provienen de ternarios o ramas duplicadas).
3. **Proponer cambios incrementales y concretos en C:** Ajustar el código en C explicando con precisión qué cambia y por qué afecta a esas instrucciones específicas.
4. **Falta de contexto:** Si falta contexto para decidir entre dos formas equivalentes en C (p. ej. definición de struct, tipo de retorno o campos/argumentos no usados), añadir un comentario `/* TODO ... */` documentando brevemente la situación antes de aplicar una solución a ciegas.

### 1b. Metodología REA + Ghidra (parte activa de cada función candidata)
REA + Ghidra no son un último recurso: se usan desde el principio para descubrir pseudocódigo y estructura lógica, tipos de argumentos/variables
(p. ej. `u8` vs `int`), callers/callees, referencias cruzadas, globals/structs/offsets/strides y relaciones entre funciones que comparten datos o patrones.
Preparación (una vez por sesión): `python3 tools/rea/prepare_elf.py`; consulta: `python3 tools/rea/rea_query.py <Func>... [--xrefs <sym/addr>...]` (ver `tools/rea/README.md`).

1. **Analizar el ASM original** (paso 1 de arriba).
2. **Consultar REA/Ghidra** antes de escribir C: `procedure_pseudo_code`, `procedure_callers`/`procedure_callees`, `xrefs` de cada global/campo tocado y
   funciones hermanas ya igualadas. No hace falta ejecutar todas las consultas en cada función, pero omitir una consulta relevante exige una
   **justificación técnica** anotada en `MATCH_LOG.md` (p. ej. «sin globals ni llamadas»); «prefiero solo ASM» no es justificación.
3. **Contrastar el pseudocódigo con las instrucciones MIPS y GCC 2.7.2** (paso 2): el pseudocódigo es una pista, nunca la fuente de verdad (perfil de 32 bits,
   colas de bloques fusionadas, sin `include/*.h`); el ASM manda.
4. **Formular hipótesis concretas** sobre firma, tipos, registros, structs/offsets y orden de operaciones, indicando qué instrucciones deberían cambiar (pasos 3 y 4).
5. **Probar cambios incrementales y medir** con comparación por palabras/`try.py`, `make` (SHA-1) + `verify_funcs.py` y `tools/objdiff_report.py` (número oficial).
6. **Sin progreso:** investigar hipótesis nuevas o funciones relacionadas (otros callers, xrefs, hermanas, `tools/match/PATTERNS.md`) antes de repetir intentos; se aplica el límite de la sección 2.

**Ejemplos reales** (detalle en `MATCH_LOG.md`; solo los match exactos cuentan como ByteMatch recuperado):
| Función | Resultado | ByteMatch recuperado | Aporte de REA/Ghidra |
|---|---|---|---|
| `Ai_ApplyImpulse` | **MATCH exacto**, 272/272, 1.088 B | 1.088 B | pseudocódigo + xrefs de `D_80114A7C/80/84`; contrastado con el ASM: `ctlTimer = 2` dentro de cada rama y comparaciones `val > min` |
| `LZ_AdvanceBits` | **MATCH exacto**, 42/42 palabras | 168 B | Ghidra tipó el argumento como `char` (u8) en vez de `int n`: pista decisiva |
| `Model_SwapPalette` | NON_MATCHING 72/81 | 0 B | estructura (`Model.palIdx`, `pals[].colors`); residual: orden de 3 `sll` |
| `SndCmd_SetEnvelopePreset` | NON_MATCHING 79/83 | 0 B | varint y tabla de 7 bytes; residual: copia del puntero en `$a3` |
| `PauseUi_DrawText` | NON_MATCHING ~71/88 | 0 B | dos recorridos (medir/dibujar), kerning; residual: cabecera de bucle y prólogo |

Los resultados parciales (NON_MATCHING) se conservan bajo `#ifdef NON_MATCHING` con `INCLUDE_ASM` en el `#else` y **no** se presentan como ByteMatch recuperado.

### 2. Regla de Reintentos y NON_MATCHING (criterios de 2026-11-24)
* Límite: **2 sesiones** por función sin progreso medible (diff que baja o hipótesis nueva comprobable). Con avance real o hipótesis nueva, una ronda más.
* Una reconstrucción C casi exacta se conserva en `src/` así, y **no cuenta como ByteMatch**:
  `#ifdef NON_MATCHING` (C) `#else` `INCLUDE_ASM(...)` `#endif`, con comentario `TODO(match-debt)` y el diff residual. La ruta normal de build sigue siendo `INCLUDE_ASM`; SHA-1 intacto.
* Nunca se descarta una función provisional; se retoma cuando aparezcan patrones nuevos.
* Prohibido para igualar: `asm()`, nops/`.space`, stubs, tocar la ROM (`tools/check_no_asm.py`).
* Documentar en `## Casi iguales, sin convertir` de `tools/match/PATTERNS.md` con diff y causa.
* Prioridad: bytes recuperables x probabilidad de match; aprovechar para descubrir structs/tipos/prototipos compartidos. Completar unidades solo si ayuda a resolver dependencias.
* Inventario de referencia: `python3 tools/inventory.py [--top N] [--mid]` (tras `make`).

### 3. Criterio de Selección de Funciones
* Priorizar y cerrar los candidatos ya iniciados en el directorio raíz (`cand_*.c`: `cand_wheel.c`, `cand_quat.c`, `cand_scroller.c`, `cand_lap.c`, etc.).
* Continuar con las funciones listadas en `python3 tools/match/targets.py` en orden de menor a mayor complejidad/tamaño.

### 4. Integración y Commits al lograr Match (100%)
* Al lograr coincidencia exacta, integrar la función en su archivo correspondiente en `src/unit_*.c`.
* **Política de commits agrupados:** no se hace un commit por función. El objetivo es **un commit cada ~30 funciones con MATCH exacto**.
  * Las funciones `NON_MATCHING` (reconstrucciones bajo `#ifdef NON_MATCHING`) **no cuentan** para las 30.
  * Agrupar funciones relacionadas (misma unidad, mismos structs/patrones) cuando sea conveniente.
  * Si se completa una unidad lógica importante antes de llegar a 30, puede evaluarse como punto de commit.
  * Antes de cada commit, listar los archivos que se incluirán (`git status`/`git diff --stat`) y **preservar cualquier WIP ajeno a la tarea**
    (incluidos los TODOs que el usuario añade en `src/`): no revertirlo ni mezclar cambios no relacionados sin avisar.
* **Compilaciones completas: solo las hace el usuario.** El agente nunca ejecuta `make clean` ni `make clean && make`; usa `make` incremental
  para comprobar la ROM. `commit_unit.sh` tampoco las ejecuta.
* Preparar/confirmar con `./tools/match/commit_unit.sh [--commit] [--force] "<mensaje>" <archivos>`:
  * **Sin `--commit` (por defecto)** el script solo prepara y **se detiene antes del commit**: `make` incremental, SHA-1 de la ROM
    (`94913a07e49005ffd43dfdfa16ff5862bdb93748`), `verify_funcs.py`, `tools/objdiff_report.py` + comprobación de que `report/us/report.json`
    coincide con `build/us/report.json`, recuento de funciones con MATCH exacto ganadas respecto a HEAD, y lista de archivos a incluir.
  * El usuario revisa, hace su `make clean && make` y solo entonces se ejecuta con `--commit`, que repite SHA-1 y `verify_funcs.py` (sin `make clean`).
  * Con `--commit` no confirma si hay menos de ~30 funciones nuevas (`TARGET_FUNCS`) ni antes de 5 minutos desde el último commit;
    `--force` salta ambas condiciones (cierre de unidad lógica o de tanda).
* Antes de cada commit, verificar el resultado contra objdiff (lo hace el script) y registrar el porcentaje en `METRICS.md`/`SESSION.md`.

### 5. Registro en `MATCH_LOG.md` (obligatorio por función)
Cada entrada separa con subtítulos: **REA/Ghidra** (qué consultas y qué dijeron), **Confirmado en ASM** (qué se verificó contra el `.s`),
**Hipótesis**, **Cambios**, **Resultado objdiff** y **Bytes recuperados**. Distinguir siempre «MATCH exacto» (cuenta en objdiff) de
«NON_MATCHING» (reconstrucción casi exacta bajo `#ifdef NON_MATCHING`, **0 B** oficiales). Los porcentajes se citan solo de objdiff (`METRICS.md`).
