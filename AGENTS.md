# HWTR Decompilation Workflow & Rules

## Trigger: "continúa la decomp de HWTR" (o similar)
Cuando el usuario indique "continúa la decomp de HWTR" o pida continuar con el proceso de decompilación:

### 1. Arranque Rápido (evitar recorridos innecesarios)
* Leer **solo** la última sesión de `SESSION.md` (~30 líneas) y ejecutar `python3 tools/inventory.py` (o buscar `TODO(match-debt)` en `src/`).
* **No** leer enteros `MATCH_LOG.md`, `SESSION.md` ni `tools/match/PATTERNS.md` al inicio; consultarlos solo de forma puntual si hace falta contexto de una función o patrón concreto.
* **Comprobar REA antes de nada** (una vez por sesión, ver §2.0). Si falla, **parar y avisar al usuario**.
* Elegir **2–3 funciones objetivo** concretas y trabajarlas **en profundidad** (no repartir 3 hipótesis superficiales entre 10+ funciones: eso no ha cerrado matches en las últimas sesiones).

### 2. Metodología de Análisis (ASM + REA/Ghidra)
0. **Arranque de REA (obligatorio, una vez por sesión):**
   * Ejecutar `python3 tools/rea/rea_query.py <Func1> <Func2> <Func3> [--xrefs <sym>...]` con **todas** las funciones objetivo en **una sola llamada** (cada llamada reimporta el ELF en Ghidra).
   * `rea` se instala con nvm y **no está en el PATH de shells no interactivos**; `rea_query.py` ya lo localiza solo (`PATH`, `REA_BIN` o `~/.nvm/versions/node/*/bin/rea`). No invocar `rea` directamente.
   * Ejecutarlo **fuera del sandbox de terminal** (necesita abrir el socket Unix de Ghidra headless) y con timeout ≥ 3 min: la primera consulta tarda ~30–100 s en importar el ELF; **no abortarla**.
   * Si `rea_query.py` termina con error, **detenerse y avisar al usuario** con el mensaje exacto. **No** continuar "solo con ASM" ni anotarlo como "no disponible" sin avisar.
   * `Callers`/`Callees` vacíos no son un fallo: las llamadas indirectas (punteros a función como `sysRandRange`, tablas) no se listan.
1. **Analizar el ASM objetivo:** examinar stack frame, registros callee-saved, argumentos en registros/stack, llamadas y estructura de saltos/bloques.
2. **Uso de REA + Ghidra:** consultar REA para **cualquier** función objetivo **antes de escribir o ajustar C** (`procedure_pseudo_code`, callers/callees, `xrefs` de globals/campos tocados) para descubrir estructura, tipos (`u8` vs `int`), offsets y strides. Volver a lanzarlo al retomar una función pendiente o cuando el diff se estanque por estructura/tipos.
   * **Para qué sirve REA:** estructura de control, tipos, offsets, strides, layout de structs compartidos (clave en `Ai_Step` y `Coll_*`).
   * **Para qué NO sirve:** diferencias de asignación de registros, orden de bloques, scheduling de delay slots o `beql`/`bnel` de GCC 2.7.2. Para esos residuos usar los permutadores (§3).
3. **Contrastar con ASM y GCC 2.7.2:** el pseudocódigo de Ghidra es una pista (perfil de 32 bits, colas fusionadas, sin `include/*.h`); **el ASM manda siempre**. Formular hipótesis concretas indicando qué instrucciones deben cambiar y por qué.
4. **Cambios incrementales y dudas:** ajustar C de forma incremental. Si falta contexto para decidir entre dos formas equivalentes (struct, retorno, campo), dejar un comentario `/* TODO ... */` antes de adivinar a ciegas.

### 3. Prioridad de Selección, Presupuesto y `NON_MATCHING`
* **Fuera de alcance:** funciones de ensamblador puro de libc/libultra sin `INCLUDE_ASM` (`bcopy`, `bcmp`, `__osException`, `__osDispatchThread`, `strcmp`, `__sincos`, `lldiv`, `func_800E7xxx`…): **no trabajarlas** (libultra no se toca). El techo realista de `objdiff` es < 100 %.
* **Prioridad 1 — casi-matches:** `NON_MATCHING` a < 10 palabras (p. ej. `Hud_DrawTurbos` 377/382, residual solo en offsets de spill por `u8 buf[24]`). Si el residual es solo de registros/stack, ir directo a permutadores:
  1. `python3 tools/match/declperm.py <lote.c> <func>` (orden de declaraciones locales; GCC 2.7.2 asigna registros en parte por ese orden).
  2. `tools/match/dperm.py` (decomp-permuter) para residuales pequeños; limpiar su salida y dejar como `TODO(match-debt)` los trucos sin alternativa legible.
