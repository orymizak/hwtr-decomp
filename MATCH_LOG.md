# Per-function match log

One entry per function worked on: size, initial match, final result, hypotheses tried, and why it matched or not.
Failed reconstructions keep their lessons here; decide whether they belong in `NON_MATCHING`.
Percentages of the whole project follow METRICS.md (objdiff only).

## Car_ComputeMass (unit_31E10, 752 B / 188 instr, frame 0x0) — MATCHED 2026-11-24
- **Initial match:** previous draft `.scratch/ul/nr/near_95970.c` (70/202 per PATTERNS.md; 102/188 in this session).
- **Final:** 188/188 words, `make clean && make` OK, `verify_funcs.py` all functions OK. objdiff +752 B.
- **Hypotheses**
  1. Stock draft with a private `Car95_` struct: pointer register in `$t0` instead of `$a3`, wrong p.z/abs/loop-tail
     order. Declaration permutation (declperm, 120 orders): no match.
  2. Index-based loop (`c->whl[i]`), `register c`, `n` as `u8`: worse (78-101/188).
  3. `w->p.z -= (f & 1) ? c->f524 : c->f50C;` as a ternary instead of if/else: **fixed** the `$a3` allocation
     (p.z load happens before the branch) — keep.
  4. Loop header `w++, i++` instead of `i++, w++`: **fixed** the counter increment position in the tail — keep.
  5. Absolute value: ternary, `if/else`, inline `absf_`, `y = x; if (x<0) y = -x;` with a separate `x`:
     all coalesce `y` and `x` into one register (`bc1tl neg.s f0,f0`), the original has
     `bc1f; mov.s f2,f0; neg.s f2,f0`. `y = -x; if (x >= 0) y = x;` gave the right shape but swapped neg/mov.
  6. **Reusing the same variable** for the difference and the final doubled/max value
     (`d = pos.x - cx; y = d; if (d < 0) y = -d; d = y + y; ...; extent.x = d;`) **matched**.
- **Why it matched:** GCC 2.7.2's allocator only keeps `y` and `x` apart when `x`'s pseudo is still live after
  the `neg` (it is redefined later by `d = y + y`); a fresh temporary lets the `mov` coalesce away.
- **Header changes (offsets unchanged):** `include/car.h` now names `wheelCount`, `centerRaw`, `extentRaw`,
  `wheelPosRaw[6]`, `wheelWRaw[6]`, `wheelHRaw[6]`, `nFront`, `nRear`, `extent` (Vec3f), `f5F4/f5F8/f5FC`.
- **Not a NON_MATCHING candidate;** no match-debt tricks used. The old PATTERNS.md entry for Car_ComputeMass
  (70/202) is now stale.

## Gfx_UpdateLights (unit_103B0, 676 B / 169 instr, frame 0x28) — NON_MATCHING 146/169 (objdiff 86.36 % fuzzy), 2026-11-24
- **Contexto:** prueba de integración REA/Ghidra (ver `tools/rea/README.md`). Sin match → queda como `#ifdef NON_MATCHING`
  con `INCLUDE_ASM` en la ruta normal; ROM SHA-1 intacto; objdiff oficial sin cambio de bytes.
- **Hallazgos REA/Ghidra útiles:** (1) pseudocódigo con estructura correcta (bucle de copia 7×16+8 B, pools, bucle de
  luces); (2) `0xDC08000A`/`0xDB020000` → `gSPLight(.., n)` y `gSPNumLights` de F3DEX2 (decodificación manual); (3) xrefs de
  `gLightsDirty` (24 sitios) y callers (Model_Draw, Light_SetCount, Gfx_BeginScene, World_DrawSlot); (4) callees
  (MtxPool_Grow, GfxPool_Grow) una vez retirados los alias `.NON_MATCHING` del ELF.
