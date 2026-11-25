# HWTR Decompilation Workflow & Rules

## Trigger: "continúa la decomp de HWTR" (o similar)
Cuando el usuario indique "continúa la decomp de HWTR" o pida continuar con el proceso de decompilación:

### 1. Metodología de Análisis Obligatoria (4 Pasos)
1. **Analizar el ASM objetivo:** Examinar stack frame (tamaño, registros callee-saved preservados), argumentos de entrada en registros y stack del llamador, llamadas a funciones y estructura de saltos/bloques.
2. **Comparar contra ASM actual / Diff:** Identificar la causa raíz más probable de las diferencias (tipos con/sin signo, layout de structs/offsets, orden de expresiones, ramas, o patrones de asignación de registros de GCC 2.7.2).
3. **Proponer cambios incrementales y concretos en C:** Ajustar el código en C explicando con precisión qué cambia y por qué afecta a esas instrucciones específicas.
4. **Falta de contexto:** Si falta contexto para decidir entre dos formas equivalentes en C (p. ej. definición de struct, tipo de retorno o campos/argumentos no usados), añadir un comentario `/* TODO ... */` documentando brevemente la situación antes de aplicar una solución a ciegas.

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
* Confirmar usando `./tools/match/commit_unit.sh "<mensaje>" <archivos>`:
  * Verifica automáticamente el SHA-1 de la ROM (`94913a07e49005ffd43dfdfa16ff5862bdb93748`) y `verify_funcs.py`.
  * Cada 3 commits compila con `make clean && make`; en los demás, incremental.
  * Respeta el intervalo de 5 minutos entre commits (usar `--force` solo al cerrar una tanda completa).
* Actualizar las métricas de avance de ByteMatch ejecutando `python3 tools/objdiff_report.py`.
