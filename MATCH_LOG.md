# Per-function match log

One entry per function worked on: size, initial match, final result, hypotheses tried, and why it matched or not.
Failed reconstructions keep their lessons here; decide whether they belong in `NON_MATCHING`.
Percentages of the whole project follow METRICS.md (objdiff only).

**Plantilla por función (AGENTS.md §5):**
```
## <Func> (<unit>, <B> B / <n> instr, frame 0x..) — MATCH exacto | NON_MATCHING <x>/<n> — <fecha>
- **REA/Ghidra:** consultas usadas (pseudocódigo, callers/callees, xrefs, hermanas) y qué aportaron; consultas omitidas + justificación técnica.
- **Confirmado en ASM:** qué se verificó contra el .s (y qué del pseudocódigo era engañoso).
- **Hipótesis:** numeradas, con la instrucción concreta que debía cambiar y el efecto medido.
- **Cambios:** lo que quedó en src/ / include/.
- **Resultado objdiff:** match exacto (cuenta) | NON_MATCHING (no cuenta) + % de objdiff del proyecto si se midió.
- **Bytes recuperados:** B oficiales (0 si NON_MATCHING).
```
Las entradas anteriores a la sesión 42 usan un formato libre; se conservan tal cual.

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

## Ai_ApplyImpulse (unit_22440, 1088 B / 272 instr, frame 0x40) — MATCHED 2026-11-24
- **Contexto:** Evaluación e integración de REA 6.4.0 + Ghidra 12.1.4 (`tools/rea/prepare_elf.py`, `tools/rea/rea_query.py`).
- **Initial match:** borrador `.scratch/ul/nr/near_84B9C.c` (250/272 palabras, tamaño 1072/1088 B).
- **Final:** 272/272 palabras (100% ByteMatch), `make` OK (SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`), `verify_funcs.py` OK, objdiff +1088 B (75.17% code, 1777/1910 functions; `inventory.py`: 75.20%, 417064/554576 B).
- **Hallazgos REA/Ghidra + contraste con ASM:**
  1. `procedure_pseudo_code` mostró los tres productos escalares (`impulse` contra las 3 filas de `car->rot`), el stride `0x2A8` sobre `gAi[*param_1]` y los accesos a `D_80114A84` (`steer`), `D_80114A80` (`throttle`), `D_80114A7C` (`ctlTimer = 2`) y `D_80114A38` (`braking = 1`).
  2. `procedure_callers` identificó al único llamador directo (`0x80084fdc` = `Ai_SyncCars`), y `xrefs` sobre `0x80114a80`, `0x80114a84` y `0x80114a7c` localizó todos los accesos en `Ai_ApplyImpulse`, `Ai_InitRacer` (`0x80086140`) y `Ai_RestoreSlot` (`0x80086b40`).
  3. Contrastando el pseudocódigo con el ASM original: en el borrador previo `gAi[o[0]].ctlTimer = 2` estaba fuera de los `if/else` de `0.0f < x` y `0.0f < z`, lo que hacía que GCC 2.7.2 pusiera `ctlTimer = 2` en el mismo bloque básico que `if (0.0f < y)`, hoisteando `li $a2, 2` y adelantando `mtc1 $zero, $fv0; c.lt.s $fv0, $ft2`. Al colocar `gAi[o[0]].ctlTimer = 2` dentro de cada rama `if/else` (igual que en `0.0f < y`), el diff bajó de 22 instrucciones a solo 2 (`270/272`, tamaño exacto 1088/1088 B).
  4. Las 2 diferencias restantes eran el orden de `sll $a1, $v0, 3` frente a la carga de `D_80138D4E`/`D_80138D50` en las dos ramas de resta de `throttle`: cambiar `((f32)(D_80138D4C - 100) < gAi[o[0]].throttle - D_80138D4E)` por `(gAi[o[0]].throttle - D_80138D4E > (f32)(D_80138D4C - 100))` evaluó primero el operando izquierdo (`gAi[o[0]].throttle - ...`) igual que en las ramas de suma, logrando **272/272 (COINCIDE 0x440)**.

## Sesión 40 (2026-11-24) — Hud_DrawSlideFromLeft/Right y Save_ReadAll: 3 reconstrucciones NON_MATCHING
Sin matches exactos nuevos; ROM OK (`#ifdef NON_MATCHING` con `INCLUDE_ASM` en el `#else`). Borradores en `.scratch/s40/`.
- **Hud_DrawSlideFromLeft/Right (unit_49CF0)**: mismo tamaño (452/440 B) y mismas instrucciones salvo el intercambio $s1/$s2 (cadena ↔ elemento).
  Claves halladas: `u8 pad[8]` (8 B extra de frame), `s32 w` unido tras if/else (la suma va después del join), `e->gap` (0xE), `-(e->w >> 1) + mid`
  en la rama central, `(s16)e->x` en Left. Sin efecto en el swap: `register`, copias `s0/e0`, orden de declaración, do-while, `while (*s)`, `p = s`.
- **Save_ReadAll (unit_50510)**: 772/776 B. Dos variables de bucle (i para el principal, j para los bucles de borrado y de slots).
  Residual: el original hoistea `&sysSaveRead` en $fp (usos [0] y [-1] = sysPakGetState) y deja `&gSaveSlots` sin hoistear; nosotros al revés.
  Sin efecto: `fn = sysSaveRead`, `register`, forma por offsets en bytes, flag `r` en la cola (el original usa bnel/beql con v1).
- Candidatas siguientes: Race_ReadPlayerControls (844 B, float), CupResults_SetupTexts.

## Sesión 41 (2026-11-24) — REA + Ghidra como método activo
Flujo por función: `procedure_pseudo_code` + `procedure_callers/callees` + `xrefs` + `procedure_assembly` (REA) → contraste con el `.s` MIPS → C → `try.py`/comparación por palabras → `make` (SHA-1) + `verify_funcs.py`.
- **LZ_AdvanceBits (src/lz.c, 168 B) — MATCH 42/42.** El pseudocódigo de Ghidra tipó `param_1` como `char` (u8); el TODO previo asumía `int n`. Con `register u8 n, register u8 *p` (-O0) coincide. REA además dio el único llamador (`LZ_Decompress`) y todas las xrefs de `D_8010C400`; `procedure_assembly` del llamador confirmó `a0 = n`, `a1 = p`.
- **Model_SwapPalette (unit_06050, 324 B) — NON_MATCHING 72/81.** Ghidra dio estructura (`Model.palIdx`, `pals[].colors + 8`, 7 colores, darken >>1 / >>3). Hallazgos aplicados contra el ASM: indexado `dst[k]` (genera el puntero de bucle `$t4`), `dst += 4` separado (delay slot `addiu`), temporales `u32 hi/mid/lo` y `lo | (hi | mid)`. Residual: orden de 3 `sll` en el empaquetado.
- **SndCmd_SetEnvelopePreset (unit_6E610, 332 B) — NON_MATCHING 79/83.** Ghidra: varint (0x80), tabla de 7 bytes en `unk74->unk18`, mismo cuerpo que `SndCmd_SetEnvelope` (ya igualada). `id &= 0x7F; id <<= 8; id |= *p++` quitó 4 diferencias. Residual: el original copia el puntero de la entrada en `$a3` (delay slot de `bnez`).
- **PauseUi_DrawText (unit_6AE90, 352 B) — NON_MATCHING ~71/88.** Ghidra: dos recorridos (medir/dibujar), centrado con `(s16)w`, kerning solo si `s[1] != 0`. Una copia local de `cx` y sumas `kern + t` fijaron la asignación de registros ($s4/$fp/$s5-$s7). Residual: cabecera de bucle (`lbu $v0,0($s3)`) y movimientos del prólogo.
- ROM intacta: `make` OK (SHA-1) y `verify_funcs.py` OK. Único match nuevo: LZ_AdvanceBits; las otras tres quedan como NON_MATCHING con `INCLUDE_ASM` en el `#else`.

## Sesión 42 (2026-11-24)
- **Race_ReadPlayerControls (unit_2C6A0, 844 B) — NON_MATCHING 194/211.** Firma K&R (`u8 pl` → s32), `sysInputGetControl` como array (dirección en registro), `D_80138D40` como array (evita mover los stores de `yaw` al delay slot), clamp `if/else if/else if` con tercera comprobación `t<0`, car.h: `unk01C/unk020` (f32). Residual: 1.0 cae en `$f6` en vez de `$f0` y el 2º salto no es branch-likely. Permuter 7 min: mejor 550 (base 620), sin match. Borradores en .scratch/s42/.

## Sesión 43 (2026-11-24)
- **RaceAudio_Update (unit_2C6A0, 1400 B) — borrador 317/350 palabras, tamaño exacto 1400/1400, NO aplicado** (`.scratch/s43/ra9.c`, previo `.scratch/ul/s36/auto/RaceAudio_Update.c` ~258). REA/Ghidra dieron la estructura (do-while con guardia `D_801184C1 != 0`, clamp, suavizado de rpm, bucle de marcadores `puVar4/puVar6`).
  Hallazgos que subieron 258 → 317: tabla de punteros `audCarSndSetRpm[]` con índices negativos (-5 pos, -4 vel, -3 reverb; +1 throttle, +2 StartLoopA, +3 StopLoopA) para que GCC hoistee `s4/s6`; todos los args de audio como `s32` (los floats/vectores se pasan como palabras: `*(s32 *)&D_801184C8[n].vol`, `s32 pos[4]`/`vel`/`up`/`fwd` de 16 B); `n = i` (u32) como índice del primer bloque; `MIN(MAX(vol,1000),6000)`; `u32 *tm = &D_80118534`; `snd = D_800F3708[kind][(b < a) ? 0 : 1]`; timer con `if (t <= dt) {StopLoopB(); t = 0;} else t -= dt`; StopLoopA/StopLoopB del else/timer por símbolo propio.
  Residual: falta el hoist de `s7 = s4 + 0x10` (SetLoopAVolume), orden de carga de `a/b` antes de `c.lt.s`, `andi/sll` de `m` en el bloque del timer, orden de los args de pila de `audListenerSetOrientation`, y por ello un registro guardado menos (s7). Ni `fv = &audCarSndSetRpm[4]` ni array propio lo arreglan.
- SndCmd_SetEnvelopePreset: sin cambios (ya probadas var_a..var_p6 en s41).

## Sesión 44 (2026-11-25) — 3 nuevos MATCH exactos (+3,696 B, 2 unidades completas) y 1 NON_MATCHING

## Bld_StepRacer (unit_21150, 1892 B / 473 instr, frame 0xF8) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code` + `procedure_callers`/`procedure_callees` sobre `Bld_StepRacer`; mostró la lectura de registros de pista con `Bld_ReadRecord`, el switch de 6 casos (`jtbl_800FFA50`) para transiciones de waypoints, y la actualización de `a0/b0` y `a1/b1` según `rec.a.kind == 0`.
- **Confirmado en ASM:** Ghidra fusionaba la cola del `if (rec.a.kind == 0)` en un único bloque antes de `a->ang = 0.0f; if (flag)`. En el `.s` original (`0x80080EA8..0x80080EE0`), las asignaciones `a->a1 = rec.a.f30; a->b1 = rec.a.f34;` están en la etiqueta `.L80080EC8` antes de `if (flag)` (`beqz $s1, .L80080F08`), con `sw $zero, 0x14($s0)` (`a->ang = 0.0f`) colocado en el delay slot del `beqz`. Además, en la rama `rec.a.kind == 0`, los stores van en orden `a->a0 = a->a1; a->b0 = a->b1;` (`0x6C/0x70` antes de `0x74/0x78`), diferencia que `try.py` ocultaba por el enmascarado de `RELOC_OPS` en `lwc1`/`swc1`.
- **Hipótesis:**
  1. Borrador previo `.scratch/ul/nr/near_80C20.c` (471/473 palabras): colocar `a->ang = 0.0f; a->a1 = rec.a.f30; a->b1 = rec.a.f34;` juntos tras el `if/else` hacía que el scheduler de GCC 2.7.2 intercalara `sw $zero, 0x14($s0)` entre `a1` y `b1`, metiendo `swc1 $f0, 0x78($s0)` en el delay slot de `beqz $s1`.
  2. Duplicar `a->a1 = rec.a.f30; a->b1 = rec.a.f34;` en un segundo `if (rec.a.kind == 0) { ... } else { ... }` justo antes de `a->ang = 0.0f; if (flag)`: GCC aplica cross-jumping a `.L28` antes de `a->ang = 0.0f`, aislando `a->ang = 0.0f; if (flag)` para que `sw $zero, 0x14($s0)` ocupe el delay slot de `beqz` → **473/473 palabras**.
  3. Reordenar `a->a0 = a->a1; a->b0 = a->b1;` en la rama `rec.a.kind == 0` y añadir `Bld_StepRacer jtbl_800FFA50` en `rodata_refs.txt`: corrigió las 4 palabras en `0x80080EAC..0x80080EBC` en el enlace final de la ROM.
- **Cambios:** `src/unit_21150.c` (`Bld_StepRacer` en C, unidad `unit_21150` 100% completa), `rodata_refs.txt`.
- **Resultado objdiff:** MATCH exacto (473/473), 78.35 % del proyecto tras integrarla.
- **Bytes recuperados:** 1,892 B.

## HighScores_SetupTexts (unit_53B30, 1080 B / 270 instr, frame 0x98) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code` + `xrefs` de `0x800F5F80` (`D_800F5F80`) y revisión de funciones hermanas en `src/unit_53B30.c` (`HighScores_Enter`, `HighScores_Draw`).
- **Confirmado en ASM:** En `.L800C4D10`, el ASM original calcula `lui $v0, %hi(gMenuObjs+120); addiu $v0, $v0, %lo(gMenuObjs+120); lw $a0, 0($v0); move $s7, $v0` en la primera llamada a `Menu_SetText`, y reutiliza `lw $a0, 0($s7)` en las siguientes. `0x800F5F80` es exactamente `gMenuObjs[30]` (`0x800F5F08 + 30 * 4`). Además, el `switch (D_80141226)` requiere `case 2:` explícito junto a `default:` para emitir el árbol `bltz / slti / beqz`, y `y = 0xA0;` va justo después de `Menu_SetText(gMenuObjs[30], 1, ...)` y antes del bucle `for (i = 0; i < 5; i++)`.
- **Hipótesis:**
  1. Reemplazar el puntero local `p = &D_800F5F80` por el acceso directo a array `gMenuObjs[30]` en todas las llamadas a `Menu_SetText`: GCC 2.7.2 CSEa la dirección `&gMenuObjs[30]` en `$s7` tras el primer `lw $a0, 0($v0)`, eliminando las 4 palabras de diferencia de `.scratch/ul/nr/near_C4D10.c` (266/270 → **270/270**).
- **Cambios:** `src/unit_53B30.c` (promovida de `#ifdef NON_MATCHING` a código C activo).
- **Resultado objdiff:** MATCH exacto (270/270), 78.54 % del proyecto tras integrarla.
- **Bytes recuperados:** 1,080 B.

## RaceView_DrawScene (unit_4F640, 724 B / 181 instr, frame 0xB0) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code` + `procedure_callees` (`svcCameraSetPerspective`, `svcModelSetAlpha`, `svcScreenSetViewport`, `World_Draw`, `Cars_Draw`, `Particles_Draw`).
- **Confirmado en ASM:**
  1. La llamada a `svcCameraSetPerspective` está desdoblada en un `if (gRaceViewCount == 1) ... else ...`, cargando el 4.º argumento float (`1.3333334f` o `2.6666667f`) en `$a3` con `lui/ori` en cada rama.
  2. El cálculo de alpha (`((car->timer == 0 ? 0xFF : 0) & 0xFF) | 0x7F`) pasa un `s32` a `svcModelSetAlpha(u8, s32)` sin `andi $a1, 0xFF` extra.
  3. En el bloque de pantalla dividida (`if ((u32)gRaceViewCount >= 2)`), antes del bucle o invariante al mismo se calculan `$fp = (h >> 1) + 1` y `$s7 = (h >> 1) - 1`, y el segundo argumento de `svcScreenSetViewport` usa `-(i == 0) & t` (`andi / sltiu / negu / and / sll / sra`).
- **Hipótesis:**
  1. Combinar el `if/else` de `svcCameraSetPerspective` y el cálculo de `alpha` de `.scratch/ul/nr/near_AEB28.c` con las variables locales `t = h / 2 + 1; b2 = h / 2 - 1; mask = i == 0; vh = b2; svcScreenSetViewport(0, -mask & t, w, vh);` de `.scratch/ul/s37/rv_lote.c`: GCC hoistea `t` a `$fp` y `b2` a `$s7` en el orden exacto del prólogo del bucle → **181/181 palabras**.
- **Cambios:** `src/unit_4F640.c` (`RaceView_DrawScene` en C, unidad `unit_4F640` 100% completa).
- **Resultado objdiff:** MATCH exacto (181/181), 78.67 % del proyecto (434,864 / 552,760 B, 1,791 / 1,910 funciones).
- **Bytes recuperados:** 724 B.

## Coll_LoadLevel (unit_39C90, 636 B / 159 instr, frame 0x138) — NON_MATCHING 155/159 — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code` + `procedure_callees` (`sprintf`, `File_Load`, `Memory_Alloc`, `Coll_RelocateTree`, `Coll_InitRooms`, `Coll_BuildRoomBounds`, `Coll_InitCells`).
- **Confirmado en ASM:** La función formatea `"%s.bvh"`, carga el archivo en `gCollLevel`, reserva las tablas de rooms/cells y llama a las rutinas de inicialización. En `0x80099110`, el ASM original recarga `gCollLevel` dos veces seguidas (`lw $v0, %lo(gCollLevel)($s2); lw $a1, %lo(gCollLevel)($s2)`).
- **Hipótesis:**
  1. Reconstrucción limpia a partir de `.scratch/ul/nr/near_98FE8.c`: **155/159 palabras** (628/636 B); la única diferencia es la doble carga redundante de `gCollLevel` en `0x80099110` y 1 instrucción de orden en los argumentos del bucle final.
- **Cambios:** Guardada bajo `#ifdef NON_MATCHING` con `INCLUDE_ASM` en el `#else` en `src/unit_39C90.c`.
- **Resultado objdiff:** NON_MATCHING (no cuenta en objdiff).
- **Bytes recuperados:** 0 B.

# Session 45 (2026-11-25) — investigation only, no exact match integrated

Nothing was promoted into `src/` this session: every candidate is still a near miss, so objdiff stays at
**78.67 %** (434,864 / 552,760 B, 1,791 / 1,910 functions). All drafts are kept in `.scratch/s45/`.

## Tooling fix: `tools/match/try.py` printed the wrong `+` lines
- **Confirmado en ASM:** the `+` side of every diff was taken from `objdump`'s *line index* instead of the
  instruction address, so from the first `nop` onward our instructions were reported shifted (e.g. a `nop`
  shown as `bc1f`). The masked word comparison and the `iguales N/M` count were always correct; only the
  printed mnemonics were wrong, which had been sending earlier sessions after phantom differences.
- **Cambios:** `tools/match/try.py` now maps `objdump` output by `address / 4`. A side-by-side variant of the
  same diff lives in `.scratch/s45/sbs.py`.

## Garage_SetupTexts (unit_53B30, 1388 B / 347 instr, frame 0x50) — NON_MATCHING 341/347
- **REA/Ghidra:** `procedure_pseudo_code` gave the whole shape: per-player `ready` / `dirty` branches, the
  `prev` / `cur` / `next` wrap-around over 41 cars, three `Car_IsUnlocked` probes and the shared third
  `Menu_SetText` reached by `goto LAB_800bed78` (GCC cross-jumping).
- **Confirmado en ASM:** (1) `gGarageDirtyP1/P2` are read with `lbu`, so they are `u8`, not `s8`.
  (2) `$s5 = 0x64` is hoisted into the prologue and each use is `andi $s1, $s5, 0xFF` — the greyed-out colour
  is a local `u8 dim = 100;`, not a literal. (3) `addu $s0, $v1, $zero` appears at the end of *both* arms,
  so `cur` is assigned inside each arm; that 2-instruction tail is what lets GCC cross-jump `cur = i;
  next = cur + 1;` into the shared block at `.L800BEC38`. (4) the inner test is written
  `if (i + 1 >= 0x29) next = 0; else next = cur + 1;` — the `>=` form puts the `next = 0` block in the
  fall-through and makes the branch `bnez` on the `slti` result, as in the ROM.
- **Hipótesis probadas:** nested vs flat `if/else` (294 → 314), `u8 dim = 100` (320), `cur = i` in all three
  arms (329, exact size), inverted inner test (334), plus 120 permutations of the declaration block and
  `register` qualifiers (no change). decomp-permuter then found a copy (`new_var = i` in the P2 half) that
  fixes the `i` / `i & 0xFF` register swap: **341/347**.
- **Residual:** 6 words. `prev` must live in `$v0` (shared with the `i - 1` / `i + 1` / `slti` temporaries);
  GCC gives it `$a2`. `s32 prev`, `register u8 prev` and moving its declaration do not change it.
- **Bytes recuperados:** 0 B. Draft: `.scratch/s45/garage_setuptexts_341.c`.

## GfxPools_Init (gfx_pools, 416 B / 104 instr, frame 0) — NON_MATCHING 82/104
- **REA/Ghidra:** `procedure_pseudo_code` listed all 40 stores; mapping them onto `GfxPool`/`VtxPool`/
  `MtxPool` (0x320 / 0x90 / 0x90 bytes) shows the function is exactly six inlined `*Pool_Init` calls plus
  `gGfxPoolIndex = 0` and the three head pointers.
- **Confirmado en ASM:** the store order (`end`, `chunks[0]`, `ptr`, `capacity`, …, with every
  `chunks[1] = NULL` / `chunkCount = 1` grouped at the end) is what the existing `inline` helpers already
  produce — those helpers are real ROM functions (`GfxPool_Init = 0x80071B50`, `VtxPool_Init = 0x80071D44`,
  `MtxPool_Init = 0x80071EBC`) and already match, so their bodies are fixed. `0x300`/`0x40`/`0x80` share one
  register (`$a0`) and the constant `1` lives in `$a2`.
- **Hipótesis probadas:** `&gGfxPools[n]` vs `gGfxPools + n`, `D_80134FD0[n]` vs `&D_80134FD0[n][0]`,
  `gGfxPools[gGfxPoolIndex]` in the tail, and a `for (i = 0; i < 2; i++)` loop (11/104, wrong size) all stay
  at 53/104 with the exact size. decomp-permuter found that moving `gGfxPoolIndex = 0;` *between the two
  `GfxPool_Init` calls* jumps to **82/104**; all seven positions for that statement were then measured
  (0: 49, 1: 82, 2-5: 72-73, 6: 53).
- **Residual:** 22 words, all in the Mtx half plus the constant `1` landing in `$a1` instead of `$a2`.
- **Bytes recuperados:** 0 B. Draft: `.scratch/s45/gfxpools_init_82.c`.
- **TODO(match-debt):** the `gGfxPoolIndex = 0;` position that scores best is not plausible source; keep it
  as a measurement, not as the answer.

## CupResults_SetupTexts (unit_53B30, 1516 B / 379 instr, frame 0xB8) — NON_MATCHING 306/380
- **REA/Ghidra:** `procedure_pseudo_code` gave both copies of the row loop (3 rows + 3 blanks when
  `gProfileP1->cup == 2`, else 6 rows) and the trailing `Menu_SetText(..., 0x12, D_80141500, 200, 200, ...)`.
- **Confirmado en ASM:** (1) `D_80123F8A` is `D_80123F60[i].f2A` (stride 0x2C) and `D_80123EDC` is
  `gRaceSetup.p[i].car` (`gRaceSetup = 0x80123EA8`, `p` at +0x24, `RacerSetup` 0x14 bytes, `car` at +0x10).
  (2) `D_80123EC1` is read inside the first loop (`lbu $v0, 0x0($t1)` after the store), so it goes through the
  `lap = &D_80123EC1` pointer idiom already used by `Race_ReturnCup`. (3) `sprintf(buf1, ...)` is called in
  *both* arms of `if (i == 0)` and GCC cross-jumps the `jal` — writing it as a single call after the `if`
  costs ~17 words. (4) the blank-row loop reuses the `y` of the last row (original bug, must be kept).
- **Hipótesis probadas:** single `sprintf` after the `if` (289) → one `sprintf` per arm (**306**);
  `lap`/`recs` pointer idiom (−2), `s32 m, s` (−21), `s32 pos` (−20) all made it worse.
- **Residual:** 74 words: `pos` gets two moves instead of one, `y` is computed before the call instead of
  in the middle of the argument setup, and the blank-row loop keeps `y` in a different register.
- **Bytes recuperados:** 0 B. Draft: `.scratch/s45/cupresults_setuptexts_306.c`.

## Race_ReadPlayerControls (unit_2C6A0, 844 B / 211 instr, frame 0x30) — NON_MATCHING 194/211
- **REA/Ghidra:** `procedure_pseudo_code` confirmed the 13 `sysInputGetControl` reads, the `0x47B`
  (`finished`) early-out and the speed-dependent steering scale.
- **Confirmado en ASM:** `add.s $fv0, $fv0, $ft1` means the scale is `t * (b - a) + a`, not `a + t * (b - a)`
  (fixed this session); `a` is `$f6` and `b` is `$f8`, with `1.0f` in `$f0`.
- **Hipótesis probadas:** operand order of the final `add` (fixed, no net count change), `a`/`b` declaration
  order, hoisting them to the top of the function, inlining both `(f32)D_80138D4x` reads (179/213) and the
  Ghidra `if (0.0f <= t)` clamp shape (192) — none beat 194. ANSI vs K&R parameter form is equivalent.
- **Residual:** 17 words: `1.0f` must stay in `$f0` (we get `$f6`, which pushes `b` into `$f0`), and the
  second and third clamp arms must be branch-likely (`bc1tl` + `mov.s` in the delay slot) into the shared
  `sub.s` block. decomp-permuter only reached 595 from a 615 base.
- **Bytes recuperados:** 0 B. Draft: `.scratch/s45/race_readplayercontrols_194.c`.

## Coll_LoadLevel (unit_39C90) — still NON_MATCHING 155/159, 9 new hypotheses ruled out
- **Confirmado en ASM:** at `0x80099110` the ROM really loads the same symbol twice
  (`lw $a0, %lo(gCollLevel)($a0)` then `lw $v1, %lo(gCollLevel)($v1)`); both relocations resolve to
  `0x8011B334`. Everything else in the function (155/159 words, 628/636 B) already matches, including the
  registers: base in `$a0`, running pointer in `$v1`.
- **Hipótesis probadas (all 155/159 or worse):** `h = gCollLevel` local base; `(Ent90_ *)(gCollLevel + 1)`
  pointer arithmetic; `sizeof(Level_)` instead of `0xD8`; a running `p` assigned before the stores;
  `(u8 *)mem + 0xD8` with `mem` live across `svcFileLoad` (GCC keeps `mem` in `$s0` instead of
  rematerialising it from the global — 149/159); passing `gCollLevel` instead of `mem` to `svcFileLoad`;
  `u8 *mem`; and a `volatile` re-read (156/160 but 640 B — the volatile form needs `lui/addiu/lw`, the ROM
  has `lui/lw`).
- **Conclusión:** GCC 2.7.2 CSEs a repeated global load inside one basic block and does not invalidate it
  across stores through pointers, so no plain C form reproduces the duplicate. Two distinct pseudos are
  needed; the remaining idea is a second declaration of the same address, which would be match debt.
- **Bytes recuperados:** 0 B. Draft: `.scratch/s45/coll_loadlevel_155.c`.

# Session 46 (2026-11-25)

## Model_SetWheel (unit_06050, 140 B / 35 instr, frame 0x40) — MATCH exacto
- **REA/Ghidra:** not queried this session; session 40 already had the full signature and both callees
  (`Model_SetWheelContact`, `Model_SetPartRotation`), and the function has no globals.
- **Confirmado en ASM:** the C from session 40 was already correct; the only difference was register
  allocation (ROM: `slot`→`$s0`, `w`→`$s1`, `rz`→`$fs0`, `rx`→`$fs1`, `ry`→`$fs2`; ours swapped both sets).
- **Hipótesis:** compiling the same draft at `-O3` gave 35/35, so the difference is not in the code but in
  how GCC 2.7.2 treats a function it may inline (it keeps a copy of the RTL and renumbers the parameter
  pseudos, which changes the global-alloc order).
- **Cambios:** the definition is declared `inline void Model_SetWheel(...)` (non-static, so the body is still
  emitted at -O2); added the `Model_SetWheelContact` prototype before it.
- **Resultado objdiff:** 35/35, `make` OK (SHA-1 unchanged).
- **Bytes recuperados:** 140 B.
- **Pattern:** a near-miss whose only diff is the parameter register order can be a non-static `inline`
  function: test the draft at `-O3` first (`sbs.py ... -O3`). Checked on the session-45 drafts
  (Garage_SetupTexts, GfxPools_Init, CupResults_SetupTexts, Race_ReadPlayerControls, Coll_LoadLevel) and on
  every `#ifdef NON_MATCHING` block: no change there.

## Cup_SetupRace (unit_53B30, 508 B / 127 instr, frame 0x38) — NON_MATCHING 126/127 (was 96/127)
- **REA/Ghidra:** pseudocode from session 37 (`D_80100E98` / `D_80100EA0` 5-byte locals, stride 0x2C).
- **Confirmado en ASM:** (1) in the `cup == 2` path `D_80123EC1` is reloaded after every `sw` and its address
  is CSE'd in `$a0`: the lap byte is `gRaceSetup.f19` (a struct field, so MEM_IN_STRUCT aliasing forces the
  reload). With the scalar `D_80123EC1` GCC keeps it in a register (85 → 112/127). (2) Each arm of
  `if (sysRandRange(5))` writes both rows 1 and 2 (`f2A` and `lap[]`); GCC cross-jumps the shared tail.
  (3) The lap-clear address in the shuffle loop is `(&lap[l] + base) + i*44`, not `(base + i*44) + lap*4`.