- **Hipótesis**
  1. Global directo `gNumLights` en cada uso: 113/169 (gcc recarga lui/lbu, sin `$s1`).
  2. `n = &gNumLights` tras la copia + `GFX_RESERVE(*n + 2)`, bucle `i < *n`: **146/169** (mejor).
  3. `volatile u8 *n`: 146/169, mismo diff, más tamaño; descartado (truco sin motivo).
  4. Permutación de declaraciones (6 órdenes × volatile/no): sin cambio.
  5. Cola con `gNumLights` directo (como parece el original): 137/169, peor.
  6. `lights + (i*16 + 8)`: 141/169, peor.
- **Diff residual:** el original deja `lights` en `$s0` y `&gNumLights` en `$s1` (nosotros al revés), copia `$s1` a `$a3` para el
  test del bucle y recalcula `&gNumLights` en la cola. Causa probable: otra forma de la expresión del bucle/prólogo en la fuente.
- **Pendiente:** retomar con hipótesis nueva (p. ej. `u8 *` distinto para el bucle, o macro GFX_RESERVE con n evaluado distinto).

## Sesión 39 (2026-11-24) — grupos relacionados con REA/Ghidra; sin nuevos matches exactos, 4 reconstrucciones NON_MATCHING
Método: por grupo, `procedure_pseudo_code` + callers/callees (REA) como esqueleto; m2c no está instalado, así que el pseudocódigo de
Ghidra sustituyó a `show.py`. Medición: `hand.py` (palabras), `tryin.py` (dentro de la unidad, con registros reales) y tres
utilidades nuevas en `.scratch/s39/` (`skel.py` = esqueleto de mnemónicos; `dis.py` = objdump del lote; `rperm.py` = permutación aleatoria de declaraciones).
**Resultado objdiff: sin cambio (74.97 %).** ROM SHA-1 intacta (`make` → OK). Las cuatro funciones quedan en `#ifdef NON_MATCHING`.

### Controls_SetupTexts (unit_53B30, 912 B) — NON_MATCHING 224/228 palabras
- REA: pseudocódigo dio la estructura (dos bucles P1/P2 + dos remaps); `Text_ButtonGlyphs` y `sysInputGetKeyName` ya existían.
- Hipótesis: (1) punteros locales `p`/`tab`: 154/234 (gcc los hoistea antes del bucle; el original no); (2) `D_800F5F4C[]`/`D_800F5F50[]` como
  arrays y sin locales: 193/228; (3) `j++, i++` en ese orden en la cabecera: 222/228; (4) `j = 0; y = 173;` antes del bucle P2: 224/228.
- Residual: gcc carga `D_8012410B/C` (a1) antes de `D_8014126C` (a2) en las dos llamadas de remap; el original carga a0, a2, a1.
  Probado sin efecto: `50 * n`, cast `(u8)`, local `n`, local `t`, asignación dentro de la llamada, `char *` extern, `volatile`.
- Lección: `gControlsRemapP1/P2` se leen con `lbu` → `*(u8 *)&` (la unidad los declara `s8`).

### HighScores_SetupTexts (unit_53B30, 1080 B) — NON_MATCHING 209/270
- REA: pseudocódigo claro; el despacho por categoría es un **`switch`** (árbol `bltz`/`slti`; un `if (x < 2)` sobre `u8` genera `sltiu`).
  `case 0: case 1: …; case 2: default: …` reproduce `bltz → slti → beqz` (el `case 2:` explícito es necesario).
- `sel` es `u32` (sin `andi` en `sll`/`multu`); `fmt = &D_80100CA0` como local asignado justo antes del primer `sprintf` (sibling RaceWin).
- Residual: gcc hoistea `&buf[1]` a un registro y reparte `sel`/`i` en otros registros ($s7/$s3 en vez de $s4/$s2). `declperm` (8! órdenes) sin match.

### Garage_SetupTexts (unit_53B30, 1388 B) — NO guardada (143/347)
- REA: dos bloques P1/P2 casi idénticos; el tercer `Menu_SetText` es una cola común tras `goto` (`LAB_800bed78`).
- Pendiente por estructura: el original asigna `p = &D_800F5F40` *dentro* de cada camino y mantiene `100` como `u8` hoisted en `$s5`
  (`andi $s1,$s5,0xFF`) solo en el camino "dirty". Plantilla en `.scratch/s39/gs8.c`. Sin hipótesis nueva no seguir.

