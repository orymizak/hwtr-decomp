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