- **Hipótesis probadas:** plain `D_80123F60[i].lap[l]` (112), `ofs` byte trick with a constant base (106:
  combine folds `l + i*11`), the same with a local `recs` base (120, but the base gets hoisted to `$s4`),
  and the row pointer `recs = (CupRec_ *)&D_80123F60->lap[D_80123EC1]; *(s32 *)((s32)recs + ofs) = 0;`
  (**126/127**). 25 further orderings/types and 20 min of decomp-permuter did not move the last word.
- **Residual:** 1 word of scheduling: `subu` of `i * 44` should come before the `lbu D_80123EC1`.
- **Bytes recuperados:** 0 B. Draft: `.scratch/s46/_v_c3.c` (header `.scratch/s46/cup_h.c`).

## Input_Init (unit_0B140, 600 B / 150 instr, frame 0x40) — MATCH exacto
- **REA/Ghidra:** not re-queried; the session-36 draft (`.scratch/ul/nr/near_6B2A0.c`) already had the logic
  (layout reset per pad, rumble probe, START-pad selection) and the 24-byte `D_800FEB20` initialiser.
- **Confirmado en ASM:** (1) the ROM keeps `&gInputLayoutCount` in `$s0` and derives the `gRumbleState`
  slot pointers (`$s0 + 0x54`) and `gContPattern` from it: the three symbols are one struct
  (`InputCtl_`: layout count, pattern at +0xC, four 0x54-byte slots at +0x10). With separate globals the
  code is the same but with `$s0`/`$s1` swapped (60 words); with a cast `*(T *)&gInputLayoutCount` GCC
  generates a different, shorter function. (2) The only other difference in the draft (147/151) was the
  delay slot of the first START test: the source loop is
  `i = 0; while (!(pads[i].button & 0x1000)) { if (++i >= 4) break; }`; `for` and `do/while` forms fill
  the slot from the branch target instead.
- **Cambios:** `gInputLayoutCount` is now declared `extern InputCtl_ gInputLayoutCount;` in the unit
  (`Input_SetLayout` uses `.layouts`); local `ContPadII_` (Pad_ without the tail byte).
- **Resultado objdiff:** 150/150, `make` OK.
- **Bytes recuperados:** 600 B.