### Password_Encode (unit_50510, 800 B) — NON_MATCHING 782/800 palabras (18 distintas, dentro de la unidad)
- REA dio el algoritmo completo (máscara de desbloqueos 24 bits + 11 bytes + empaquetado 5 bits + checksum + permutación de 6 pares).
- **Hallazgo clave:** `Unlocks_GetMask` es `inline` (GNU89: se emite fuera de línea *y* se expande) → la máscara vive en la pila (`sw/lw 0x10($sp)`).
  Pasar `Unlocks_GetMask(p, &mask)` con la función ya definida y marcada `inline` reproduce el prólogo; `p` queda en `$a0`.
- `% 32` por rama (dentro de cada `if`) reproduce `bgez/addiu 31/andi 0x3E0`; con la variable compartida sale `sra/sll`.
- `((u8)(b23 << 4) & 0x80)` da `andi 0x80` (con `& 0x80` a secas gcc usa `-128` en registro o `0x3F80`).
- Bucle de permutación: con `i * 2` y una sola tabla gcc hoistea las direcciones; con `j2 = i * 2` (s32) y dos símbolos
  (`D_800F601C`/`D_800F601D`) no. Pasó de 60 a 18 palabras distintas.
- Residual: `bi` y `r` intercambian `$a2/$a3`; orden de la sentencia `out[k] = …`. Probado sin efecto: orden de declaración (4!),
  orden de inicializaciones (3!), inicializaciones al principio, temporal `c`.

### Password_Decode (unit_50510, 1304 B) — NON_MATCHING 239/328
- Mismo `inline` para `Unlocks_SetMask` (dos expansiones: tabla de trucos y máscara decodificada). Copia de 20 B = struct `Cdt20_`
  (lwl/lwr/swl/swr). Tabla de trucos `{char *name; u32 mask; u32 w1C}` (7 × 12 B) en `D_800F5FC8`.
- Residual: asignación de registros ($s1 en el original) y forma del bucle de búsqueda de carácter (el original rota el bucle con
  `andi` en el delay slot del `bnel`); mismo truco `j2` para la permutación.

### Casi-matches revisados (sin cambio)
- `SceneNode_DrawMesh` (5 palabras): estructura correcta con `if (mat != NULL) {…; lastId = id;} else {lastId = id;}` (da el `beqzl` con la
  tienda en el delay slot). Solo difieren registros: original p3C=$v0/mat=$a0; nosotros $v1/$v0. Probado: local `mt`, `me`, `(mt = …)`, if/else
  invertido (peor, 36). Sin hipótesis nueva no seguir.
- `SndCmd_SetEnvelopePreset` (5 palabras): el original deja `e` como copia (`move $a3,$v0` en el delay slot del `bnez`) y r en `$a2`;
  probado reordenar `r`/`e` y expresiones: gcc siempre fusiona y asigna `e` directamente a `$a3`.

### Límites del análisis REA documentados
Ver `tools/rea/README.md` (sección «Verificación de prepare_elf.py» y «Limitaciones»): `prepare_elf.py` ahora verifica símbolos,
direcciones, cabeceras de sección y los 206 contenidos PROGBITS; el ELF no tiene relocaciones, así que las referencias son absolutas.

### Sesión 40: Model_SetWheel (unit_06050, 140 B) — sin match
- Firma deducida: (u8 slot, u8 w, f32 rx, f32 ry, f32 rz, s32 unused, u8 surface, u8 contact, Vec3f *pos, Vec3f *n); llama a Model_SetWheelContact y luego Model_SetPartRotation. Lógica correcta.
- Residual: registros. Original s0=slot, s1=w, f20=rz, f22=rx, f24=ry; nosotros s1=slot, s0=w, f24=rz, f20=rx, f22=ry.
- Probado: 325 combinaciones de locales (s, ww, x, y, z en todos los órdenes/subconjuntos) sin efecto. Fuente devuelta a INCLUDE_ASM.