* **Prioridad 2 — bytes grandes con REA:** `Ai_Step` (13 KB) y `Coll_BoxCornersVsWorld` / `Coll_PointsVsWorld` / `Coll_ProbeWheels` / `Coll_SyncObjects` (~17 KB): usar `--xrefs` para recuperar los structs compartidos en `include/*.h` antes de escribir C; trabajar una a la vez.
* **Prioridad 3:** resto de `INCLUDE_ASM` ordenadas por **bytes recuperables × probabilidad de match** (`python3 tools/inventory.py`).
* **Presupuesto intra-sesión:** máximo **5–6 intentos sin reducir el diff** (o sin hipótesis nueva comprobable). Al agotarlo, congelar/actualizar la función bajo `#ifdef NON_MATCHING` con `TODO(match-debt)` + diff residual y pasar a la siguiente función.
* **Límite entre sesiones:** máximo **2 sesiones** por función sin progreso medible (salvo hipótesis nueva o nueva pasada con REA/Ghidra).
* **Reglas estrictas:**
  * Nunca descartar un borrador `NON_MATCHING`; se conserva bajo `#ifdef NON_MATCHING` / `#else INCLUDE_ASM(...) #endif` y **no cuenta como ByteMatch (0 B oficiales)**.
  * Prohibido para igualar: `asm()`, nops/`.space`, stubs o tocar la ROM (`tools/check_no_asm.py`).

### 4. Verificación (rápida en bucle, completa solo al cierre)
* **Durante la iteración:** usar solo comparación rápida (`tools/match/try.py` o compilación aislada del `.o`). No correr `make`, `verify_funcs.py` ni `objdiff_report.py` en cada variante.
* **Al integrar un `MATCH exacto` en `src/`:** ejecutar `make` incremental para comprobar compilación y enlace.
* **Una sola vez al final de la sesión** (o antes de preparar commit):
  1. `make` incremental y verificar SHA-1 de la ROM (`94913a07e49005ffd43dfdfa16ff5862bdb93748`).
  2. `python3 tools/match/verify_funcs.py` y `python3 tools/check_no_asm.py`.
  3. `python3 tools/objdiff_report.py` y comprobar que `report/us/report.json` coincide con `build/us/report.json`.
* **Compilaciones completas (`make clean`):** solo las hace el usuario. El agente nunca ejecuta `make clean` ni `make clean && make`.

### 5. Integración y Commits
* Al lograr coincidencia exacta (100 %), reemplazar `INCLUDE_ASM` en su archivo `src/unit_*.c`.
* **Política de commits agrupados (recta final >90 %):** objetivo de **~10 funciones con MATCH exacto** por commit (o cierre de una unidad grande / +5–10 KB). Las funciones `NON_MATCHING` no cuentan.
* Antes de preparar commit: revisar `git status`/`git diff --stat` y **preservar cualquier WIP ajeno** (incluidos los `TODO` del usuario en `src/`): nunca revertir ni mezclar cambios no relacionados sin avisar.
* Usar `./tools/match/commit_unit.sh [--commit] [--force] "<mensaje>" <archivos>`:
  * Sin `--commit` (por defecto): verifica (`make` incremental, SHA-1, `verify_funcs.py`, `objdiff_report.py`), cuenta funciones ganadas respecto a `HEAD`, lista archivos y **se detiene**.
  * El usuario revisa, hace su `make clean && make` y solo entonces se ejecuta con `--commit` (`--force` salta el mínimo de funciones o el intervalo de 5 min).

### 6. Registro Conciso (al cierre de la sesión)
* **`MATCH_LOG.md`:** entrada breve por función (3–5 líneas): resultado (`MATCH exacto` | `NON_MATCHING x/n`), **línea `REA:` obligatoria** con lo concreto que aportó (tipo, offset, stride, xref o estructura; o "sin aporte: residual de registros"), hipótesis ganadora o causa del residual, y **Bytes recuperados** (`0 B` si `NON_MATCHING`). Una entrada sin línea `REA:` indica que no se consultó.
* **`SESSION.md` y `METRICS.md`:** actualizar con el resumen de la sesión y el porcentaje oficial de `objdiff` (única fuente de porcentajes).
* **`tools/match/PATTERNS.md`:** actualizar solo si aparece un patrón reutilizable nuevo de GCC 2.7.2 o un casi-match relevante.