## Mesh_GetMaterialList (unit_19D40, 392 B / 98 instr, leaf) — still NON_MATCHING 91/98
- **Confirmado en ASM:** the session-36 draft is right except for one register swap in the object-list loop:
  the ROM gives the `out[count++]` pointer (loop giv) `$t0` (reusing the command pointer's register) and the
  id `$t1`; GCC gives them the other way round.
- **Hipótesis probadas:** block-scoped id, reusing `id` for both loops (77), `out[count] = ...; count++`,
  operand order of the `&` test, `<` instead of `!=` (85), `s32`/`u16` types for `id`/`ref`/`count`, all 720
  declaration orders, `inline` / `-O3` (no change), decomp-permuter 400 s (best 40, only by reusing `max`,
  which changes the semantics).
- **Bytes recuperados:** 0 B. Draft: `.scratch/s46/ml1.c`.

## SndSrc_Play (unit_01050, 420 B / 105 instr, frame 0x48) — NON_MATCHING 92/105 (was 90/105)
- **Confirmado en ASM:** (1) the `Snd_IsPlaying` result is a variable reused for the new handle
  (`h = Snd_IsPlaying(n->f10); if (h == 0) { h = Snd_Play(...); n->f10 = h; Snd_SetPitch(h, pitch); return 1; }
  return 0;`), which gives the ROM's `beqz` / `j` layout (+2). (2) The ROM computes
  `Snd3D_GetVolume(n) * gSfxVolume` into `$fs0` and never reads it (the `$fs0` save/restore is why the frame
  is 0x48); every sibling (`CarSnd_StartLoopA/B`) uses that volume as `v = ... * 128.0f * vol;
  if (v == 0) v = 1;` for `Snd_Play`'s second argument, which is a constant 1 here.
- **Hipótesis probadas:** `if (vol) {}`, `v = vol * 0.0f` / `0 * vol` (keeps the mul but adds a
  conversion), unused `f32` parameter of a `static inline` helper, `inline` / `-O3`: none keeps exactly a dead
  `mul.s`.
- **Residual:** the dead `lwc1 gSfxVolume; mul.s` pair, `$fs1` for `pitch`, and the `move $a0, $v0` before
  the `beqz` (13 words).
- **Bytes recuperados:** 0 B. Draft: `.scratch/s46/sp3.c`.

## Gfx_SetPerspective (unit_103B0, 708 B / 177 instr, frame 0x40) — NON_MATCHING, rewritten with GBI macros
- **Confirmado en ASM:** (1) `$s0 = 1` is hoisted to the prologue and used both for `clipFar != 1` and for the
  four `gSPClipRatio(FRUSTRATIO_1)` words: a local `one = 1`. (2) The display list is exactly
  `GFX_RESERVE(6); gSPMatrix(proj | load | nopush); gSPPerspNormalize(norm); gSPClipRatio(FRUSTRATIO_1);`.
  (3) `far = D_800F2114; far *= 0.25f;` (load straight into `$fs0`) fixes the prologue.
- **Residual (111 normalized words):** the ROM reloads `D_800F2168` for `guPerspective`'s first argument and
  loads `aspect` with an integer `lw $a3` instead of reusing the FPR copy of `D_800F2124` from the fovy
  division, i.e. CSE lost both values before the call; we have not found which source form causes that.
  The tail (DL stores) differs only by scheduling.
- **Bytes recuperados:** 0 B. Draft: `.scratch/s46/gsp3.c` (applies inside unit_103B0 with `tryin.py`).

## Lap_GetTimes (unit_482C0, 624 B / 156 instr, frame 0x30) — MATCH exacto
- **REA/Ghidra:** not re-queried; the session-37 draft (`.scratch/ul/nr/near_A7C8C.c`, 120/157) had the logic
  (finish estimate from `Ai_GetRaceDistance`, per-lap random times, best lap).
- **Confirmado en ASM / Cambios (in order of effect):**
  1. `car->lapStart[lap] = X` evaluated the array address before the right-hand side; the ROM converts
     first: `t = (s32)((f32)t / car->k534);` as its own statement (122 → 18 normalized words).
  2. The random lap time adds the conversion first: `(s32)(gLapLength / k534) + svcRandRange(...)` (−1).
  3. `t = d * 1000` is computed before the remaining-distance threshold, written
     `n = (gLapCount - 1 - lap) * gLapLength;` (that form puts `lap + 1` in `$v0` like the ROM) (−9).
  4. decomp-permuter found the last 8 words: `gRaceClock` is read into a local before the store
     (`now = gRaceClock; car->lapStart[lap] = now + t;`), so it is loaded ahead of the `lap` index.
- **Resultado objdiff:** 156/156, `make` OK.
- **Bytes recuperados:** 624 B.

## Model_SwapPalette (unit_06050, 324 B / 81 instr, frame 8) — NON_MATCHING, 9 → 5 words
- **Confirmado en ASM:** the loop counter is unsigned (`sltiu`), and the palette index is first loaded into the
  same register as the counter (`$t3`): the source reuses the counter, `k = gModels[i]->palIdx;
  dst = gModels[i]->pals[k].colors;`.
- **Hipótesis probadas:** 8 orderings/types of the RGBA5551 pack expression, `static inline` pack helpers,
  4 forms of the palette address, decomp-permuter 360 s (best only with a bogus double store).
- **Residual:** the three `sll` of the pack come out lo-first (ROM: hi, mid, lo) and the palette pointer /
  offset use `$v1`/`$v0` instead of `$v0`/`$v1`. The improved draft replaces the `#ifdef NON_MATCHING` body.
- **Bytes recuperados:** 0 B.

## Gfx_ClearFrame (unit_13490, 668 B / 167 instr, frame 0x30) — NON_MATCHING, rewritten with GBI macros (52 words)
- **Confirmado en ASM:** the function is the standard z-buffer + frame-buffer clear:
  `GFX_RESERVE(12); gDPPipeSync; gDPSetDepthImage(OS_K0_TO_PHYSICAL(D_801380A8)); gDPPipeSync;
  gDPSetCycleType(G_CYC_FILL); gDPSetColorImage(RGBA, 16b, w, zbuffer); gDPSetFillColor(GPACK_ZDZ x2);
  gDPFillRectangle(0, 0, w - 1, h - 1); gDPPipeSync; gDPSetColorImage(..., osVirtualToPhysical(D_8014C648));
  gDPSetFillColor(GPACK_RGBA5551(r, g, b, 1) x2); if (gBgImage == 0) gDPFillRectangle(...); gDPPipeSync;`
  With the macros the whole middle (0x88–0x1E8) is identical (the old hand-written draft was 84/167).
- **Residual:** parameter registers (ROM: r→`$s5`, g→`$s6`, b→`$s4`; ours r→`$s4`, g→`$s5`, b→`$s6`) and
  the scheduling of the second fill colour. `inline`, K&R definition and signed/unsigned parameter types do
  not change the allocation; decomp-permuter (1630 → 720) only by inserting an empty `do {} while (0)`.
- **Bytes recuperados:** 0 B. Draft: `.scratch/s46/cf1.c` (applies in unit_13490 with `tryin.py`).

## Gfx_SetViewport (unit_103B0, 820 B) — investigation only
- **Confirmado en ASM:** `gDPPipeSync; gDPSetScissor(G_SC_NON_INTERLACE, x, y, x + w - 1, y + h - 1);
  gSPViewport(*vp)` with a manual 3-command reserve on a `gDisplayListHead` copy taken before the Vp stores.
  The ROM reloads `D_800F2170` after every `sh` into the viewport, i.e. those stores are treated as aliasing
  the pointer; `Vp *`, `s16 *`, `volatile` and a one-field struct view all fail to reproduce that order.
- Draft: `.scratch/s46/sv1.c`; the unit was left unchanged.

**Session 46 result:** 3 exact matches (Model_SetWheel 140 B, Input_Init 600 B, Lap_GetTimes 624 B);
objdiff 78.92 % (1,794 / 1,910 functions). No commit (below the 30-function threshold).

# Session 47 (2026-11-25)

## Bg_DrawImage (unit_13490, 1204 B / 301 instr, frame 0x40) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`0x80078320`) y `procedure_callees` (`GfxPool_Grow`); mostró la división por tiles de 64×32, reserva de `n*10 + 14` comandos, paleta CI8 vía `gBgPalette`, `timg + 0x200` y bucle `x += 64` / `y += 32`.
- **Confirmado en ASM:** los comandos GBI coinciden con la hermana `Image_DrawCI8Scaled`: `gDPPipeSync`, `G_CYC_COPY`, `G_RM_NOOP/G_RM_NOOP2`, `G_TT_RGBA16`, `G_TP_NONE`, `G_TF_POINT`, `gDPLoadTLUT_pal256(gBgPalette)` (que ya incluye su `gDPPipeSync` final), `timg += 0x200`, `gDPLoadTextureTile` CI8 + `gSPTextureRectangle` (`4<<10, 1<<10`), y restauración `G_TT_NONE` + `G_TF_BILERP`.
- **Hipótesis:**
  1. Con `GFX_RESERVE(n + 14)` + `gDPPipeSync` extra + `timg += 0x200` antes de los macros: 283/306 (1224/1204 B).
  2. Quitando el `gDPPipeSync` duplicado y moviendo `timg += 0x200` tras `gDPLoadTLUT_pal256`: **300/301** (1204/1204 B), único diff `addu $a0, $a0, $a2` vs `addu $4, $6, $4` en `00E0`.
  3. Escribiendo la suma de reserva como `(u32)((u32)(n * 8) + (u32)gDisplayListHead + 14 * 8)`: **301/301** exacto.
- **Cambios:** integrada `Bg_DrawImage` en `src/unit_13490.c` reemplazando `INCLUDE_ASM`.
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK (SHA-1 intacto), `verify_funcs.py` OK.
- **Bytes recuperados:** 1204 B.

## Model_UpdateWheelEffects (unit_06050, 1140 B / 285 instr, frame 0xE0) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`0x80065940`) y `procedure_callees` (`SkidMark_AddQuad`, `Particle_SpawnColored`, `Particle_SpawnJitteredVel`, `rand`); reveló el doble bucle 6×`nParts`, los dos umbrales aleatorios (`0x6E` y `0x3C`), el caso especial de superficie `11 || 12` y la llamada a `SkidMark_AddQuad` seguida de copia de `lastPos`/`lastN` y `partA = 1`.
- **Confirmado en ASM:** `$s0 = &D_80138AAC` sirve de base negativa para `gModels` (`-0x938`), `partB` (`-0x8F8`) y `partC` (`-0x8D4`), lo que exige declarar `gModels` como struct unificado `ModelAll_` (sin cast). El color `D_800F0220[surf]` se pasa como 3 escalares `s32` (`$s1/$s5/$s6`), y `b`/`surf` son `s32`.
- **Hipótesis:**
  1. Struct unificado `ModelAll_` + `Vec3f col`: 223/285 (1140/1140 B).
  2. Color como 3 escalares `s32 c0, c1, c2` y `s32 b, surf`: 273/289 (recargaba `gModels.models[i]` y hoisteaba `20.0f` a `$f20`).
  3. Puntero local `Model_ *m = gModels.models[i]` para el bloque `pos += m->mtx[0][2] * 20.0f`: **285/285**.
  4. Al integrar en `src/unit_06050.c`, el cast `((ModelAll_ *)&gModels)` rompió el código (PATTERNS.md); cambiando `extern Model_ *gModels[6]` a `extern ModelAll_ gModels` y declarando `u32 j; u32 i;` (para fijar los spills `0x5C/0x64($sp)`): **285/285** y ROM OK.
- **Cambios:** `src/unit_06050.c` (`ModelAll_ gModels`, `Model_UpdateWheelEffects`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 1140 B.

## Ai_RestoreSlot (unit_22440, 2100 B / 525 instr, leaf) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`0x800937e8`), sin callees (hoja); mostró la copia de struct de `0x88` bytes (`blk`), inicialización de `v94[0..3]`, matriz `m[3][3]`, dos bucles de 10 elementos (`h1`/`h2`) y puesta a cero de contadores/vectores.
- **Confirmado en ASM:** todos los accesos recargan `car->id` y multiplican por `0x2A8` sobre `gAi` (sin puntero local ni cast sobre `gAi`).
- **Hipótesis:**
  1. Borrador directo usando `SlotD_` sobre `gAi[car->id]`: **525/525** al primer intento.
  2. Al integrar en `src/unit_22440.c`, ni puntero local `ai` (`0x760`) ni cast `((SlotD_ *)gAi)` (`0x7E8`) igualaban; unificando los campos de `SlotD_` en `Slot85_` (unión en `+0x08` para `f24` y `blk`): **525/525** exacto (`0x834`).
- **Cambios:** `src/unit_22440.c` (`Slot85_` unificado + `Ai_RestoreSlot`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 2100 B.

## MenuCar_Draw (unit_53B30, 1608 B / 402 instr, frame 0x80) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (4), `procedure_callees` (`Menu_DeltaTenths`, `Menu_DeltaSeconds`, `Menu_InitViewMtx`, `cosf`, `sinf`); mostró el suavizado de ángulo/escala/posición y las dos rotaciones de matriz (`-80°` y `D_800F60B0[id]`) iguales a la hermana `Menu_DrawModel`.
- **Confirmado en ASM:** `sw $a1..$a3` a `0x84..0x8C($sp)` confirma segundo parámetro `Vec3f off` por valor; `0x10($sp)` es `Vec3f d` declarado antes de `f32 m[3][4]` (`0x20($sp)`).
- **Hipótesis:**
  1. Primer borrador con `id != 0` en `Menu_DeltaTenths`/`Menu_DeltaSeconds`: 357/402 (`sltu` extra y `$s2`).
  2. `if (id == 0) ... else ...` para ambas llamadas: 384/402 (tamaño exacto 1608/1608).
  3. Local `f32 diff = D_800F60B8[id] - D_800F60B0[id]` (y análogo para `60A8/60A0`) para evaluar el destino antes del origen: **402/402**.
  4. Al integrar en `src/unit_53B30.c`: declarados `sysModelRotateAllParts`/`sysModelDrawWithLookAt`, actualizados los 6 llamadores a `*(Vec3f *)&D_801240E8/F4`, y `Vec3f d` antes de `m[3][4]`: **402/402** y ROM OK.
- **Cambios:** `src/unit_53B30.c` (`MenuCar_Draw` y prototipo/llamadores).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK; **objdiff oficial: 80.01 % (442,280 / 552,756 B, 1,798 / 1,910 funciones, 174 / 221 unidades completas)**.
- **Bytes recuperados:** 1608 B.

## Borradores casi iguales de la sesión 47 (no integrados, 0 B oficiales)
- **Hud_DrawTurbos (`unit_49CF0`, 1528 B):** `.scratch/s47/hd_f2.c` (**374/382**, 1524/1528 B, registros exactos, solo falta `addu $s4, $v0, $zero` en `0130`) y `.scratch/s47/hd_m2.c` (342/382, **1528/1528 B**, instrucciones exactas con rotación `$s4..$s7`).
- **Coll_SyncObjects (`unit_39C90`, 1216 B):** `.scratch/s47/cs_for.c` (**277/304**, 1212/1216 B); resueltos `&` bit a bit en los umbrales de reposo, `!(x < 0.0f) && !(x > 0.0f)` y `static inline TransformShape_`; residual: `i = 0` se hoistea al delay slot de `004C` en vez de `0140`.
- **Audio_Init (`unit_01050`, 1164 B):** `.scratch/s47/audio_init.c` (**273/291**, 1164/1164 B); struct `AudioGlobals_` iguala exactamente los dos bucles regresivos (`+0x28` y `+0x10C`) y 5 símbolos ROM separados fijan el tamaño exacto; residual: `sched` hoistea `lui $a0, PTR_ROM_START` a `00C8` en vez de `0338`.

**Session 47 result:** 4 exact matches (`Bg_DrawImage` 1204 B, `Model_UpdateWheelEffects` 1140 B, `Ai_RestoreSlot` 2100 B, `MenuCar_Draw` 1608 B = **+6,052 B**); **objdiff 80.01 % (442,280 / 552,756 B, 1,798 / 1,910 functions, 174 / 221 complete units)**. No commit (below the 30-function threshold).

# Session 48 (2026-11-25)

## RaceAudio_Update (unit_2C6A0, 1400 B / 350 instr, frame 0xB8) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`0x8008c548`), `procedure_callees` (`AnimTrack_GetPos`, `RaceCam_GetListener`, `Car_GetAudioInfo`); mostró el temporizador de voz (`D_80118534`), bucle de coches con clamp/rate-limit de RPM, loops A/B y bucle de fuentes animadas (`D_800F3704`).
- **Confirmado en ASM:** Ghidra omitió el argumento de la llamada indirecta `audVoicePlayRandom(D_8011852C)` (`$a0` ya cargado antes de `bne $a0, 3`); el bloque de punteros de audio en `0x8014C0D0..0x8014C0F4` se direcciona desde una struct `AudLoop_` (`$s4`, `$s6`, `$s7`); `D_801184C0` es `u8` (`lbu`).
- **Hipótesis:**
  1. Borrador inicial con `AudLoop_` y `MIN/MAX`: 296/357 (1428/1400 B).
  2. `slipB < slipA ? 0 : 1`, `if (hold <= dt)` primero, puntero `E10_ *p` con `p++, i++`, y `u32 *tp = &D_80118534`: 339/350 (1400/1400 B).
  3. Añadido argumento `audVoicePlayRandom(D_8011852C)` y cambiado `D_801184C0` de `s8` a `u8` en `src/unit_2C6A0.c`: **350/350** exacto.
- **Cambios:** `src/unit_2C6A0.c` (unificado `E10b_`, `D_801184C0` como `u8`, integrada `RaceAudio_Update`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK (SHA-1 intacto), `verify_funcs.py` OK.
- **Bytes recuperados:** 1400 B.

## CollBox_EdgeSeparatingAxis (unit_39C90, 1220 B / 305 instr, leaf, frame 0x98) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`0x800a25ec`), sin callees (hoja); mostró la transposición de la matriz 3×3 de `boxA`, transformación de `dir` y `b->pos - a->pos`, y las 7 pruebas SAT con salidas `1, 2, 3, 5, 7, 10, 13, 0` y escrituras en `D_8011B764[]`.
- **Confirmado en ASM:** `Mat3T_MulVec_` inline con `Vec3f *` reproduce la copia de retorno por `0x58($sp)` a `0x00($sp)` y `0x48($sp)`; `if (v < 0.0f) v = -v;` evita `mov.s`; `f32 eps = 0.001f;` fija la carga en `0x170`.
- **Hipótesis:**
  1. Macro `ABSF` + paso por valor: 58/364 (1456/1220 B, evaluaba doble).
  2. `Mat3T_MulVec_` por puntero + locales `tx/ty/tz/hx/hy/hz/len/dx/dy/dz` + `if (v < 0.0f) v = -v;`: 233/310.
  3. Orden `tx..len..dx`, parentización `tx*dx + (ty*dy + tz*dz)` / `hx*ax + (hy*ay + hz*az)` y `f32 eps = 0.001f;`: **305/305** exacto.
- **Cambios:** `src/unit_39C90.c` (`CollBox_EdgeSeparatingAxis`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 1220 B.

## Cars_Init (unit_31E10, 2320 B / 580 instr, frame 0xD8) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`0x8008baa0`), `procedure_callees` (`Coll_GetStartPos`, `Car_ComputeMass`, `RigidBody_Init`, `Quat_ToMtx3`, `Bld_GetStartPos`, `Ai_InitRacer`, `Coll_AddCar`, `Race_LoadProgressStep`, `memset`).
- **Confirmado en ASM:** `RigidBody_Init` recibe 3 escalares `extent.x/y/z` + `Vec3f zero`; `svcModelLoadDefault`/`svcCarInfoLoadEx` en struct `SvcModel_`; `svcModelSetTransform` direcciona `&gCars[i].m2C + 0x94` (`$s7`); `switch (cfg->p[i].kind)` para flags 1/2; `gCars[i].k53C *= ...; gCars[i].k53C = MIN(..., 4.0f);` duplicado en ambas ramas del `if/else` humano/AI.
- **Hipótesis:**
  1. Borrador inicial con `Vec3f` en `RigidBody_Init`: 464/580.
  2. 3 floats en `RigidBody_Init`, `switch (kind)`, `offset = gCars[i].center` copia de struct y `k53C` en ambas ramas: 557/581.
  3. `(Mtx3f *)((u8 *)&gCars[i].m2C + 0x94)` en `svcModelSetTransform`: 575/580 (2320/2320 B).
  4. `f32 ox = offset.x;` antes de `offset.z += h;` con multiplicación matriz-vector expandida: **580/580** exacto.
- **Cambios:** `src/unit_31E10.c` (`#include "race.h"`, `Cars_Init`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 2320 B.

## Model_SetPartRotation (unit_06050, 1276 B / 319 instr, frame 0xB0) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (5), `procedure_callees` (`cosf`, `sinf`); mostró la matriz 3×4 en pila (`piv`, `(0,0,ry)`, `RotateZ(-rx)`, `RotateX(-rz)`, `-piv`), recorrido de lista de hijos (`+0x50`/`+0x4C`), copia a `mtx[part + 1]` y escalado por `D_80138AB4`.
- **Confirmado en ASM:** `Mtx34_Identity` con asignación encadenada `= 0.0f` (como `Model_SetTransform` en la misma unidad) emite `swc1 $fv0`; `D_80138AB4` recargado 9 veces porque pertenece a `D_80138190.partScale` (`MEM_IN_STRUCT_P`); rotaciones fila a fila con 2 temporales `a, b` (como `unit_53B30.c`).
- **Hipótesis:**
  1. Helpers `static inline` con 6 temporales + `D_80138AB4` suelto: 121/319 (1176/1276 B).
  2. `D_80138190.partScale` + asignación encadenada `= 0.0f` + `m[r][3] += ...`: 215/319 (1248/1276 B).
  3. `Mtx34_RotateZ_`/`Mtx34_RotateX_` fila a fila con 2 temporales `a, b`: **319/319** exacto.
- **Cambios:** `src/unit_06050.c` (`MdlInst_.next`/`child`, `Model_.partPos[6]`, `Model_SetPartRotation`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 1276 B.

## Clouds_Init (unit_0B140, 1268 B / 317 instr, frame 0xA0) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`0x800655cc`), `procedure_callees` (`Rand_Range`, `sprintf`, `File_GetSize`, `Memory_Alloc`, `File_Load`); mostró los 5 dobles en `.rodata` (`21000.0`, `14000.0`, `4294967296.0`), rejilla hasta 6×6, relleno hasta 36 y carga de 3 texturas.
- **Confirmado en ASM:** `vel` es struct `Vec2f_` copiado con `lw/sw`; `s32 rv = Rand_Range(100); vel.x = (f64)(rv % 7 - 3);` genera `addu $a1, $v0, $zero` antes de `mult`; `if (max->z + 1200.0f > 10000.0f)` mantiene `z` en `0x34($sp)`; `gCloudCount = nx * ny` antes de `D_80101EA0[1] = min->y`.
- **Hipótesis:**
  1. `vx`/`vy` separados y `i` reutilizado: 199/320 (1280/1268 B).
  2. `Vec2f_ vel` + `s32 k` separado: 285/317 (1268/1268 B).
  3. `c++, j++`, `c++, k++`, `if (max->z + 1200.0f > 10000.0f)`, `gCloudCount` antes de `D_80101EA0[1]`: 301/317.
  4. Local `s32 rv = Rand_Range(100);`: **317/317** exacto.
- **Cambios:** `src/unit_0B140.c` (`#include "math.h"`, `Clouds_Init`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK; **objdiff oficial: 81.37 % (449,764 / 552,756 B, 1,803 / 1,910 funciones, 174 / 221 unidades completas)**.
- **Bytes recuperados:** 1268 B.

## Borradores casi iguales de la sesión 48 (no integrados, 0 B oficiales)
- **Hud_ShowTrick (`unit_49CF0`, 1984 B):** `.scratch/s48/ht5_cond_not1.c` (**491/497**, 1988/1984 B) y `.scratch/s48/ht4_if2.c` (**485/496**); resueltos `t0`/`t1`, orden `if (gHudPlayerCount != 2)`, `static inline MeasureText_` con `Font_GetKerning(0, *s++, *s)`; residual: 5 palabras en el `sprintf` de `turbos == 1` (GCC propaga `turbos` a `1` en `if/else` y hoistea `D_800F39F4` en ternario).
- **SortQueue_Sort (`unit_20830`, 816 B):** `.scratch/s48/sq_v3.c` (66/204, 808/816 B, desplazamiento de 1 registro en `l`/`sp`/`r`).
- **Hud_DrawTurbos (`unit_49CF0`, 1528 B):** `.scratch/s48/hd_v5.c` (342/382, 1528/1528 B exacto; `count = (D_800F3B18[idx].b = turbos - base);` iguala `0x12C..0x13C` pero rota `$s4..$s7`).

**Session 48 result:** 5 exact matches (`RaceAudio_Update` 1400 B, `CollBox_EdgeSeparatingAxis` 1220 B, `Cars_Init` 2320 B, `Model_SetPartRotation` 1276 B, `Clouds_Init` 1268 B = **+7,484 B**); **objdiff 81.37 % (449,764 / 552,756 B, 1,803 / 1,910 functions, 174 / 221 complete units)**. No commit (below the 30-function threshold).

# Session 49 (2026-11-25)

## Input_Update (unit_0B140, 736 B / 184 instr, frame 0x28) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (indirecto vía `sysInputUpdate`), `procedure_callees` (`osGetCount`, `nuContDataGet`); mostró la estructura del bucle de 14 controles (8 analógicos con zona muerta `±7.0`, escala `±255.0 / 63.0`, y 6 digitales con umbral `±40`), clamping `[0, 255]` y guardado de `time`.
- **Confirmado en ASM:** `D_800FEBB8..D_800FEBE0` en ROM son `4294967296.0, 7.0, 255.0, 63.0, -7.0, -255.0`; el despacho `0..7` vs `8..13` (`slti 8; beqz; bltz`) es un `switch (i)` (casos `0..7` + `default`), y el `switch (bind)` ordena casos `0x80000, 0x40000, 0x20000, 0x10000, 0, default`.
- **Hipótesis:**
  1. `if (i < 8 && i >= 0)` con `f64 dt`: 131/190 (`sltiu 8` en vez de `slti 8 + bltz`).
  2. `switch (i)` + `switch (bind)` con casos en orden inverso y `u32 bind; s32 val;`: 178/184 (736/736 B).
  3. `f32 dt2 = (f64)(u32)(t - gRumbleState[pad].time); if (dt2 < 0.0f) dt2 = 0.0f;` + `bind & cont.button`: 182/184.
  4. `if (bind & cont.button) { val = 1; } else { val = 0; }` en el `default:` digital: **184/184** exacto.
- **Cambios:** `src/unit_0B140.c` (`Input_Update`; completa `unit_0B140` al 100 %).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK (SHA-1 intacto), `verify_funcs.py` OK.
- **Bytes recuperados:** 736 B.

## Ai_InitRacer (unit_22440, 2700 B / 675 instr, frame 0x20) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`Cars_Init`), `procedure_callees` (`Bld_InitRacer`, `RaceClock_Get`); mostró el lerp inicial de `D_80138D3E..3F`, copias de posición/matriz, puesta a cero de vectores, `Bld_InitRacer`, los dos bucles de 10 entradas (`h1`/`h2`), el LCG `D_80115854 * 0x19660D + 0x3C6EF35F % 201 - 100` y la conversión `u32 -> f64 -> f32` de `D_80115824 * D_80115820`.
- **Confirmado en ASM:** igual que la hermana `Ai_RestoreSlot`, todos los accesos sobre `gAi[car->id]` usan `Slot85_` unificado sin puntero local; `divu` por `0xC9` (sin multiplicación mágica) proviene de una función `static inline u32 Ai_Rand_(u32 n)` (GCC 2.7.2 expande `% n` antes de inlinear la constante).
- **Hipótesis:**
  1. Borrador inicial con `% 201` directo y `Lerp85`: 640/681 (GCC emitió multiplicación mágica en vez de `divu`).
  2. `static inline u32 Ai_Rand_(u32 n)` + lerp directo: 662/675 (2700/2700 B).
  3. Moviendo `gAi[car->id].f1FC = (s32)Ai_Rand_(201) - 100;` tras `n1DC/z1F8/z200/z204 = 0;`: **675/675** exacto.
- **Cambios:** `src/unit_22440.c` (unificados `ok`, `f1FC`, `f258..u264`, `s2A0..b2A5` en `Slot85_`, `Ai_Rand_`, `Ai_InitRacer`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 2700 B.

## Car_AirControl (unit_31E10, 3120 B / 780 instr, frame 0x140) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`Car_UpdatePhysics`), `procedure_callees` (`RigidBody_ConstrainSpin`); mostró la zona muerta `±0.2f`, amortiguación `0.98f` de `vE4`, selección de eje dominante (`yaw`/`roll`/`pitch`), pares de fuerzas opuestas en `center ± axis * scale` y `RigidBody_ConstrainSpin(..., 1/2/0)`.
- **Confirmado en ASM:** usa los helpers inline `Vec3_Scale`/`Add`/`Sub`/`Neg` y `push_AC_` (análogo a `push_954` de `Car_SelfRight`), copia `Mtx34f m` en `0x40($sp)`, `sPitch = car->scale; sYaw = sPitch; sRoll = sPitch;` con `sPitch *= pitch;` primero, `Vec3_Neg_(&f)` después de `Vec3_Sub_`, y dos locales `tq1`/`tq2` en la rama `yaw` (`0xF0`/`0x100($sp)`) frente a `tq1` reutilizado en `roll`/`pitch` (fija marco `-0x140`).
- **Hipótesis:**
  1. Primer borrador con `f`/`pt` compartidos: 442/780 (3072/3120 B).
  2. `Vec3_Scale_(&car->vE4, &car->vE4, 0.98f)`, `f1/f2/f3`, `pt1/pt2/pt3` y `sPitch *= pitch` primero antes de `center`: 688/780 (3120/3120 B).
  3. Inercias antes de `center` + `Vec3_Neg_` tras `Vec3_Sub_` + `tq1`/`tq2` explícitos en `push_AC_`: **780/780** exacto y ROM OK.
- **Cambios:** `src/unit_31E10.c` (`Car_AirControl` y prototipo `void Car_AirControl(Car *);`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 3120 B.

## CollBox_FindSeparatingAxis (unit_39C90, 3176 B / 794 instr, leaf, frame 0xC0) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`Coll_BroadPhase`, `Coll_ResolveBoxPair`), sin callees (hoja); mostró la matriz traspuesta `mt`, producto `R = mt * b->m`, vector `t = mt * (b->pos - a->pos)`, `abs(R) + 0.001f` y las 15 pruebas SAT guardando `D_8011B764[0..14]`.
- **Confirmado en ASM:** hermano directo de `CollBox_EdgeSeparatingAxis`: `Mat3T_MulVec_FSA_` devolviendo `Vec3f` por valor, `f32 eps = 0.001f;`, `if (v < 0.0f) v = -v;` y sumas asociativas por la derecha (`a + (b + (c + d))`).
- **Hipótesis:**
  1. Borrador inicial con sumas asociativas por la izquierda: 587/794 (3132/3176 B; `0x000..0x264` exacto).
  2. Parentización asociativa por la derecha en los 15 ejes SAT y 3 productos escalares: **794/794** exacto.
- **Cambios:** `src/unit_39C90.c` (`Mat3T_MulVec_FSA_`, `CollBox_FindSeparatingAxis`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 3176 B.

## SkidMark_AddQuad (unit_1CC80, 1188 B / 297 instr, frame 0xB0) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`Model_UpdateWheelEffects`), `procedure_callees` (`sqrtf`); mostró la copia del quad plantilla `D_800F23C0`, el desplazamiento `z += 3.5` (`D_800FF9F8`), el cálculo del radio máximo y la inserción vía `ParticlePool_Push(&gParticlesAlt, &p)`.
- **Confirmado en ASM:** 5 `Vec3f` pasados por valor (`p0..p3, center`) + `s32 alpha, s32 surf`; `surf = D_800F2634[surf];` reutiliza `$a0` (forzando `lui $at` en `gas`); orden de inicialización `flags, life, vx=vy=vz=0.0f, f58, alpha, alphaVel, r=g=b=0xFF, kind`; `static inline f32 maxf_sm_(a, b)` para los 4 máximos.
- **Hipótesis:**
  1. Borrador inicial con `p.vx/vy/vz` separados y `if`: 151/297 (1172/1188 B).
  2. Encadenados `= 0.0f` y `= 0xFF` + `static inline maxf_sm_` + `p0..p3 -= center` antes de `+= 3.5`: 267/297 (1188/1188 B).
  3. `surf = D_800F2634[surf];` al principio + `p.alphaVel = -0x40;` tras `p.alpha`: **297/297** exacto en `src/unit_1CC80.c`.
- **Cambios:** `src/unit_1CC80.c` (`maxf_sm_`, `SkidMark_AddQuad`; completa `unit_1CC80` al 100 %).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK, `check_no_asm.py` OK; **objdiff oficial: 84.37 % (466,380 / 552,756 B, 1,814 / 1,910 funciones, 176 / 221 unidades completas)**.
- **Bytes recuperados:** 1188 B.

## Borradores casi iguales de la sesión 49 (no integrados, 0 B oficiales)
- **Mesh_GetMaterialList (`unit_19D40`, 392 B):** `.scratch/s49/mg8.c` (**91/98**, bucle 1 y cola exactos; residual: `$t0/$t1` intercambiados en bucle 2).
- **SndSrc_Play (`unit_01050`, 420 B):** `.scratch/s49/sp5.c` (**96/105**, 416/420 B; `if (vol < 0.0f) vol = 0.0f;` mantiene `mul.s $fs0`; residual en offset `-0x378` y `busy == 0`).

**Session 49 result:** 5 exact matches (`Input_Update` 736 B, `Ai_InitRacer` 2700 B, `Car_AirControl` 3120 B, `CollBox_FindSeparatingAxis` 3176 B, `SkidMark_AddQuad` 1188 B = **+10,920 B**; 2 units completed: `unit_0B140`, `unit_1CC80`); **objdiff 84.37 % (466,380 / 552,756 B, 1,814 / 1,910 functions, 176 / 221 complete units)**. No commit (20 functions gained since HEAD `51af82d`, below the ~30-function threshold).

---

## Sesión 51 (2026-11-25)

## Hud_ShowTrick (unit_49CF0, 1984 B / 496 instr, frame 0x50) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** Reutilizado análisis previo de sesión 48 (`.scratch/s48/ht5_cond_not1.c`, 491/497; estructura, `MeasureText_`, bubble sort y `D_800F3934`/`D_800F3B18` ya confirmados en REA/Ghidra).
- **Confirmado en ASM:** En `0x041C..0x0458`, las ramas `turbos == 1` (`D_80141570`) y `else` (`D_80141574`) cargan cada una `$a0` y `$a3`, y comparten la cola cross-jumped `D_80100AD0` + `andi $a2, $s5, 0xFF` + `jal sprintf`; pasar `(s16)turbos` a `sprintf` (igual que `(s16)pts`) impide que GCC propague la constante `1` en `$a2`. Además, `D_80100AC4` (`{0, 1, 2}`) se copia como `Order3_49CF0_` y `HudF0_` usa `ElPair_ el[3]` sin cast para preservar los símbolos `D_800F39A4`/`D_800F39F4`; `svcFontGetCharWidth` debe quedar declarada fuera de `#ifdef NON_MATCHING`.
- **Hipótesis:**
  1. Ternario dentro de `sprintf`: 485/496 (1968/1984 B).
  2. `if (turbos == 1)` con `turbos` como `u8`: 486/498 (GCC sigue propagando `li $a2, 1`).
  3. `if (turbos == 1)` pasando `(s16)turbos` a `sprintf` en ambas ramas + `Order3_49CF0_` (`D_80100AC4`) + `ElPair_ el[3]` en `HudF0_`: **496/496** exacto y ROM OK.
- **Cambios:** `src/unit_49CF0.c` (`E0Cb_.e`, `ElPair_`, `HudF0_.wC`/`el[3]`, `MeasureText_49CF0_`, `Hud_ShowTrick`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 1984 B.

## Garage_SetupTexts (unit_53B30, 1388 B / 347 instr) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** Reutilizado análisis previo de sesión 45 (`.scratch/s45/garage_setuptexts_341.c`, 341/347, tamaño exacto 1388/1388 B; residual de 6 palabras: `prev` en `$a2` en vez de `$v0` en P1 y P2).
- **Confirmado en ASM:** En `0x5F808..0x5F83C` (`Garage_SetupTexts.s`), `addiu $v0, $v1, -1` va en el delay slot de `bnez $v0, .L800BEC38` (tras `slti $v0, $v0, 0x29`), lo que exige que `prev` no esté vivo durante el `slti`.
- **Hipótesis:**
  1. Convertir `else { prev = i - 1; if (i + 1 >= 0x29) ... else ... }` en `else if (i + 1 >= 0x29) { prev = i - 1; ... } else { prev = i - 1; ... }` en P1 y P2: `prev` deja de vivir a través de `slti` y entra en `$v0`, logrando **347/347** exacto.
- **Cambios:** `src/unit_53B30.c` (`Garage_SetupTexts`, usando `*(u8 *)&gGarageDirtyP1/P2` para `lbu` sin alterar otras funciones).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 1388 B.

## Coll_BoxVsWorldEdges (unit_39C90, 3784 B / 946 instr, frame 0x148) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`Coll_RacersVsWorldEdges`), `procedure_callees` (`CollBox_EdgeSeparatingAxis`); mostró el bucle sobre `obj->tris`, carga de aristas `EdgeBwe_` (`stride 0x14`), llamada a `CollBox_EdgeSeparatingAxis`, transformación a espacio local de la caja con `mt`, recorte del segmento `[p0, p1]` contra los 6 planos `±hx/±hy/±hz`, cálculo de profundidades `dx/dy/dz`, empuje de `body->offset`, y registro de hasta 2 contactos en `gCollContacts`.
- **Confirmado en ASM:** `CollBox_EdgeSeparatingAxis` devuelve `u8`; `Vec3_Sub(&tmp, &seg.pos, &box->pos)` antes de `Mat3_MulVec(mt, &tmp)`; cada uno de los 6 recortes guarda `u8 a = (p0.c < hc); u8 b = (p1.c < hc);` y reutiliza `if (!a)` (`bnez $v1`) sin reevaluar `c.lt.s`; una sola variable `f32 t` compartida entre los 6 recortes, `depth` y `dot` fija `$fa1` (`$f14`) y desplaza `ldir.y` a `$a2`/`$fs1`; `dxp..dzm` con ternarios `(p0.c < p1.c)` y `((p0.c > p1.c) ? p0.c : p1.c) - (-hc)` emiten `neg.s` + `sub.s`; `tmp.x = ldir.x * seg.len;` directo (sin inline) fija el orden `0x03D8`/`0x03DC`.
- **Hipótesis:**
  1. Borrador inicial con `t` locales y `if (p0.x >= hx)`: 648/946 (3736/3784 B).
  2. `u8 CollBox_EdgeSeparatingAxis`, `Vec3_Sub(&tmp, ...)`, `u8 a, b; if (!a)` + `f32 t` compartida con `depth`: 846/946 (3768/3784 B).
  3. Ternarios `dxp..dzm` con `neg.s` + `sub.s` y `t = Vec3_Dot(...)`: 945/946 (3784/3784 B).
  4. `tmp.x/y/z = ldir.x/y/z * seg.len` directo: **946/946** exacto y ROM OK.
- **Cambios:** `src/unit_39C90.c` (`Coll_BoxVsWorldEdges`, `u8 CollBox_EdgeSeparatingAxis`, prototipo `void Coll_BoxVsWorldEdges(void *);`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK, `check_no_asm.py` OK.
- **Bytes recuperados:** 3784 B.

## ModelData_Draw (unit_13490, 3572 B / 893 instr, frame 0x160) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers` (`World_Draw`), `procedure_callees` (`ModelData_SetVariant`, `Mtx34_Mul`, `Frustum_TestSphere`, `Mtx_New`, `SortQueue_Add`, `SceneNode_DrawMesh`, `SceneNode_DrawSingle`, `Particle_SpawnColored`, `ModelData_DrawShadow`, `rand`, `sqrtf`); mostró la selección de LOD `0..3` contra `gLodDist0..3`, el recorrido de nodos e hijos con `childMtx[4][3][4]`, el quad deformable `envVtx[16]` encolado con `SortQueue_Add`, la emisión de partículas `(arg6 != 0) & (arg5 == 1)` y la llamada final a `ModelData_DrawShadow`.
- **Confirmado en ASM:** `SceneNode_DrawSingle` y `ModelData_SetVariant` están expandidas `inline` dentro de `ModelData_Draw`; en el bucle de nietos (`child->child`), guardar `a0Mtx = &m->childMtx[j]; a1Mtx = child; Mtx34_Mul((s32)a0Mtx, (s32)a1Mtx, tmpMtx);` reproduce el orden exacto de carga de `$a0`/`$a1`; el bloque del quad reflectante `m->alphaEnv` actualiza los 16 vértices `ev[0..15]` simétricos respecto a `ev[0..7]` con `iy = (t0 >= 25.0f) ? 50 : (s32)(t0 * 2.0f)`.
- **Hipótesis:**
  1. Llamadas no inlined a `SceneNode_DrawSingle` y `ModelData_SetVariant`: frame `0x130` en vez de `0x160` (`buf[0x30]` faltante).
  2. Marcando `inline` ambas funciones en `src/unit_13490.c` + variables `a0Mtx`/`a1Mtx` para la multiplicación de nietos + `ev = m->envVtx` y bucle de partículas con `vB0 = m->pos48[i + 1]`: **893/893** exacto (completa `unit_13490` al 100 %).
- **Cambios:** `src/unit_13490.c` (`inline SceneNode_DrawSingle`, `inline ModelData_SetVariant`, `MdlNodeMd_`, `MdlDraw_`, `ModelData_Draw`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 3572 B.

## Password_Encode (unit_50510, 800 B / 200 instr, frame 0x38) — MATCH exacto — 2026-11-25
- **REA/Ghidra:** Reutilizado análisis previo de sesión 39 (`src/unit_50510.c` bajo `#ifdef NON_MATCHING`, 182/200 palabras; residual de 18 palabras: intercambio `$a2`/`$a3` entre `bi` y `r` y posición de `out[k] = gPasswordChars[...]`).
- **Confirmado en ASM:** En `Password_Encode.s`, cada rama de `if (r < 4)` escribe directamente `D_80123D2C` y `out[k] = gPasswordChars[D_80123D2C]` antes de `k++; r += 5;` (sin variable temporal `v`), y la variable `bi` se reutiliza como contador `0..17` y `0..5` en los dos bucles finales (eliminando `u8 i`).
- **Hipótesis:**
  1. Eliminar `v` y `i`, asignar `D_80123D2C = ... % 32; out[k] = gPasswordChars[D_80123D2C];` dentro de ambas ramas `if (r < 4) / else`, e inicializar `bi = 0; k = 0; r = 0;`: **200/200** exacto.
- **Cambios:** `src/unit_50510.c` (`Password_Encode` promovida fuera de `#ifdef NON_MATCHING`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK, `check_no_asm.py` OK; **objdiff oficial: 89.28 % (493,488 / 552,756 B, 1,826 / 1,910 funciones, 180 / 221 unidades completas)**.
- **Bytes recuperados:** 800 B.

## Reconstrucciones NON_MATCHING congeladas tras pasada de permuter (0 B oficiales)
- **CarSnd_Update (`src/unit_01050.c`, 3296 B):** **819/824** palabras (tamaño exacto 3296/3296 B; guardada bajo `#ifdef NON_MATCHING`; residual: 5 palabras en `0x019C..0x01C8` por `li $v1, 13` en delay slot de `beqz $s7`; permuter 15 s sin llegar a 0).
- **Hud_DrawTurbos (`src/unit_49CF0.c`, 1528 B):** **374/382** palabras (1524/1528 B; guardada bajo `#ifdef NON_MATCHING`; residual: asignación de registros en `D_800F3B18[idx].b = count = turbos - base` y orden de condición de `audSfxPlay`; permuter 15 s sin llegar a 0).
- **Coll_PointsVsWorld (`src/unit_39C90.c`, 5232 B):** **969/1311** palabras (5244/5232 B, frame `0x288` y todos los offsets de pila exactos; guardada bajo `#ifdef NON_MATCHING`; residual: asignación FPR/GPR y reducción de fuerza GIV de `wnorm[k]`; permuter 15 s sin llegar a 0).

## CupResults_SetupTexts (unit_53B30, 1516 B / 379 instr, frame 0xB8) — NON_MATCHING 377/379 — 2026-11-26
- **REA/Ghidra:** `procedure_pseudo_code`, `procedure_callers`, `procedure_callees` (`Cup_GetPosition`, `Menu_SetText`, `Time_Minutes`, `Time_Seconds`, `Time_Hundredths`, `sprintf`); confirmó las dos ramas `gProfileP1->cup == 2` (3 filas de tiempos + 3 filas vacías `3..5`) y `else` (6 filas de puntos con `D_80123F60[i].f2A`).
- **Confirmado en ASM:** `u8 Cup_GetPosition(u8);` evita el `move` intermedio antes de `andi $a1, $s4, 0xFF`; `Time_Minutes`/`Time_Seconds`/`Time_Hundredths` están expandidas `inline`; en la rama `cup == 2`, `y = pos * 0x19 + 0xF0` se pasa inline en el 5.º argumento del primer `Menu_SetText` y se declara `s32 y` dentro de la rama `cup == 2` (`$s2`) y `s32 y2` dentro del cuerpo del bucle `else` (`$s0`, igualando la rama `else` al 100 %); antes del bucle `for (i = 3; i < 6; i++)`, asignar `yBlank = y;` emite `addu $s1, $s2, $zero` y libera `$s2` para la constante `1`.
- **Hipótesis:**
  1. Eliminar local `Profile *p = gProfileP1;` (base `.scratch/s45/cupresults_setuptexts_306.c` 306/380): 314/379 (1512/1516 B).
  2. Pasar `y = pos * 0x19 + 0xF0` inline en el primer `Menu_SetText` + inlines `Time_*`: 318/379.
  3. Acotar `s32 y` al bloque `cup == 2` y `s32 y2` al cuerpo del bucle `else`: 369/379.
  4. Añadir `s32 yBlank = y;` antes del bucle de filas vacías `3..5`: **377/379** (tamaño exacto **1516/1516 B**).
  5. 5 variantes adicionales para el orden de `addu` en `D_80123F60[i].lap[D_80123EC1]` (`0x0094`) y la posición de `addu $s1, $s2, $zero` (`0x02C8`) no subieron de 377/379 → tope de intentos aplicado.
- **Cambios:** `src/unit_53B30.c` (`CupResults_SetupTexts` bajo `#ifdef NON_MATCHING`).
- **Resultado objdiff:** NON_MATCHING (no cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 0 B oficiales.

## Reconstrucciones NON_MATCHING adicionales trabajadas y guardadas en `src/` (0 B oficiales)
- **Coll_SyncObjects (`src/unit_39C90.c`, 1216 B / 304 instr):** **277/304** palabras (1212/1216 B; `REA` confirmó `TransformShape_` inline y cola compartida de `sleepTimer`; 5 variantes sobre el bucle de vértices `for`/`do-while`/`ShapeSync_ *s` no subieron de 277/304; guardada bajo `#ifdef NON_MATCHING`).
- **Audio_Init (`src/unit_01050.c`, 1164 B / 291 instr):** **273/291** palabras (tamaño exacto 1164/1164 B; `REA` confirmó `gAudio` unificado con `songTable[7]`, `sfxTable[61]` y `extra[9][8]`; residual: GCC programa `lui/addiu` de `rom_files_CCCSND_PTR_ROM_START/END` tras el bucle de borrado de `sfxTable`; guardada bajo `#ifdef NON_MATCHING`).
- **Cup_SetupRace (`src/unit_53B30.c`, 508 B / 127 instr):** **96/127** palabras (496/508 B, subido desde 88/127 al usar `D_80123EC1` directo sin `u8 *lap` y `D_80123F60[i].lap[D_80123EC1] = 0`; 5 variantes siguientes no subieron de 96/127; guardada bajo `#ifdef NON_MATCHING`).
- **SndSrc_Play (`src/unit_01050.c`, 420 B / 105 instr):** **96/105** palabras (416/420 B; guardada bajo `#ifdef NON_MATCHING`).
- **Mesh_GetMaterialList (`src/unit_19D40.c`, 392 B / 98 instr):** **91/98** palabras (388/392 B, bucle 1 y cola exactos; residual: `$t0/$t1` intercambiados en bucle 2; guardada bajo `#ifdef NON_MATCHING`).

**Session 51 result:** 5 exact matches (`Hud_ShowTrick` 1984 B, `Garage_SetupTexts` 1388 B, `Coll_BoxVsWorldEdges` 3784 B, `ModelData_Draw` 3572 B, `Password_Encode` 800 B = **+11,528 B**; 1 unit completed: `unit_13490`); **objdiff 89.28 % (493,488 / 552,756 B, 1,826 / 1,910 functions, 180 / 221 complete units)**. No commit (5 exact functions since HEAD `8df6220`, below the ~30-function threshold).

---

## Sesión 52 (2026-11-26)

## GfxPools_Init (gfx_pools, 416 B / 104 instr, frame 0x0) — MATCH exacto — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no está instalado en PATH; justificación técnica); análisis directo del `.s` y de los helpers `inline` ya igualados en `src/gfx_pools.c` (`GfxPool_Init`, `VtxPool_Init`, `MtxPool_Init`).
- **Confirmado en ASM:** `GfxPools_Init` expande `inline` las 6 inicializaciones (`gGfxPools[0..1]` con `0x300`/`D_80134FD0[0..1]`, `gVtxPools[0..1]` con `0x40`/`D_801347C0[0..1]`, `gMtxPools[0..1]` con `0x80`/`D_801480A0[0..1]`), pone `gGfxPoolIndex = 0` y carga `gDisplayListHead`, `gVtxHead`, `gMtxHead` desde `[0].ptr`. El orden de asignaciones en los tres `inline *_Init` coloca `chunkCount = 1` después de `end = pool->chunks[0] + pool->capacity` (antes de `ptr = pool->chunks[0]`), lo que aloja `0x300` en `$a0` y `1` en `$a2`.
- **Hipótesis:**
  1. Con el orden previo de `*_Init` (`chunkCount = 1` en 2.ª línea): 53/104 palabras, tamaño exacto 416/416 B (`1` en `$a0`, `0x300` en `$a1`).
  2. Reordenar en `GfxPool_Init`/`VtxPool_Init`/`MtxPool_Init` a `capacity; chunks[0]; chunks[1] = NULL; end; chunkCount = 1; ptr;`: **104/104** exacto (completa `src/gfx_pools.c` al 100 %).
- **Cambios:** `src/gfx_pools.c` (`GfxPool_Init`, `VtxPool_Init`, `MtxPool_Init`, `GfxPools_Init` reemplazando `INCLUDE_ASM`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, ROM SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` OK, `verify_funcs.py` OK, `check_no_asm.py` OK; **objdiff oficial: 89.35 % (493,904 / 552,756 B, 1,827 / 1,910 funciones, 181 / 221 unidades completas)**.
- **Bytes recuperados:** 416 B.

## Mesh_BuildDisplayList (unit_19D40, 2708 B / 677 instr, frame 0x58) — NON_MATCHING 591/677 — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no instalado en PATH); análisis completo de `asm/nonmatchings/unit_19D40/Mesh_BuildDisplayList.s` y sus 3 tablas de saltos (`jtbl_800FF820`, `jtbl_800FF858`, `jtbl_800FF870`), más funciones hermanas de `src/unit_19D40.c`.
- **Confirmado en ASM:** Switch principal de 13 casos (`0..12`), empaquetado de triángulos `G_TRI1`/`G_TRI2` mediante `pendingTri` (`$t4`) y `pv0/pv1/pv2`, `curTex = p[0]; curTex &= 0xF; curTex += p[1] << 4;` en Case 3, palabras de render/combine en Cases 5/6 según `gLowRes.v == 1` y `mesh->alpha == 0xFF`, `gDPSetPrimColor` con `(mesh->alpha < pa) ? mesh->alpha : pa` en Case 7, y `mode = (p[0] & 0xF) + (p[1] << 4); mode = DecodeGeomMode_(mode); if (flipCull) mode = FlipCull_(mode);` con `f`/`b` precalculados en Cases 8/9.
- **Hipótesis:**
  1. Reconstrucción inicial de los 13 casos: 530/677 (2700/2708 B).
  2. Separar `DecodeGeomMode_` y `FlipCull_` (con `u32 f, b`) + `u32 mode` de ámbito de función reutilizada para `bits` y `mode` + `(mesh->alpha < pa) ? mesh->alpha : pa` en Case 7: **591/677** palabras, **tamaño exacto 2708/2708 B**, **0 diferencias de opcodes/scheduling**; las 86 palabras restantes son exclusivamente rotación `$s0..$s6` (`curTex` recibe `$s6` en GCC 2.7.2 `global.c` por `15 refs / 664 insns` frente a `p+3` con `89 refs / 629 insns`).
- **Cambios:** `src/unit_19D40.c` (`Mesh_BuildDisplayList` guardada bajo `#ifdef NON_MATCHING` con `TODO(match-debt)`).
- **Resultado objdiff:** NON_MATCHING (no cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 0 B oficiales.

## Password_Decode (unit_50510, 1304 B / 326 instr, frame 0x48) — NON_MATCHING 318/326 — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no instalado en PATH); análisis directo de `asm/nonmatchings/unit_50510/Password_Decode.s` y de su hermana ya igualada `Password_Encode`.
- **Confirmado en ASM:** Reutilización de `bi` en bucles de trucos, suma, puesta a cero y decodificación; `static inline void Password_Unscramble_(u8 *p)` fija el orden de registros del intercambio (`$t0`/`$a3`); `Password_FindChar_` + `Password_DecodeChar_` con `d = (u8)v - prev; return (d < 0) ? (d + 32) : d;` y `s32 val` igualan `0x000..0x2B4` y `0x2FC..0x514` al 100 %.
- **Hipótesis:**
  1. Subido desde 239/328 a **318/326** palabras, **tamaño exacto 1304/1304 B**; residual de 8 palabras en `0x2B8..0x2F8` (`bnez` vs `bnel` y `bltzl`/`andi` vs `bltz`/`j`).
- **Cambios:** `src/unit_50510.c` (`Password_Decode` actualizada bajo `#ifdef NON_MATCHING`).
- **Resultado objdiff:** NON_MATCHING (no cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 0 B oficiales.

## n_alFxPull (unit_74AB0, 784 B / 196 instr, frame 0x48) — NON_MATCHING 190/196 — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no instalado en PATH); análisis de `asm/nonmatchings/unit_74AB0/n_alFxPull.s` y headers `include/n_audio.h`, `include/n_abi.h`.
- **Confirmado en ASM:** `0x000..0x2BC` coincide 100 % byte a byte (incluido `prev_outptr = &r->input[d->output]` con la peculiaridad original de Nintendo).
- **Hipótesis:**
  1. Reconstrucción con `aDMEMMove(ptr++, 0x930, 0x7C0, 184 << 1)`: **190/196** palabras, **tamaño exacto 784/784 B**; residual de 6 palabras en `0x2C0..0x2DC` por scheduling del último `aDMEMMove`.
- **Cambios:** `src/unit_74AB0.c` (`n_alFxPull` guardada bajo `#ifdef NON_MATCHING` con `TODO(match-debt)`).
- **Resultado objdiff:** NON_MATCHING (no cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 0 B oficiales.

## Coll_BoxCornersVsWorld (unit_39C90, 6472 B / 1618 instr, frame 0x278) — NON_MATCHING 992/1618 — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no instalado en PATH); análisis de `asm/nonmatchings/unit_39C90/Coll_BoxCornersVsWorld.s` y de su hermana `Coll_PointsVsWorld` en `src/unit_39C90.c`.
- **Confirmado en ASM:** Frame `0x278` y todos los offsets de pila (`0x10..0x208`, spill `0x20C`) exactos; recorre `gCollPathCars`, usa `ShapeBcw_ *shape = &obj->shape`, interpola esquinas `(c0, c2, c3, c1)`, `CHECK_WALL_BCW` con `f32 d` local y bucle `do { ... wnp++; k++; } while (wnp < &wnorm[4]);`.
- **Hipótesis:**
  1. Reconstrucción inicial -> **992/1618** palabras (6408/6472 B); residual: en bucle 2 GCC hoistea `1.0f` a `$s6/$s7` en lugar de `&wnorm[0]` en `$s6` y `4096.0f` en `$fp`; en bucle 1 `pl.n.x` queda en `$f30` en vez de `$a0`.
- **Cambios:** `src/unit_39C90.c` (`Coll_BoxCornersVsWorld` guardada bajo `#ifdef NON_MATCHING` con `TODO(match-debt)`).
- **Resultado objdiff:** NON_MATCHING (no cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 0 B oficiales.

**Session 52 result:** 1 exact match (`GfxPools_Init` 416 B, completing `src/gfx_pools.c` 100%) + 4 high-fidelity `NON_MATCHING` reconstructions/upgrades (`Mesh_BuildDisplayList` 591/677 exact size 2708 B, `Password_Decode` 318/326 exact size 1304 B, `n_alFxPull` 190/196 exact size 784 B, `Coll_BoxCornersVsWorld` 992/1618); **objdiff 89.35 % (493,904 / 552,756 B, 1,827 / 1,910 functions, 181 / 221 complete units)**. No commit (6 exact functions since HEAD `8df6220`, below the ~30-function threshold).

---

## Sesión 53 (2026-11-26)

## Model_SwapPalette (unit_06050, 324 B / 80 instr + align nop, frame 0x8) — MATCH exacto — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no está instalado en PATH; justificación técnica); se reutilizó el análisis REA/Ghidra de sesión 41 (`src/unit_06050.c` bajo `#ifdef NON_MATCHING`, 77/80 palabras).
- **Confirmado en ASM:** En `Model_SwapPalette.s`, `0x54..0x58` carga `lw $v0, 0x34($v0)` (`pals`) en `$v0` y `sll $v1, $t3, 3` (`k << 3`) en `$v1`; en `0xE4..0x104`, los tres desplazamientos `>> 3` se calculan antes de los tres desplazamientos `<< 11`, `<< 6`, `<< 1` y finalmente `lo | (hi | mid) | 1`.
- **Hipótesis:**
  1. `dst = *(u16 **)((u8 *)gModels.models[i]->pals + (k << 3) + 4);`: fija `0x54..0x58` → 79/80 palabras.
  2. Guardar `u32 a = hi << 11; u32 c = mid << 6; u32 d = lo << 1; dst[k] = d | (a | c) | 1;` tras `hi/mid/lo = r/g/b >> 3`: fija `0xF8..0x104` → **80/80** exacto (completa `src/unit_06050.c` al 100 %).
- **Cambios:** `src/unit_06050.c` (`Model_SwapPalette` promovida fuera de `#ifdef NON_MATCHING`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK, `check_no_asm.py` OK.
- **Bytes recuperados:** 324 B.

## Scroller_DrawLine (unit_27A40, 492 B / 123 instr, frame 0x70) — MATCH exacto — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no está instalado en PATH; justificación técnica); análisis directo de `asm/nonmatchings/unit_27A40/Scroller_DrawLine.s` y de su único llamador `Scroller_Draw` en `src/unit_27A40.c`.
- **Confirmado en ASM:** Firma `void Scroller_DrawLine(u8 *str, s32 x0, s32 x1, s32 y, u8 bright)` (5.º arg `u8` en `0x83($sp)`). Inicializa `u32 x = 0; u8 *p = str;` antes del `if (bright != 0)` que fija `u8 r, g, b` a `0xFF` o `0xAA`; dos recorridos `while (p[1] != 0)` acumulando `x += sysFontGetCharWidth(0, *p) + Font_GetKerning(0, *p, p[1]); p++;` (mide y dibuja centrado con `x = (x0 + x1) / 2 - (x >> 1)`).
- **Hipótesis:**
  1. Con `cw` temporal y `x = 0; p = str;` tras `if (bright)`: 74/125 palabras (500/492 B).
  2. Inicializar `u32 x = 0; u8 *p = str;` antes de `if (bright != 0)` y combinar `x += sysFontGetCharWidth(0, *p) + Font_GetKerning(0, *p, p[1]);`: **123/123** exacto (completa `src/unit_27A40.c` al 100 %).
- **Cambios:** `src/unit_27A40.c` (prototipo actualizado y `Scroller_DrawLine` reemplazando `INCLUDE_ASM`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK, `check_no_asm.py` OK.
- **Bytes recuperados:** 492 B.

## PauseUi_DrawText (unit_6AE90, 352 B / 88 instr, frame 0x40) — MATCH exacto — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no está instalado en PATH; justificación técnica); se reutilizó el análisis REA/Ghidra de sesión 41 (`src/unit_6AE90.c` bajo `#ifdef NON_MATCHING`, 73/88 palabras).
- **Confirmado en ASM:** `cx` es `u16`, `w` y `x` son `s16`, `s = text` se asigna antes de `svcDraw2DSetColor(r, g, b)`, y el recorrido de medida está en un helper `static inline s16 PauseUi_MeasureText_(u8 *s)` (que explica `addu $s0, $s3, $zero` seguido de `lbu $v0, 0x0($s3)` en `0x40`).
- **Hipótesis:**
  1. `u16 cx`, `s16 w = 0; s16 x;`, `s = text;` antes de `svcDraw2DSetColor`: 87/88 palabras (solo `lbu $2, 0($s0)` vs `0($s3)` en `0x40`).
  2. Extraer el recorrido de medida a `static inline s16 PauseUi_MeasureText_(u8 *s)`: **88/88** exacto (completa `src/unit_6AE90.c` al 100 %).
- **Cambios:** `src/unit_6AE90.c` (`PauseUi_MeasureText_` + `PauseUi_DrawText` promovida fuera de `#ifdef NON_MATCHING`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK, `check_no_asm.py` OK.
- **Bytes recuperados:** 352 B.

## CarSnd_Update (unit_01050, 3296 B / 824 instr, frame 0xE0) — MATCH exacto — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no está instalado en PATH; justificación técnica); se partió de la reconstrucción previa de sesión 51 (`src/unit_01050.c` bajo `#ifdef NON_MATCHING`, 819/824 palabras, tamaño exacto 3296/3296 B).
- **Confirmado en ASM:** En el bloque `if (p1)` del coche 0 (`0x019C..0x01C8`), el original carga `gCarSnd[i].b45` en `$v0`, aplica `andi $v1, $v0, 0xFF`, carga `13` en `$v0` y hace `beq $v1, $v0`, evitando que `li $v1, 13` suba al delay slot de `beqz $s7`.
- **Hipótesis:**
  1. Asignar `v = gCarSnd[i].b45;` antes del ternario de `pitch` en `if (p1)`: 822/824 palabras (elimina el hoist de `li 13` y el `nop` extra tras `mtc1`, pero `v` vive tras llamadas y usa `$s0` en vez de `$v1`).
  2. Declarar `s32 type;` a nivel de función y usarlo para `gCarSnd[i].b45` en `if (p1)` y `gCarSnd[i].b44` en `if (p0)` (sin cruzar llamadas, por lo que GCC le asigna `$v1`): **824/824** exacto.
- **Cambios:** `src/unit_01050.c` (`CarSnd_Update` promovida fuera de `#ifdef NON_MATCHING`, retirada declaración duplicada de `gListenerPos`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, ROM SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` OK, `verify_funcs.py` OK, `check_no_asm.py` OK; **objdiff oficial: 90.16 % (498,368 / 552,756 B, 1,831 / 1,910 funciones, 184 / 221 unidades completas)**.
- **Bytes recuperados:** 3296 B.

## SortQueue_Sort (unit_20830, 816 B / 204 instr, frame 0xC8) — NON_MATCHING 158/204 — 2026-11-26
- **REA/Ghidra:** No disponible en este entorno (`rea` no está instalado en PATH; justificación técnica); análisis de `asm/nonmatchings/unit_20830/SortQueue_Sort.s` y borrador previo `.scratch/s48/sq_v3.c` (66/204, 808/816 B).
- **Confirmado en ASM:** Quicksort iterativo de Numerical Recipes con corte a insertion sort para `r - l < 7` y pila `s32 stack[50]`. El orden de argumentos de `SORTQ_SWAP` es `(l, (l+r)>>1)`, `(r, l+1)`, `(r, l)`, `(l, l+1)`, `(j, i)`, `(j, l)` con temporales `ta, tb, tk` a nivel de función, y las tres comparaciones de mediana de 3 usan `>` (`key[l+1] > key[r]`, `key[l] > key[r]`, `key[l+1] > key[l]`).
- **Hipótesis:**
  1. Temporales `ta, tb, tk` a nivel de función + orden exacto de `SORTQ_SWAP`: 140/204 palabras (808/816 B).
  2. Invertir las 3 comparaciones de mediana de 3 a `>` y ordenar `i = l + 1; pivot = q->c[l]; j = r;`: **158/204** palabras, **tamaño exacto 816/816 B**; residual: intercambio `$t3/$t4` entre `sp` y `q` más `move $4, $9` en `0x12C`.
- **Cambios:** `src/unit_20830.c` (`SortQueue_Sort` guardada bajo `#ifdef NON_MATCHING` con `TODO(match-debt)`).
- **Resultado objdiff:** NON_MATCHING (no cuenta), `make` OK, `verify_funcs.py` OK.
- **Bytes recuperados:** 0 B oficiales.

**Session 53 result:** 4 exact matches (`Model_SwapPalette` 324 B, `Scroller_DrawLine` 492 B, `PauseUi_DrawText` 352 B, `CarSnd_Update` 3,296 B = **+4,464 B**; 3 units completed: `unit_06050`, `unit_27A40`, `unit_6AE90`) + 1 `NON_MATCHING` reconstruction (`SortQueue_Sort` 158/204, exact size 816 B); **objdiff 90.16 % (498,368 / 552,756 B, 1,831 / 1,910 functions, 184 / 221 complete units) — past 90 %!** No commit (10 exact functions since HEAD `8df6220`, below the ~30-function threshold).


## Gfx_UpdateLights (unit_103B0, 676 B / 169 instr, frame 0x28) — MATCH exacto — 2026-11-26
- **REA/Ghidra:** `rea_query.py Gfx_UpdateLights` (`procedure_pseudo_code`, `procedure_callers`, `procedure_callees`): confirmó copia de 120 B de `D_800F1DE0`, `GFX_RESERVE(gNumLights + 2)`, bucle `gSPLight` × `gNumLights`, `gSPLight` ambient + `gSPNumLights`, y `gLightsDirty = 0`.
- **Confirmado en ASM:** El original mantiene `&gNumLights` en `$s1` durante `GFX_RESERVE` + bucle y lo recarga en `$a1` tras el bucle (comportamiento de `extern u8 gNumLights[]`), calcula `&lights->l[i]` con `addiu +8` antes de `addu $s0`, y emite `sb $zero, gLightsDirty` tras `sw $v0, 0xC($a0)`.
- **Hipótesis:**
  1. `extern u8 gNumLights[]` + eliminar `u8 *n`: 166/169 palabras (676/676 B exacto).
  2. `Lights7 *lights` (`&lights->l[i]`, `&lights->a`): 167/169 palabras (solo `sb $zero, gLightsDirty` subido antes de `sw`).
  3. `((LightFlag_ *)&gLightsDirty)->v = 0` (`MEM_IN_STRUCT_P = 1` bloquea el hoist sin cambiar `extern u8 gLightsDirty;` en `Light_SetColor`): **169/169** exacto (completa `src/unit_103B0.c` al 100 %).
- **Cambios:** `src/unit_103B0.c` (`extern u8 gNumLights[]`, `Gfx_UpdateLights` promovida fuera de `#ifdef NON_MATCHING`, retirado `#include "include_asm.h"`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, `verify_funcs.py` OK, `check_no_asm.py` OK.
- **Bytes recuperados:** 676 B.

## Coll_LoadLevel (unit_39C90, 636 B / 159 instr, frame 0x138) — MATCH exacto — 2026-11-26
- **REA/Ghidra:** Omitido (borrador `NON_MATCHING` previo a <10 palabras: 155/159 palabras, solo faltaba un par `lui/lw` de `gCollLevel` en `0x88..0x8C`).
- **Confirmado en ASM:** En `0x80099110`, el original carga `gCollLevel` en `$a0` y vuelve a cargarlo en `$v1` antes de `addiu $v1, $v1, 0xD8; sw $v1, 0xC0($a0)`.
- **Hipótesis:**
  1. Quitar `mem`, puntero `u8 *ptr` local, `Hdr90_ *h` local: sin cambio (155/159; CSE fusiona ambos `lw`).
  2. Cast `((CollPtr_ *)&gCollLevel)->p + 0xD8` (`MEM_IN_STRUCT_P = 1` en el RHS impide que CSE fusione las dos cargas de `gCollLevel`): **159/159** exacto.
- **Cambios:** `src/unit_39C90.c` (`Coll_LoadLevel` promovida fuera de `#ifdef NON_MATCHING` con `TODO(match-debt)`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, ROM SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` OK, `verify_funcs.py` OK, `check_no_asm.py` OK; **objdiff oficial: 90.40 % (499,680 / 552,756 B, 1,833 / 1,910 funciones, 185 / 221 unidades completas)**.
- **Bytes recuperados:** 636 B.

**Session 54 result:** 2 exact matches (`Gfx_UpdateLights` 676 B, `Coll_LoadLevel` 636 B = **+1,312 B**; 1 unit completed: `unit_103B0`); **objdiff 90.40 % (499,680 / 552,756 B, 1,833 / 1,910 functions, 185 / 221 complete units)**. 12 exact functions gained since HEAD `8df6220` (no commit executed).

---

## Sesión 55 (2026-11-26)

## Audio_Init (unit_01050, 1164 B / 291 instr, frame 0x18) — MATCH exacto — 2026-11-26
- **REA/Ghidra:** Reutilizado análisis previo (`gAudio` unificado + `D_800EFD88[9][8]`).
- **Confirmado en ASM:** Separar `D_800EFD88[9][8]` de `AudioGlobals_` (`#define gAudio (*(AudioGlobals_ *)&gMusicHandle)` para no alterar `Music_PlayTrack`), barrera `do { } while (0);` tras `sfxTable[60]`, locales `ptrStart`/`ptrEnd` con `rom_files_CCCSND_PTR_ROM_START`/`END` y `rom_files_CCCSND_WBK_ROM_START`, y punteros `p` en ambos bucles de puesta a cero (`for (i = 6, p = &gAudio.songTable[6]; ...)` y `for (i = 60, p = &st[67]; ...)`): **291/291 exacto**.
- **Cambios:** `src/unit_01050.c` (`Audio_Init` promovida fuera de `#ifdef NON_MATCHING`).
- **Resultado objdiff:** MATCH exacto (cuenta), `make` OK, ROM SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` OK, `verify_funcs.py` OK, `check_no_asm.py` OK.
- **Bytes recuperados:** 1,164 B.

## Mejoras NON_MATCHING guardadas en `src/` (0 B oficiales)
- **CupResults_SetupTexts (`src/unit_53B30.c`, 1516 B / 379 instr):** **378/379** (subido desde 377/379, tamaño exacto 1516 B; `rea_query.py` + `pos * 0x19 + 0xF0` en el bucle `3..5` elimina `yBlank` y cierra toda la segunda mitad; solo resta `addu $v0, $v0, $t0` vs `$v1` en `0x0094`).
- **n_alFxPull (`src/unit_74AB0.c`, 784 B / 196 instr):** **195/196** (subido desde 190/196, tamaño exacto 784 B; `rea_query.py` + `w1` precalculado antes de `do { } while (0);`; solo resta la posición de `addiu $v0, $t0, 0x8`).
- **SndSrc_Play (`src/unit_01050.c`, 420 B / 105 instr):** **100/105** (subido desde 96/105, tamaño exacto 420 B; struct `AudioBlock_` cierra `0x000..0x0F4` al 100 %; residual de 5 palabras en `Snd_IsPlaying`).
- **SortQueue_Sort (`src/unit_20830.c`, 816 B / 204 instr):** **179/204** (subido desde 158/204, tamaño exacto 816 B; `sp += 2` en ambas ramas resuelve el intercambio `$t3/$t4`).

**Session 55 result:** 1 exact match (`Audio_Init` 1,164 B = **+1,164 B**) + 4 `NON_MATCHING` upgrades (`CupResults_SetupTexts` 378/379, `n_alFxPull` 195/196, `SndSrc_Play` 100/105, `SortQueue_Sort` 179/204); **objdiff 90.61 % (500,844 / 552,756 B, 1,834 / 1,910 functions, 185 / 221 complete units) — past 500 KB!** 13 exact functions gained since HEAD `8df6220` (no commit executed).

---

## Sesión 56 (2026-11-26)

Nota de entorno: `rea` no está disponible en `PATH` en esta sesión (`prepare_elf.py` OK, análisis basado en ASM + código hermano en `src/`).

## CupResults_SetupTexts (unit_53B30, 1516 B / 379 instr, frame 0xB8) — NON_MATCHING 378/379 — 2026-11-26
- **Hipótesis probadas:** `volatile u8 *lap`, `CupRec_ *recs` local (como `Race_ReturnCup`, que corrige el orden de `addu $v0, $v0, $t0` pero rota `$t0/$t1/$t2` y añade 1 instr en el bucle), y variantes de indexación `((s32 (*)[11])D_80123F60)[i][lap]`.
- **Resultado:** Sin mejora sobre 378/379 (sin cambios en `src/unit_53B30.c`).
- **Bytes recuperados:** 0 B oficiales.

## Password_Decode (unit_50510, 1304 B / 326 instr, frame 0x58) — NON_MATCHING 318/326 — 2026-11-26
- **Hipótesis probadas:** `s32`/`u8` en `Password_DecodeChar_` y `Password_FindChar_`, `if/else` vs ternario, `u8 val` vs `s32 val`, y `u8 v` para mover `andi 0xFF` tras `sb`.
- **Resultado:** Sin mejora sobre 318/326 (sin cambios en `src/unit_50510.c`).
- **Bytes recuperados:** 0 B oficiales.

## Coll_ProbeWheels (unit_39C90, 4244 B / 1061 instr, frame 0x400) — NON_MATCHING 835/1061 — 2026-11-26
- **Confirmado en ASM:** Reconstrucción completa de 4 pasadas sobre `gCollRacers` (inicialización de límites/radios por rueda, salas curvas `flags & 0x800` con interpolación bilineal + normalización `sqrtf`, salas planas con portales filtrados y aplicación final de fuerza/contacto con `D_800F38E4`/`D_801009F0`).
- **Resultado:** **835/1061** palabras (**4240/4244 B**, **1023/1061 opcode-only**; Pasada 1: 87/90, Pasada 4: 162/164, Pasada 3: 203/246). Residual: marco `0x3F0` vs `0x400` porque GCC retiene `($sp+0x10)+id*24` en `$s7` en lugar de hacer spill a `0x394($sp)`, desplazando `nWheels`/`roomIdx` a `$t8` en las 4 pasadas y omitiendo el hoist de `$sp+0x10`/`id*12` fuera de los bucles externos de Pasada 2.
- **Cambios:** `src/unit_39C90.c` (`Coll_ProbeWheels` guardada bajo `#ifdef NON_MATCHING` con `TODO(match-debt)`).
- **Bytes recuperados:** 0 B oficiales.

**Session 56 result:** 0 exact matches (+0 B; detenida por criterio de 3 funciones seguidas congeladas como `NON_MATCHING` sin ganar bytes) + 1 nueva reconstrucción grande `NON_MATCHING` (`Coll_ProbeWheels` 835/1061, 4240/4244 B, 1023/1061 opcode-only); **objdiff 90.61 % (500,844 / 552,756 B, 1,834 / 1,910 functions, 185 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

---

## Sesión 57 (2026-11-26)

Nota de entorno: `rea` no está disponible en `PATH` en esta sesión (análisis basado en ASM + código hermano en `src/`).

## SndSrc_Play (unit_01050, 420 B / 105 instr, frame 0x48) — NON_MATCHING 100/105 (1 instr residual) — 2026-11-26
- **Confirmado en ASM:** Cambiar `if (s != NULL) { ... if (busy == 0) { ... return 1; } } return 0;` por `if (s != NULL) { ... if (busy != 0) return 0; } else { return 0; }` seguida de `Snd_Play`/`Snd_SetPitch`/`return 1;` reproduce el 100 % del flujo de control (`0xF8: beqz $s0, 0x150`, bloque `return 0` en `0x150`, `return 1` con fallthrough en `0x180`).
- **Residual:** Solo falta `addu $a0, $v0, $zero` antes de `beqz $a0` en `0x144` (comprobado 105/105 con asignación de diagnóstico a `$a0`; variantes en C puro reutilizando `h`, `id`, `busy`, `ret` y `switch` mantuvieron `beqz $v0`).
- **Cambios:** `src/unit_01050.c` (`SndSrc_Play` actualizada bajo `#ifdef NON_MATCHING` con `TODO(match-debt)`).
- **Bytes recuperados:** 0 B oficiales.

## Race_ReadPlayerControls (unit_2C6A0, 844 B / 211 instr, frame 0x30) — NON_MATCHING 194/211 — 2026-11-26
- **Hipótesis probadas:** `a`/`b` inline, `static inline` `LerpClamped`/`Clamp01`, `goto done`, ternarios y anidado `if (!(t < 0.0f)) { if (!(t > 1.0f)) ... }` (corrige el 2.º `bc1tl` en `0x168`, pero el 3.º queda en `bc1fl` y `1.0f` sigue en `$f6` en vez de `$f0`).
- **Resultado:** Sin mejora sobre 194/211 (sin cambios en `src/unit_2C6A0.c`).
- **Bytes recuperados:** 0 B oficiales.

## n_alFxPull (unit_74AB0, 784 B / 196 instr, frame 0x48) — NON_MATCHING 195/196 — 2026-11-26
- **Hipótesis probadas:** `aDMEMMove`, `static inline`, `ptr++` separado, barreras `for (;0;)`/`do {} while (0)` y reordenación `w0`/`w1`.
- **Resultado:** Sin mejora sobre 195/196 (sin cambios en `src/unit_74AB0.c`).
- **Bytes recuperados:** 0 B oficiales.

## Mesh_GetMaterialList (unit_19D40, 392 B / 98 instr, frame 0x0) — NON_MATCHING 91/98 — 2026-11-26
- **Hipótesis probadas:** `u32 *dst = &out[count]` explícito (`for` y `do-while`), `u32 m` compartido entre ambos bucles, tipos `u16`/`s32` y variantes de incremento `out[count] = m2; count++`.
- **Resultado:** Sin mejora sobre 91/98 (sin cambios en `src/unit_19D40.c`).
- **Bytes recuperados:** 0 B oficiales.

**Session 57 result:** 0 exact matches (+0 B) + 1 mejora estructural `NON_MATCHING` (`SndSrc_Play` reducida a 1 instrucción residual); **objdiff 90.61 % (500,844 / 552,756 B, 1,834 / 1,910 functions, 185 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

---

## Sesión 58 (2026-11-26)
- **Nota REA/Ghidra:** `prepare_elf.py` OK, pero binario `rea` no disponible en `PATH`; análisis realizado directamente sobre ASM y contexto de `src/`.

## SndSrc_Play (unit_01050, 420 B / 105 instr, frame 0x48) — MATCH exacto — 2026-11-26
- **Hipótesis ganadora:** Al igual que las funciones hermanas `CarSnd_PlayLoop*`, el código original calcula el volumen entero muerto `s32 v = s->v * 128.0f * vol; if (v == 0) v = 1;` entre `busy = Snd_IsPlaying(s->f10)` y `if (busy != 0) return 0;`, lo que fuerza `addu $a0, $v0, $zero` antes de `beqz $a0`. Declarar `extern AudioBlock_ D_800EFD88;` en `src/unit_01050.c` resuelve el direccionamiento real `-0x378` desde `gSndSrcList` y cierra la unidad `unit_01050.c` al 100 %.
- **Bytes recuperados:** +420 B.

## CupResults_SetupTexts (unit_53B30, 1516 B / 379 instr, frame 0xB8) — MATCH exacto — 2026-11-26
- **Hipótesis ganadora:** Helper `static inline u8 *Cup_AddOff_(u8 *b, u32 o)` impide que `fold()` reasocie `(lap * 4 + D_80123F60) + i * sizeof(CupRec_)`, emitiendo `addu $v0, $v0, $t0` antes de `addu $v0, $v0, $v1`; las locales `s32 *d = D_800F60E8; u8 *lap = &D_80123EC1;` tras `gProfileP1->cupPoints[i]` fijan el orden de hoisting (`$t2`, `$t1`, `$t0`). Corregidas las firmas reales de `Time_Seconds`/`Time_Hundredths` (`u16`) y `Cup_GetPosition` (`u8`).
- **Bytes recuperados:** +1,516 B.

## Hud_DrawTurbos (unit_49CF0, 1528 B / 382 instr, frame 0x68) — NON_MATCHING 382/382 enmascaradas (377/382 exactas) — 2026-11-26
- **Hipótesis ganadora parcial:** `count = (D_800F3B18[idx].b = turbos - base)` con `reset` duplicado en la segunda rama cruza el umbral `floor_log2(15 -> 16)` de `global.c`, restaurando `$s4..$s7` exactos y `1528/1528 B` (382/382 instrucciones/registros idénticos).
- **Residual:** Los 5 accesos al spill de `turbos` quedan en `0x37($sp)` en vez de `0x1F($sp)` porque `u8 buf[24]` se asigna en `expand` antes que el spill de `reload`. Actualizada bajo `#ifdef NON_MATCHING`.
- **Bytes recuperados:** 0 B oficiales.

## n_alFxPull (unit_74AB0, 784 B / 196 instr) — NON_MATCHING 195/196 — 2026-11-26
- **Hipótesis probadas:** `aDMEMMove`, `-O3`, pre/post-incremento de `ptr`, `do {} while (0)` y reordenación de `w0`/`w1`; sin mejora sobre 195/196 (sin cambios en `src/unit_74AB0.c`).
- **Bytes recuperados:** 0 B oficiales.

## Hud_DrawSlideFromLeft (unit_49CF0, 452 B / 113 instr) — NON_MATCHING 84/113 — 2026-11-26
- **Hipótesis probadas:** eliminar `w`, `do-while` (invierte `$s1/$s2` al orden objetivo pero sube a 476 B), condición combinada en `while` y `e->x += e->h8`; sin mejora neta (sin cambios en `src/unit_49CF0.c`).
- **Bytes recuperados:** 0 B oficiales.

**Session 58 result:** 2 exact matches (`SndSrc_Play` 420 B, `CupResults_SetupTexts` 1,516 B = **+1,936 B**; 1 unit completed: `unit_01050`) + mejora `NON_MATCHING` en `Hud_DrawTurbos` (382/382 enmascaradas, 377/382 exactas); **objdiff 90.96 % (502,780 / 552,756 B, 1,836 / 1,910 functions, 186 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 59 (2026-11-26)

## Cup_SetupRace (unit_53B30, 508 B / 127 instr, frame 0x38) — NON_MATCHING 126/127 (mejorada desde 96/127) — 2026-11-26
- **REA:** `procedure_pseudo_code` confirmó el acceso por dirección base `&DAT_80123f8c` / `&DAT_80123fb8` + `D_80123EC1 * 4` en ambas ramas de `cup == 2`, el bucle `do/while` con puntero `pcVar4` sobre `used[r]` y la limpieza `&D_80123F60 + i * 0x2c + D_80123EC1 * 4`.
- **Hipótesis ganadora parcial:** Acceder a `D_80123EC1` como miembro de struct (`((CupRace_ *)&D_80123EC1)->race`, `MEM_IN_STRUCT_P = 1`) fuerza el reload de `$a0` tras `sw $zero`, y duplicar `D_80123F60[2].lap[...] = 0` en ambas ramas iguala el 100 % de `cup == 2`; en `else`, `ofs = i * sizeof(CupRec_)` + `CupSetup_AddOff_` iguala 126/127 (508/508 B exactos).
- **Residual:** 1 posición de scheduling (`subu $v1, $v1, $a0` en `0x1A8` vs `0x1B0` tras `lbu D_80123EC1`); `decomp-permuter` 25 s sin llegar a 0. Actualizada bajo `#ifdef NON_MATCHING` en `src/unit_53B30.c`.
- **Bytes recuperados:** 0 B oficiales.

## Hud_DrawTurbos (unit_49CF0, 1528 B / 382 instr, frame 0x68) — NON_MATCHING 382/382 enmascaradas (377/382 exactas) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Hud_DrawPlayer`), `callees` (`Car_Get`, `RaceClock_Get`); confirmó estructura de ambos bucles, `D_800F3B3E[gHudPlayerCount][i]` y `D_800F3B5C` stride 3 (sin aporte adicional: residual de offset de pila).
- **Diagnóstico RTL (`-dl -dg`):** Sin `u8 buf[24]`, `turbos` (reg 82) ya cae en `0x1F($sp)` con `vars = 16` (`0x18..0x1F` + reg 117 `ST_REGS` en `0x24`), pero cualquier array/struct en `expand` se asigna antes que `reload` y empuja `0x1F` a `0x37`; structs/arrays para `turbos` caen a 342/382. Sin cambios en `src/unit_49CF0.c`.
- **Bytes recuperados:** 0 B oficiales.

## Hud_DrawSlideFromLeft / Hud_DrawSlideFromRight (unit_49CF0, 452/440 B) — NON_MATCHING 84/113 y 80/110 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Hud_DrawTrickPopup`), `callees` (`Font_GetKerning`); confirmó tipos y bucle compartido (sin aporte adicional: residual de asignación `$s1/$s2`).
- **Diagnóstico RTL (`-dl -dg`):** En `global.c`, `e` (24 refs / 86 insns, prioridad `1.116`) vence a `s` (14 refs / 48 insns, prioridad `0.875`). Dividir `e` tras el bucle invierte `$s1/$s2` al orden objetivo (89/114, 456/452 B); `while` combinado / `for` / `static inline` del bucle no alteran el orden. Sin cambios en `src/unit_49CF0.c`.
- **Bytes recuperados:** 0 B oficiales.

## Password_Decode (unit_50510, 1304 B / 326 instr, frame 0x50) — NON_MATCHING 318/326 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Password_Apply`), `callees` (`strcmp`); confirmó copia de 20 B en pila, bucles de trucos/checksum y decodificación de 5 bits (sin aporte adicional: residual de 8 palabras en `0x2B8..0x2F8`).
- **Hipótesis probadas:** Rejilla de 20 combinaciones de tipos (`u8`/`s32`) y formas de retorno (`if/else`, ternario, `goto`, bloque inline) en `Password_FindChar_`/`Password_DecodeChar_` + `decomp-permuter` 35 s; `s32` en `Password_FindChar_` + `u8 v` mueve `sb` antes de `subu` y fusiona `andi $a3, $v0, 0xFF`, pero sigue emitiendo `bltzl` y `bnez` (315/326). Sin cambios en `src/unit_50510.c`.
- **Bytes recuperados:** 0 B oficiales.

## Mesh_BuildDisplayList (unit_19D40, 2708 B / 677 instr, frame 0x58) — NON_MATCHING 591/677 — 2026-11-26
- **REA:** `procedure_pseudo_code` confirmó los 13 casos del `switch (*p >> 4)` y el uso exclusivo de `curTex` en `case 3` y `case 12` (sin aporte adicional: residual puro de rotación `$s0..$s6`).
- **Diagnóstico RTL (`-dl -dg`):** `p+3`..`mesh` siguen el orden exacto de prioridad (`89, 82, 23, 23, 19, 17, 14`), mientras `curTex` (reg 78, 15 refs / 663 insns) recibe `$s6` en lugar de `$s0`; `register` y orden de declaración no cambian su prioridad. Sin cambios en `src/unit_19D40.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 59 result:** 0 exact matches + mejora `NON_MATCHING` importante en `Cup_SetupRace` (`96/127` → `126/127`, `508/508 B` exactos); **objdiff 90.96 % (502,780 / 552,756 B, 1,836 / 1,910 functions, 186 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 60 (2026-11-26)

## SortQueue_Sort (unit_20830, 816 B / 204 instr, frame 0xC8) — MATCH exacto — 2026-11-26
- **REA:** `procedure_pseudo_code` confirmó los offsets de `SortQueue_` (`+0x10`, `+0x970`, `+0x12D0`), los 3 campos intercambiados, la partición `M = 7` y la pila local `local_c8` con `piVar14 = piVar13 + 2` antes del `if`.
- **Hipótesis ganadora:** Reemplazar `do { ... } while (0)` por bloque `{ ... }` en `#define SORTQ_SWAP(i, j)` elimina las notas `NOTE_INSN_LOOP_BEG`/`END` que en GCC 2.7.2 forzaban la copia `move $4, $9` (`&q->a[l]`) y bloqueaban `addiu $t0, -1` en insertion sort; subir `sp += 2;` antes del `if (r - i + 1 >= j - l)` y ordenar `i = l + 1; j = r; pivot = q->c[l];` cierra el **204/204 exacto** y completa `src/unit_20830.c` al 100 %.
- **Bytes recuperados:** +816 B.

## Cup_SetupRace (unit_53B30, 508 B / 127 instr, frame 0x38) — NON_MATCHING 126/127 — 2026-11-26
- **REA:** `procedure_pseudo_code` confirmó la estructura de `cup == 2`, `local_28`/`local_20` y el bucle `else` con `&D_80123F60 + uVar2 * 0x2c + D_80123EC1 * 4` (sin aporte adicional: residual de 1 posición de scheduling).
- **Hipótesis probadas:** Variantes de offset en palabras (`i * 11` + `<< 2`), orden de argumentos en `CupSetup_AddOff_`, casts `(s32)`/`(u32)` y `decomp-permuter` 30 s; el residual `subu $v1, $v1, $a0` en `0x1A8` vs `0x1B0` persiste (sin cambios en `src/unit_53B30.c`).
- **Bytes recuperados:** 0 B oficiales.

## Hud_DrawTurbos (unit_49CF0, 1528 B / 382 instr, frame 0x68) — NON_MATCHING 382/382 enmascaradas (377/382 exactas) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Hud_DrawPlayer`), `callees` (`Car_Get`, `RaceClock_Get`); confirmó estructura y tablas (sin aporte adicional: residual de offset de pila).
- **Diagnóstico RTL (`-dl -dg`):** Comprobado que `u8 buf[1]` mantiene `vars = 16` y `0x1F($sp)`, mientras cualquier `buf[>=8]` (incluso en bloques internos/finales) desplaza `turbos` a `0x1F + align8(N)`; `reg 117` (`ST_REGS`, `(use (reg:SI 117))`) explica `vars = 16` sin `u8 buf[24]`, pero no los +24 B hasta `vars = 40`. Sin cambios en `src/unit_49CF0.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 60 result:** 1 exact match (`SortQueue_Sort` **+816 B**, cierra `src/unit_20830.c` al 100 %); **objdiff 91.11 % (503,596 / 552,756 B, 1,837 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 61 (2026-11-26)

## Save_ReadAll (unit_50510, 776 B / 194 instr, frame 0x48) — MATCH exacto — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Save_Load`, `Save_CreateNew`, `LoadGame_Enter`), `callees` (`Memory_Alloc`, `Memory_Free`); confirmó los offsets de `gSaveSlots` (`0x500` / `0x140`), la escritura del `id` en `+0x00` tras la copia de bloque y el acceso a `sysSaveRead`/`sysPakGetState` en el bucle externo.
- **Hipótesis ganadora:** Tipar `E140_` con `s32 id; u8 d[0x13C];` y escribir `gSaveSlots[i][j].id = gSaveIds[i];` (en lugar de `*(s32 *)&gSaveSlots[i][j]`) evita que `loop.c` suba `&gSaveSlots` a `$fp`, liberando `$fp` para `&sysSaveRead` (`0x0($fp)` y `-0x4($fp)`); junto con `for (k = 0, n = 0xD90; k < n; k++)` (como en `Save_CreateOnPak`) cierra el **194/194 exacto**.
- **Bytes recuperados:** +776 B.

## Cup_SetupRace (unit_53B30, 508 B / 127 instr, frame 0x38) — NON_MATCHING 126/127 — 2026-11-26
- **REA:** `procedure_pseudo_code` confirmó `local_28`/`local_20` y el bucle `else` con `&D_80123F60 + uVar2 * 0x2c + D_80123EC1 * 4` (sin aporte adicional: residual de 1 posición de scheduling).
- **Hipótesis probadas:** Variantes de `ofs`, `D_80123EC1` plano/volatile, `do { } while (0)`, `do-while`, `register` y `decomp-permuter` 25 s; persiste `subu $v1, $v1, $a0` en `0x1B0` vs `0x1A8`. Sin cambios en `src/unit_53B30.c`.
- **Bytes recuperados:** 0 B oficiales.

## Hud_DrawTurbos (unit_49CF0, 1528 B / 382 instr, frame 0x68) — NON_MATCHING 382/382 enmascaradas (377/382 exactas) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Hud_DrawPlayer`), `callees` (`Car_Get`, `RaceClock_Get`); sin aporte adicional (residual puro de offset de pila).
- **Diagnóstico:** Confirmado que ya alcanzó el límite de sesiones sin avance sobre el desplazamiento `0x37($sp)` vs `0x1F($sp)` de `turbos`; se congela en `NON_MATCHING` sin cambios en `src/unit_49CF0.c`.
- **Bytes recuperados:** 0 B oficiales.

## Coll_SyncObjects (unit_39C90, 1216 B / 304 instr, frame 0x30) — NON_MATCHING 277/304 — 2026-11-26
- **REA:** `procedure_pseudo_code` y `callers` (`Coll_Update`); confirmó las dos expansiones de transformación de vértices (rama `!b->asleep` y rama `type == 2`), `m[12]` en pila y el bucle `do { ... } while (bVar9 < *(byte *)(o + 0x48))`.
- **Hipótesis probadas:** Reordenación de `src`/`dst`/`i` antes y dentro de `for`, macros, helpers en línea, `Mat34_`, `if/do-while` y permutación de declaraciones; cargar `src`/`dst` antes de `m[0..11]` iguala el tamaño (`1216/1216 B` con `nop` en `0x4C`) pero desplaza los loads a `0x78`/`0x80` (274/304). Sin cambios en `src/unit_39C90.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 61 result:** 1 exact match (`Save_ReadAll` **+776 B** en `src/unit_50510.c`); **objdiff 91.25 % (504,372 / 552,756 B, 1,838 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 62 (2026-11-26)

## Mesh_GetMaterialList (unit_19D40, 392 B / 98 instr, frame 0x0) — MATCH exacto — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Fx_ShatterMesh`), `callees` (`[]`); confirmó la estructura del `switch (*p >> 4)` (casos `0..12`), el bucle `sub->count` y el relleno final de `out[count..max-1]`.
- **Hipótesis ganadora:** Unificar `u32 m;` a ámbito de función (compartido entre ambos bucles) y separar cada `case 0:`..`case 12:` con su propio `p += ...; break;` (antes de que `jump2` fusione las colas idénticas) sube las referencias de `p` por encima de `m` en `global-alloc`, asignando `$t0` a `p`, `$t1` a `m` en ambos bucles y `$t0` a `out + count` en el bucle de submesh (**98/98 exacto**; `SubMeshMat_` evita colisión con `SubMesh_` en `unit_19D40.c`).
- **Bytes recuperados:** +392 B.

## Coll_SyncObjects (unit_39C90, 1216 B / 304 instr, frame 0x30) — MATCH exacto — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Update`), `callees` (`[]`); confirmó las dos expansiones en línea de transformación de vértices (rama `!b->asleep` y rama `type == 2`), `m[12]` en pila y el acceso `*(byte *)(o + 0x48)`.
- **Hipótesis ganadora:** Reemplazar `static inline void TransformShape_` por macro `#define TRANSFORM_SHAPE_(sp) { ... }` **sin** `do { ... } while (0)` (evita `NOTE_INSN_LOOP_BEG`/`END` en GCC 2.7.2) y compartir `s`, `m[12]`, `i`, `src`, `dst` a ámbito de función fija `sltu $v0, $v1, $v0`, el `nop` en `0x4C` y toda la asignación `$a2/$a3/$t0/$t1/$t2/$t3` en ambos bucles (**304/304 exacto**).
- **Bytes recuperados:** +1,216 B.

## Race_ReadPlayerControls (unit_2C6A0, 844 B / 211 instr, frame 0x30) — NON_MATCHING 194/211 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Race_Update`), `callees` (`Car_Get`, `RaceCam_SetFlag4A`, `Hud_SetVisible`); confirmó el doble chequeo `< 0.0f` en el clamp y la fórmula `((t * (D_80138D72 - D_80138D41) + D_80138D41) / 100.0f) * yaw`.
- **Hipótesis probadas:** Reordenación de `a`/`b`, ternarios, `if/else if`, `goto`, `static inline LerpClamp`/`Clamp`, `c->yaw *= ...` y `decomp-permuter` (25 s); `greg` muestra que `b` (`reg 108`) recibe `preferences: 32` (`$f0`) desde `sub.s`, desplazando `1.0f` a `$f6`, y la segunda rama del clamp no se convierte a `bc1tl`. Sin cambios en `src/unit_2C6A0.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 62 result:** 2 exact matches (`Mesh_GetMaterialList` **+392 B**, `Coll_SyncObjects` **+1,216 B**; **+1,608 B**); **objdiff 91.54 % (505,980 / 552,756 B, 1,840 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 63 (2026-11-26)

## Coll_ProbeWheels (unit_39C90, 4244 B / 1061 instr, frame 0x400) — NON_MATCHING 887/1061 (mejorada desde 835/1061) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Solve`), `callees` (`[]`); confirmó `p0 = q0*t + q2*s`, el layout exacto de pila (`local_3f0[216]`, `afStack_318..afStack_168`) y el desplazamiento `local_6c = local_3f0 + id*0x18` para `wheelRad` (`+600`).
- **Hipótesis probadas:** Cambiar `p0` a `q0*t + q2*s` y `p1` a `q3*s + q1*t` sube el match de `835/1061` a **887/1061** (+52 palabras, `0x200..0x3F0` y `0x548..0xA20` estructuralmente exactos). El volcado `-dL` (`loop.c`) confirma la causa raíz del residual restante (frame `0x3F0` vs `0x400`): en el bucle `rn` (399 insns) y el bucle `node` (416 insns) de Pass 2, `loop.c` agota `threshold` exactamente 1 instrucción antes de hoistear `id*12` (`regno 568`) y `$sp+0x10` (`regno 1196`), reteniendo `($sp+0x10)+id*24` en `$s7` en vez de hacer spill a `0x394($sp)` y liberar `$s7` como registro de reload en las 4 pasadas.
- **Bytes recuperados:** 0 B oficiales.

## Mesh_BuildDisplayList (unit_19D40, 2708 B / 677 instr, frame 0x58) — NON_MATCHING 591/677 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`SceneNode_PrepareMaterial`, `SceneNode_DrawMesh`, `ModelData_SetVariant`, `ModelData_Draw`), `callees` (`Gfx_LoadTexture`); confirmó los 13 casos y el uso de `curTex` (`iVar10`) solo en `case 3` y `case 12`.
- **Diagnóstico:** Remapeando `$s0..$s6` se confirma que las 677/677 instrucciones son idénticas al 100 % salvo la rotación de `curTex` (`$s0` en objetivo vs `$s6` en GCC 2.7.2, `15 refs / 706 insns` frente a `p+3` con `89 refs / 684 insns`); variantes de tipo (`u16`/`s16`/`s32`), `register`, expresión de `case 3` y tipo de bucle no movieron `curTex` a `$s0`. Sin cambios en `src/unit_19D40.c`.
- **Bytes recuperados:** 0 B oficiales.

## Hud_DrawSlideFromLeft / Hud_DrawSlideFromRight (unit_49CF0, 452 B / 440 B, frame 0x40) — NON_MATCHING 84/113 y 80/110 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Hud_DrawTrickPopup`), `callees` (`Font_GetKerning`); confirmó exactamente el flujo del bucle, las llamadas indirectas `svcFontDrawChar`/`svcFontGetCharWidth` y el ajuste final de `e->x`.
- **Diagnóstico:** `-df`/`-dg` confirma que en `global_alloc` `e` (`reg 73`, 24 refs / 126 insns) supera a `s` (`reg 72`, 14 refs / 71 insns), asignando `$s1` a `e` y `$s2` a `s` al revés que el objetivo (y el frame extra de 8 B corresponde a `0x18..0x1F($sp)` sin accesos). Sin cambios en `src/unit_49CF0.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 63 result:** 0 exact matches, mejora estructural `NON_MATCHING` en `Coll_ProbeWheels` (`835/1061` → **887/1061**, +52 palabras); **objdiff 91.54 % (505,980 / 552,756 B, 1,840 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 64 (2026-11-26)

## Hud_DrawSlideFromLeft (unit_49CF0, 452 B / 113 instr, frame 0x40) — MATCH exacto — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Hud_DrawTrickPopup`), `callees` (`Font_GetKerning`); confirmó el bucle sobre `*s`, las dos ramas hacia `svcFontGetCharWidth` (`LAB_800aaad8`), `svcFontDrawChar` y el ajuste final de `e->x`.
- **Hipótesis ganadora:** Desdoblar `if ((s16)x >= e->x0 && (u8)c % mod == 0)` en `if` anidados con `svcFontGetCharWidth(0, *s)` en ambos `else` aporta a `s` la referencia extra necesaria antes de `jump2` para ganar `$s1` frente a `e` (`$s2`); copiar `u8 *s = arg0; u8 c = cnt;` fija el prólogo (`sw $s2`/`$s1` y `addu $s3, $a3, $zero` en `0x2C`); `-e->gap + Font_GetKerning(...)` + `while (0) {}` cierra el orden `lbu 0xE($s2)`/`subu`/`lbu 0($s1)` (**113/113 exacto**).
- **Bytes recuperados:** +452 B.

## Hud_DrawSlideFromRight (unit_49CF0, 440 B / 110 instr, frame 0x40) — MATCH exacto — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Hud_DrawTrickPopup`), `callees` (`Font_GetKerning`); confirmó estructura de bucle idéntica a `Hud_DrawSlideFromLeft` y ajuste final `e->x = ... - ...`.
- **Hipótesis ganadora:** Mismo patrón que `Hud_DrawSlideFromLeft`: `if` anidado con doble `svcFontGetCharWidth`, `u8 c = cnt;`, `-e->gap + Font_GetKerning(...)` y barrera `while (0) {}` (**110/110 exacto**).
- **Bytes recuperados:** +440 B.

## Password_Decode (unit_50510, 1304 B / 326 instr, frame 0x58) — NON_MATCHING 324/326 (mejorada desde 318/326) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Password_Apply`), `callees` (`strcmp`); confirmó los 4 tramos (`strcmp`, unscramble, suma+checksum, desempaquetado 5-bit con `D_80123D2C` y `gUnlockBitMap`).
- **Hipótesis probadas:** Extraer `static inline u8 Password_Wrap32_(s32 d) { if (d >= 0) return d; return d + 32; }` cierra `bnel` (`0x2B8`), `bltz` (`0x2DC`) y `addiu $v0, $v1, 0x20` (`0x2F4`), subiendo de `318/326` a **324/326** (1304/1304 B). Residual de 2 palabras: `andi $v0, $v0, 0xFF` en `0x2CC` (antes de `sb D_80123D2C`) vs `0x2D4` (después), y `andi $a3, $v0, 0xFF` en el delay slot `0x2E8` frente a `addu $v0, $v1, $zero`. Guardada en `#ifdef NON_MATCHING` de `src/unit_50510.c`.
- **Bytes recuperados:** 0 B oficiales.

## Mesh_BuildDisplayList (unit_19D40, 2708 B / 677 instr, frame 0x58) — NON_MATCHING 591/677 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`SceneNode_PrepareMaterial`, `SceneNode_DrawMesh`, `ModelData_SetVariant`, `ModelData_Draw`), `callees` (`Gfx_LoadTexture`); sin aporte nuevo (residual puro de registros).
- **Diagnóstico:** `-df`/`-dg` confirma que `curTex` (`reg 78`, 15 usos / 706 insns) queda 7.º en `allocno_compare` (`$s6`) frente a `p+3` (`reg 582`, 89 usos / 684 insns, `$s0`); variantes de bucle (`for`/`while`), `register`, tipos y orden de declaraciones no alteran la rotación `$s0..$s6`. Sin cambios en `src/unit_19D40.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 64 result:** 2 exact matches (`Hud_DrawSlideFromLeft` **+452 B**, `Hud_DrawSlideFromRight` **+440 B**; **+892 B**) + mejora `NON_MATCHING` en `Password_Decode` (`318/326` → **324/326**); **objdiff 91.70 % (506,872 / 552,756 B, 1,842 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 65 (2026-11-26)

## Coll_PointsVsWorld (unit_39C90, 5232 B / 1308 instr, frame 0x288) — NON_MATCHING 1258/1308 (mejorada desde 969/1311) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Solve`), `callees` (`gcc2_compiled.`); confirmó el bucle de 4 paredes con `pfVar11 == local_1b0` (`k == 0`) y los accesos por índice `uVar22` / `pfVar21`.
- **Hipótesis probadas:** Quitar `do { ... } while (0)` de `VEC3_SCALE_M` y `CHECK_WALL_PVW` (+169 palabras), compartir `i`, `Vec3f *vp` y `ContactPvw_ *c` entre ambos bucles, invertir operandos en `Vec3_Cross_P_` (`b->y * a->z`, `b->x * a->z`, `b->x * a->y`), usar `if (k != 0)` y producto punto directo `wn.x * wnorm[k].x + ...` sube de `969/1311` (5244 B) a **1258/1308 (5232/5232 B exactos, +289 palabras)**. Residual de 50 palabras: intercambio `startVert` (`$fp`) ↔ `roomIdx` (`$t4`), `$fv0/$fv1` en `pl.n.y` (13w) y `$ft1/$ft3` en `vrot2` (19w). Guardada en `#ifdef NON_MATCHING` de `src/unit_39C90.c`.
- **Bytes recuperados:** 0 B oficiales.

## Coll_BoxCornersVsWorld (unit_39C90, 6472 B / 1618 instr, frame 0x278) — NON_MATCHING 1481/1618 (mejorada desde 992/1618) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Solve`), `callees` (`gcc2_compiled.`); confirmó el bucle de 4 paredes (`local_1b0`, `pfVar11 == local_1b0`), `s32 pd0s/pd1s` (`lh`) y la estructura de los 2 bucles sobre `gCollPathCars`.
- **Hipótesis probadas:** Quitar `do { ... } while (0)` de `VEC3_SCALE_M` y `CHECK_WALL_BCW` (+322 palabras), cambiar `pd0s`/`pd1s` a `s32` (+68), compartir `ContactBcw_ *c` (+6) y reemplazar el bucle `do-while (wnp)` por `for (k = 0; k < 4; k++)` con productos punto directos sobre `wnorm[k]` + `if (k != 0)` sube de `992/1618` (6408 B) a **1481/1618 (6472/6472 B exactos, +489 palabras)**; `0x0000..0x04C4` y `0x058C..0x0E0C` coinciden al 100 % (con `&wnorm[0]` en `$s6`, `shape` en `$s7` y `4096.0f` en `$fp`). Residual (137 palabras): asignación FPR en `vrot1`/`vrot2` y `CHECK_WALL_BCW`. Guardada en `#ifdef NON_MATCHING` de `src/unit_39C90.c`.
- **Bytes recuperados:** 0 B oficiales.

## Coll_ProbeWheels (unit_39C90, 4244 B / 1061 instr, frame 0x400) — NON_MATCHING 887/1061 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Solve`), `callees` (`gcc2_compiled.`); confirmó los 4 pasos sobre `gCollRacers`, el cálculo de `dotN` con `carUp[id]` (`afStack_168 + uVar39 * 3`) y el hoisting de `uVar39 * 3` / `uVar39 * 0xc`.
- **Diagnóstico:** Introducir `static inline f32 DotPW_(Vec3f *a, Vec3f *b)` en `carUp[id] * tmp` iguala el tamaño exacto (`4244/4244 B`), logra 100 % en Pass 1 (`0x000..0x144`) y eleva `&carUp[0]` en `0x178`, pero baja 1 palabra (`886/1061`) al quedar `rn` en `$t8` en vez de `$fp`; variantes de `register`/declaración/`up = &carUp[id]` no superan `887/1061`. Sin cambios en `src/unit_39C90.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 65 result:** 0 exact matches, grandes mejoras estructurales `NON_MATCHING` en `src/unit_39C90.c` (`Coll_PointsVsWorld` `969/1311` → **1258/1308**, 5232/5232 B exactos; `Coll_BoxCornersVsWorld` `992/1618` → **1481/1618**, 6472/6472 B exactos; +778 palabras exactas acumuladas); **objdiff 91.70 % (506,872 / 552,756 B, 1,842 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 66 (2026-11-26)

## Coll_PointsVsWorld (unit_39C90, 5232 B / 1308 instr, frame 0x288) — MATCH exacto 1308/1308 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Solve`), `callees` (`gcc2_compiled.`); sin cambios estructurales (residual puro de asignación de registros y CSE).
- **Hipótesis ganadora:** (1) Asignar `dist = Vec3_Dot_P_(&objRel, &pl.n) + pl.d; if (dist < obj->radius)` saca `dot + pl.d` a `global_alloc` (`$fa1`) y cierra las 13 palabras de `0x194..0x20C`; (2) introducir `f32 vn;` compartido entre Bucle 1 y Bucle 2 (`vn = ...; if (vn < 0.0f)`) cierra las 19 palabras de `vrot2` (`0x1190..0x1254`); (3) reutilizar `j` en lugar de `k` en `for (j = 0; j < 4; j++)` iguala `reg_n_sets(j) == 4` en `cse.c`, manteniendo `sltu $v0, $t2, $v0` (`0x22C`), reduciendo `REG_N_REFS(startVert)` bajo `roomIdx` e intercambiando `$fp`↔`$t4` (**1308/1308 exacto**). Integrada en `src/unit_39C90.c` (sin `TODO(match-debt)`).
- **Bytes recuperados:** **+5,232 B** oficiales.

## Coll_BoxCornersVsWorld (unit_39C90, 6472 B / 1618 instr, frame 0x278) — NON_MATCHING 1566/1618 (mejorada desde 1481/1618) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Solve`), `callees` (`gcc2_compiled.`); sin aporte nuevo (residual puro de asignación FPR).
- **Hipótesis probadas:** Compartir `f32 vn;` entre Bucle 1 y Bucle 2 cierra al 100 % ambos bloques `vrot1`/`vrot2` (+49 palabras → `1530/1618`); extraer `static inline f32 SubAbs_B_(f32 d, f32 p)` y asignar el último paso directamente (`wd[idx] = SubAbs_B_(d, Vec3_Dot_B_(&wn, &ax2))`) cierra al 100 % `ax1`, `ax2` y `0xF1C..0xF24` (+36 palabras → **1566/1618**, 6472/6472 B exactos). Residual (52 palabras): únicamente las mismas 13 palabras × 4 paredes de permutación FPR local (`$f6/$f8/$f2/$f4`) en `ax0` de `CHECK_WALL_BCW`. Actualizada en `#ifdef NON_MATCHING` de `src/unit_39C90.c`.
- **Bytes recuperados:** 0 B oficiales.

## Password_Decode (unit_50510, 1304 B / 326 instr, frame 0x58) — NON_MATCHING 324/326 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Password_Apply`), `callees` (`strcmp`); sin aporte nuevo (residual de 2 instrucciones en `Password_DecodeChar_`/`Password_Wrap32_`).
- **Diagnóstico:** Probadas variantes de tipos (`u8 v`, `s32` en `Password_FindChar_`/`Password_Wrap32_`, CSE vía `D_80123D2C`, ternario/variable `res` en `Password_Wrap32_`, `u8 val`): `u8 v` o `s32 Password_FindChar_` colapsan el trampolín `j .L800B0D20` (baja a `310/326`) y el resto mantiene `324/326`. Sin cambios en `src/unit_50510.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 66 result:** 1 exact match grande (`Coll_PointsVsWorld` **+5,232 B**, 1308/1308) + mejora `NON_MATCHING` en `Coll_BoxCornersVsWorld` (`1481/1618` → **1566/1618**, +85 palabras); **objdiff 92.65 % (512,104 / 552,756 B, 1,843 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 67 (2026-11-26)

## Coll_BoxCornersVsWorld (unit_39C90, 6472 B / 1618 instr, frame 0x278) — MATCH exacto 1618/1618 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Solve`), `callees` (`gcc2_compiled.`); confirmó los 4 bloques de pared y los 3 ejes `ax0..ax2` sobre `wd[idx]`.
- **Hipótesis ganadora:** Eliminar el temporal local `f32 d;` en `CHECK_WALL_BCW` y asignar directamente `wd[idx] = Vec3_Dot_B_(&wn, &wv); wd[idx] = SubAbs_B_(wd[idx], Vec3_Dot_B_(&wn, &ax0)); ...` resuelve la permutación FPR local (`$f6/$f8/$f2/$f4`) en `ax0` de las 4 paredes y cierra las 52 palabras residuales (**1618/1618 exacto**, `6472/6472 B`). Integrada en `src/unit_39C90.c` (sin `TODO(match-debt)`).
- **Bytes recuperados:** **+6,472 B** oficiales.

## Coll_ProbeWheels (unit_39C90, 4244 B / 1061 instr, frame 0x400) — NON_MATCHING 899/1061 (mejorada desde 887/1061) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Coll_Solve`), `callees` (`gcc2_compiled.`); confirmó los 4 pasos sobre `gCollRacers` y el acceso `carUp[id]` con puntero hoisted.
- **Hipótesis probadas:** Combinar `static inline f32 DotPW_(&carUp[id], &tmp)` en Pass 2 con `dist = rel.x * pl.n.x + rel.y * pl.n.y + rel.z * pl.n.z + pl.d; if (dist < o->b.rad)` en Pass 3 iguala el tamaño exacto (`4244/4244 B`) y sube +12 palabras (`887/1061` → **899/1061**); variantes de `Vec3_*` en Pass 2 no cruzan el umbral de hoisting de `loop.c` para `0x3F0` vs `0x400`. Actualizada en `#ifdef NON_MATCHING` de `src/unit_39C90.c`.
- **Bytes recuperados:** 0 B oficiales.

## Ai_Step (unit_22440, 13120 B / 3280 instr, frame 0x3C8) — NON_MATCHING 2512/3280 (borrador inicial) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Ai_Update`), `callees` (`Bld_StepRacer`, `Mtx3_Orthonormalize`, `Car_StartCrash`, `RaceClock_Get`, `cosf`, `sinf`); recuperó los 21 bucles sobre `gAi[6]` (stride `0x2A8`), todos los offsets de `SlotAi_`/`TrkAi_` y el parámetro `u32 dtMs`.
- **Avance:** Borrador completo en `#ifdef NON_MATCHING` de `src/unit_22440.c` con `SlotAi_ *ai` + `Car *car` a ámbito de función (**2512/3280 palabras**, `12712/13120 B`, frame `0x3C8` exacto). Bucles 7–10 y 20 ya casi al 100 %; delta residual (-408 B) concentrado en Bucle 3 (`Bld_StepRacer`/ramas), Bucles 13–14 (suavizado/base) y Bucles 17–19 (roll/stunts).
- **Bytes recuperados:** 0 B oficiales.

**Session 67 result:** 1 exact match grande (`Coll_BoxCornersVsWorld` **+6,472 B**, 1618/1618) + avances `NON_MATCHING` en `Coll_ProbeWheels` (`887/1061` → **899/1061**, 4244/4244 B exactos) y borrador inicial de `Ai_Step` (**2512/3280**, frame `0x3C8` exacto); **objdiff 93.82 % (518,576 / 552,756 B, 1,844 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.

# Session 68 (2026-11-26)

## Hud_DrawTurbos (unit_49CF0, 1528 B / 382 instr, frame 0x68) — NON_MATCHING 377/382 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Hud_DrawPlayer`), `callees` (`Car_Get`, `RaceClock_Get`); sin aporte nuevo (residual puro de orden de slots en stack).
- **Diagnóstico:** Confirmado aislando `Hud_DrawTurbos` que sin `u8 buf[24]` el spill `turbos` cae en `0x1F($sp)` exacto (con `$s0` en `0x28($sp)` y marco `-0x50`), mientras que cualquier `buf[24]` en `expand` desplaza tanto `$s0` (`0x40`) como `turbos` (`0x37`) en `+0x18`; `volatile u8 turbos` rompe optimizaciones (`339/389`). Sin cambios en `src/unit_49CF0.c`.
- **Bytes recuperados:** 0 B oficiales.

## Cup_SetupRace (unit_53B30, 508 B / 127 instr, frame 0x38) — NON_MATCHING 126/127 — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`[]`), `callees` (`[]`); sin aporte nuevo (residual de 1 instrucción de scheduling `subu $v1, $v1, $a0` en `0x1A8` vs `0x1B0`).
- **Diagnóstico:** Probadas 14 variantes sobre el bucle `else` (indexación directa, `ofs` `u32`/inline, intercambio de parámetros de `CupSetup_AddOff_`, reordenación con `used[r] = 1` y puntero `lp`); todas quedan en `126/127` o bajan a `106-113/127`. Sin cambios en `src/unit_53B30.c`.
- **Bytes recuperados:** 0 B oficiales.

## Ai_Step (unit_22440, 13120 B / 3280 instr, frame 0x3C8) — NON_MATCHING 3037/3280 (mejorada desde 2512/3280) — 2026-11-26
- **REA:** `procedure_pseudo_code`, `callers` (`Ai_Update`), `callees` (`Bld_StepRacer`, `Mtx3_Orthonormalize`, `Car_StartCrash`, `RaceClock_Get`, `cosf`); confirmó la estructura de los 21 bucles sobre `gAi[6]` y los bloques de ramas, suavizado, matriz de roll y stunts.
- **Avance:** Acceso directo al global `gAi[i]` (con `extern Slot85_ gAi[];` bajo `#ifndef NON_MATCHING`), firma exacta `u8 Bld_StepRacer(..., PlaneAi_)` (4º arg de 16 B, alinea todo el stack), struct `AiCfg_` para `D_80138D51..D_80138D57`, macro ternaria `FABS_` inline en Bucles 4 y 13, `< 0.0f || > 0.0f` en Bucle 14, ramas invertidas en Bucles 4/13/18/20, `u8 willCrash` + `if (rate != 0.0f)` por rama en Bucle 18, `proj` en dos pasos en Bucle 6, `rollM`/`skewM` desenrollados y `continue;` en `else` de Bucle 21. Sube **+525 palabras** (`2512/3280` → **3037/3280**, `13072/13120 B`, delta `-48 B`). Bucles 1–2, 5–16 y 20–21 prácticamente al 100 %. Actualizada en `#ifdef NON_MATCHING` de `src/unit_22440.c`.
- **Bytes recuperados:** 0 B oficiales.

**Session 68 result:** gran avance `NON_MATCHING` en `Ai_Step` (`2512/3280` → **3037/3280**, +525 palabras, `13072/13120 B`, delta `-48 B`) + diagnósticos sin cambio en `Hud_DrawTurbos` (`377/382`) y `Cup_SetupRace` (`126/127`); **objdiff 93.82 % (518,576 / 552,756 B, 1,844 / 1,910 functions, 187 / 221 complete units)**. Verificación final (`make`, SHA-1 `94913a07…`, `verify_funcs.py`, `check_no_asm.py`, `objdiff_report.py`) OK.
