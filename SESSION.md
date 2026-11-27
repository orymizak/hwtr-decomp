# Sesión de Decompilación HWTR

## Sesión 64 (2026-11-26) — REA + Ghidra + C + ASM: 2 nuevos MATCH exactos (`Hud_DrawSlideFromLeft` +452 B, `Hud_DrawSlideFromRight` +440 B; **+892 B**) + mejora `Password_Decode` (`324/326`) → **91.70 % objdiff**
- **Métrica oficial objdiff:** **91.70 %** (**506,872 / 552,756 B**), **1,842 / 1,910 funciones (96.44 %)**, **187 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py` y `check_no_asm.py`: OK.
- **Entorno:** `rea_query.py` OK (una sola consulta para `Hud_DrawSlideFromLeft`, `Hud_DrawSlideFromRight`, `Password_Decode` y `Mesh_BuildDisplayList`).
- Sin commit (el árbol de trabajo acumula **21 funciones exactas ganadas desde HEAD `8df6220`** — listo para revisión y `make clean && make` del usuario si desea commitear).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `Hud_DrawSlideFromLeft` | `src/unit_49CF0.c`, 452 B | **MATCH 113/113** (+452 B) | `if` anidado con doble `svcFontGetCharWidth` da a `s` la ref extra para `$s1`/`$s2`; `u8 *s = arg0; u8 c = cnt;` fija prólogo; `-e->gap + Font_GetKerning(...)` + `while (0) {}` cierra la última palabra |
| `Hud_DrawSlideFromRight` | `src/unit_49CF0.c`, 440 B | **MATCH 110/110** (+440 B) | Mismo patrón que `Hud_DrawSlideFromLeft` (`if` anidado, `u8 c = cnt;`, `-e->gap + Font_GetKerning(...)`, `while (0) {}`) |
| `Password_Decode` | `src/unit_50510.c`, 1,304 B | `NON_MATCHING` **324/326** (mejorada desde 318/326, 0 B) | `static inline u8 Password_Wrap32_(s32 d)` cierra `bnel`, `bltz` y `addiu`; solo quedan 2 palabras (`andi` en `0x2CC` vs `0x2D4`, y delay slot `0x2E8`) |
| `Mesh_BuildDisplayList` | `src/unit_19D40.c`, 2,708 B | `NON_MATCHING` **591/677** (sin cambio, 0 B) | `-df`/`-dg` confirma rotación pura `$s0..$s6` (`curTex` 15 usos vs `p+3` 89 usos en `allocno_compare`) |

---

## Sesión 61 (2026-11-26) — REA + Ghidra + C + ASM: 1 nuevo MATCH exacto (`Save_ReadAll`, +776 B) → **91.25 % objdiff**
- **Métrica oficial objdiff:** **91.25 %** (**504,372 / 552,756 B**), **1,838 / 1,910 funciones (96.23 %)**, **187 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py` y `check_no_asm.py`: OK.
- **Entorno:** `rea_query.py` OK (una sola consulta para `Hud_DrawTurbos`, `Save_ReadAll`, `Cup_SetupRace` y `Coll_SyncObjects`).
- Sin commit (el árbol de trabajo acumula ya **17 funciones exactas ganadas desde HEAD `8df6220`** — listo para revisión y `make clean && make` del usuario si desea commitear).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `Save_ReadAll` | `src/unit_50510.c`, 776 B | **MATCH 194/194** (+776 B) | Tipar `E140_` con `s32 id; u8 d[0x13C];` y escribir `gSaveSlots[i][j].id = gSaveIds[i]` evita subir `&gSaveSlots` a `$fp` y libera `$fp` para `&sysSaveRead`; `for (k = 0, n = 0xD90; k < n; k++)` cierra la última palabra |
| `Cup_SetupRace` | `src/unit_53B30.c`, 508 B | `NON_MATCHING` **126/127** (sin cambio, 0 B) | Variantes de `ofs`, `D_80123EC1`, barreras y `decomp-permuter` 25 s no movieron `subu $v1, $v1, $a0` de `0x1B0` a `0x1A8` |
| `Hud_DrawTurbos` | `src/unit_49CF0.c`, 1,528 B | `NON_MATCHING` **382/382 masked / 377/382 exactas** (sin cambio, 0 B) | Límite de sesiones alcanzado sobre el offset de spill `0x37($sp)` vs `0x1F($sp)` |
| `Coll_SyncObjects` | `src/unit_39C90.c`, 1,216 B | `NON_MATCHING` **277/304** (sin cambio, 0 B) | Variantes de `TransformShape_` (macros, helpers, permutaciones de locales); cargar `src`/`dst` antes de `m` iguala tamaño (`1216/1216 B`) pero desplaza loads (274/304) |

---

## Sesión 57 (2026-11-26) — C + ASM: mejora estructural `NON_MATCHING` en `SndSrc_Play` (residual reducido a 1 instr) → **90.61 % objdiff**
- **Métrica oficial objdiff:** **90.61 %** (**500,844 / 552,756 B**), **1,834 / 1,910 funciones (96.02 %)**, **185 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py` y `check_no_asm.py`: OK.
- **Entorno:** `rea` no estaba disponible en `PATH` (análisis por ASM y código hermano en `src/`).
- Sin commit (0 funciones exactas ganadas en esta sesión; siguen **13 funciones exactas ganadas desde HEAD `8df6220`** — listo para revisión y `make clean && make` del usuario si desea commitear).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `SndSrc_Play` | `src/unit_01050.c`, 420 B | `NON_MATCHING` **100/105** (mejorada, **104/105** salvo 1 instr / 3 offsets, 0 B) | `if (s != NULL) { ... if (busy != 0) return 0; } else { return 0; }` cierra el 100 % del flujo de control (`0xF8`, `0x150` `return 0`, `0x180` `return 1` fallthrough); solo falta `addu $a0, $v0, $zero` antes de `beqz $a0` |
| `Race_ReadPlayerControls` | `src/unit_2C6A0.c`, 844 B | `NON_MATCHING` **194/211** (sin cambio, 0 B) | Anidado `if (!(t < 0.0f)) { if (!(t > 1.0f)) ... }` corrige el 2.º `bc1tl`, pero persisten el 3.º y la asignación `$f0/$f6` para `1.0f` |
| `n_alFxPull` | `src/unit_74AB0.c`, 784 B | `NON_MATCHING` **195/196** (sin cambio, 0 B) | Variantes de `aDMEMMove`, `static inline` y barreras no movieron `addiu $v0, $t0, 0x8` de `0x2D4` a `0x2C8` |
| `Mesh_GetMaterialList` | `src/unit_19D40.c`, 392 B | `NON_MATCHING` **91/98** (sin cambio, 0 B) | Variantes de puntero `dst`, `m` compartido y `do-while` no resolvieron el intercambio `$t0`/`$t1` en el bucle de submesh |

---

## Sesión 56 (2026-11-26) — C + ASM: 1 nueva reconstrucción grande `NON_MATCHING` (`Coll_ProbeWheels`, 4,244 B) → **90.61 % objdiff** (detenida por 3 `NON_MATCHING` seguidas sin ganar bytes)
- **Métrica oficial objdiff:** **90.61 %** (**500,844 / 552,756 B**), **1,834 / 1,910 funciones (96.02 %)**, **185 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,814 funciones en C, 483,472 B, todas en su dirección final. `check_no_asm.py`: OK.
- **Entorno:** `rea` no estaba en `PATH` (`prepare_elf.py` OK; análisis por ASM + funciones hermanas en `src/`).
- Sin commit (0 funciones exactas ganadas en esta sesión; siguen **13 funciones exactas ganadas desde HEAD `8df6220`**).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `CupResults_SetupTexts` | `src/unit_53B30.c`, 1,516 B | `NON_MATCHING` **378/379** (sin cambio, 0 B) | `CupRec_ *recs` local corrige el orden de `addu $v0, $v0, $t0` pero rota `$t0/$t1/$t2` (+1 instr) |
| `Password_Decode` | `src/unit_50510.c`, 1,304 B | `NON_MATCHING` **318/326** (sin cambio, 0 B) | Variantes `u8`/`s32` en `Password_DecodeChar_`/`Password_FindChar_` y `if/else` vs ternario no cerraron `0x2B8..0x2F8` |
| `Coll_ProbeWheels` | `src/unit_39C90.c`, 4,244 B | `NON_MATCHING` **835/1061** (nueva, **4240/4244 B**, **1023/1061 opcode-only**, 0 B) | Reconstrucción completa de 4 pasadas (`P1: 87/90`, `P4: 162/164`, `P3: 203/246`); residual en marco `0x3F0` vs `0x400` (`$s7` retenido para `($sp+0x10)+id*24` en vez de spill a `0x394($sp)`) |

También exploradas brevemente sin cambio en `src/`: `n_alFxPull` (195/196), `SndSrc_Play` (100/105), `Mesh_GetMaterialList` (91/98), `Hud_DrawSlideFromRight` (80/110).

---

## Sesión 55 (2026-11-26) — REA + Ghidra + C + ASM: 1 nuevo MATCH exacto (+1,164 B) + 4 mejoras `NON_MATCHING` → **90.61 % objdiff**
- **Métrica oficial objdiff:** **90.61 %** (**500,844 / 552,756 B** — **¡superados los 500 KB!**), **1,834 / 1,910 funciones (96.02 %)**, **185 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,813 funciones en C, 479,228 B, todas en su dirección final. `check_no_asm.py`: OK.
- Sin commit aún (**13 funciones exactas ganadas desde HEAD `8df6220`** — listo para revisión y `make clean && make` del usuario si desea commitear).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `Audio_Init` | `src/unit_01050.c`, 1,164 B | **MATCH 291/291** (+1,164 B) | Separar `D_800EFD88[9][8]` de `gAudio`, barrera `do { } while (0);` tras `sfxTable[60]`, locales `ptrStart`/`ptrEnd` con símbolos `rom_files_CCCSND_*` y punteros `p` en ambos bucles de puesta a cero |
| `CupResults_SetupTexts` | `src/unit_53B30.c`, 1,516 B | `NON_MATCHING` **378/379** (subido desde 377/379, 0 B) | Usar `pos * 0x19 + 0xF0` en el bucle `3..5` elimina `yBlank` y cierra toda la segunda mitad al 100 %; solo queda el orden de `addu` en `0x0094` |
| `n_alFxPull` | `src/unit_74AB0.c`, 784 B | `NON_MATCHING` **195/196** (subido desde 190/196, 0 B) | Precalcular `w1` antes de `do { } while (0);` en el `aDMEMMove` final; solo queda la posición de `addiu $v0, $t0, 0x8` (`0x2D4` vs `0x2C8`) |
| `SndSrc_Play` | `src/unit_01050.c`, 420 B | `NON_MATCHING` **100/105** (subido desde 96/105, 0 B) | Struct `AudioBlock_` (`extra[9][8]` + `sndSrcList` en `+0x378`) iguala `0x000..0x0F4` al 100 %; residual de 5 palabras en `Snd_IsPlaying` |
| `SortQueue_Sort` | `src/unit_20830.c`, 816 B | `NON_MATCHING` **179/204** (subido desde 158/204, 0 B) | Mover `sp += 2` dentro de ambas ramas del push resuelve el intercambio `$t3/$t4` en toda la función; residual en `move $4,$9` (`0x12C`) |

Otras funciones investigadas con REA/Ghidra y ASM sin cambio en `src/`: `Password_Decode` (318/326), `Mesh_GetMaterialList` (91/98), `Race_ReadPlayerControls` (194/211), `Mesh_BuildDisplayList` (591/677).

---

## Sesión 54 (2026-11-26) — REA + Ghidra + C + ASM: 2 nuevos MATCH exactos (+1,312 B, 1 unidad completa) → **90.40 % objdiff**
- **Métrica oficial objdiff:** **90.40 %** (**499,680 / 552,756 B**), **1,833 / 1,910 funciones (95.97 %)**, **185 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,813 funciones en C, 479,228 B, todas en su dirección final. `check_no_asm.py`: OK.
- Sin commit aún (**12 funciones exactas ganadas desde HEAD `8df6220`** — listo para revisión y `make clean && make` del usuario si desea commitear).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `Gfx_UpdateLights` | `src/unit_103B0.c`, 676 B | **MATCH 169/169** (completa `unit_103B0` 100 %) | `extern u8 gNumLights[]` + `Lights7 *lights` (`&lights->l[i]`, `&lights->a`) + `((LightFlag_ *)&gLightsDirty)->v = 0` (`MEM_IN_STRUCT_P` impide el hoist de `sb` sobre `sw 0xC($a0)` sin alterar `Light_SetColor`) |
| `Coll_LoadLevel` | `src/unit_39C90.c`, 636 B | **MATCH 159/159** | `((CollPtr_ *)&gCollLevel)->p + 0xD8` (`MEM_IN_STRUCT_P` mantiene la segunda carga `lui/lw` de `gCollLevel` en `$v1` antes de `+ 0xD8`) |

Otras funciones investigadas sin cambio en `src/` (presupuesto agotado): `n_alFxPull` (190/196), `Race_ReadPlayerControls` (192/211), `Password_Decode` (318/326), `Mesh_GetMaterialList` (91/98), `Cup_SetupRace` (hipótesis probada a 112/127 con `lap[2]` duplicado y `D_80123EC1[]`), `Hud_DrawSlideFromLeft`/`Right` (84/113, 80/110), `Coll_SyncObjects` (277/304), `SndSrc_Play` (96/105).

---

## Sesión 53 (2026-11-26) — C + ASM: 4 nuevos MATCH exactos (+4,464 B, 3 unidades completas) + 1 `NON_MATCHING` → **90.16 % objdiff**
- **Métrica oficial objdiff:** **90.16 %** (**498,368 / 552,756 B**), **1,831 / 1,910 funciones (95.86 %)**, **184 / 221 unidades completas** (`report/us/report.json == build/us/report.json`) — **¡superado el 90 %!**
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,813 funciones en C, 479,228 B, todas en su dirección final. `check_no_asm.py`: OK.
- Sin commit aún (10 funciones exactas ganadas desde HEAD `8df6220`, en espera de ~30 funciones o confirmación del usuario con `make clean && make`).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `Model_SwapPalette` | `src/unit_06050.c`, 324 B | **MATCH 80/80** (completa `unit_06050` 100 %) | `(u8 *)pals + (k << 3) + 4` fija `$v0/$v1`; temporales `a/c/d` para `<< 11`/`<< 6`/`<< 1` antes de `d \| (a \| c) \| 1` |
| `Scroller_DrawLine` | `src/unit_27A40.c`, 492 B | **MATCH 123/123** (completa `unit_27A40` 100 %) | `u32 x = 0; u8 *p = str;` antes de `if (bright != 0)` + `x += sysFontGetCharWidth(0, *p) + Font_GetKerning(0, *p, p[1])` |
| `PauseUi_DrawText` | `src/unit_6AE90.c`, 352 B | **MATCH 88/88** (completa `unit_6AE90` 100 %) | `static inline s16 PauseUi_MeasureText_(u8 *s)`, `u16 cx`, `s16 w/x`, `s = text` antes de `svcDraw2DSetColor` |
| `CarSnd_Update` | `src/unit_01050.c`, 3,296 B | **MATCH 824/824** | Variable `s32 type;` compartida en `if (p1)` y `if (p0)` del coche 0 (sin cruzar llamadas → `$v1`, evita hoist de `li 13` al delay slot de `beqz $s7`) |

Reconstrucción `NON_MATCHING` añadida en `src/` bajo `#ifdef NON_MATCHING` (0 B oficiales):
- `SortQueue_Sort` (`src/unit_20830.c`, 816 B): **158/204** palabras, tamaño exacto **816/816 B** (quicksort iterativo + insertion sort `M=7`; residual: intercambio `$t3/$t4` entre `sp`/`q` y `move $4, $9`).

---

## Sesión 52 (2026-11-26) — C + ASM: 1 nuevo MATCH exacto (+416 B, completa `gfx_pools` 100 %) + 4 mejoras `NON_MATCHING` → **89.35 % objdiff**
- **Métrica oficial objdiff:** **89.35 %** (**493,904 / 552,756 B**), **1,827 / 1,910 funciones (95.65 %)**, **181 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,811 funciones en C, 477,920 B, todas en su dirección final. `check_no_asm.py`: OK.
- Sin commit aún (6 funciones exactas ganadas desde HEAD `8df6220`, en espera de ~30 funciones o confirmación del usuario con `make clean && make`).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `GfxPools_Init` | `src/gfx_pools.c`, 416 B | **MATCH 104/104** (completa `gfx_pools` 100 %) | Reordenado de `chunkCount = 1` tras `end` en `GfxPool_Init`/`VtxPool_Init`/`MtxPool_Init` (`0x300` en `$a0`, `1` en `$a2`) |

Reconstrucciones `NON_MATCHING` añadidas/mejoradas en `src/` bajo `#ifdef NON_MATCHING` (0 B oficiales):
- `Mesh_BuildDisplayList` (`src/unit_19D40.c`, 2,708 B): **591/677** palabras, tamaño exacto **2,708/2,708 B**, 0 diffs de opcodes/scheduling (residual: rotación pura `$s0..$s6` con `curTex` en `$s6` vs `$s0`).
- `Password_Decode` (`src/unit_50510.c`, 1,304 B): **318/326** palabras (subido desde 239/328), tamaño exacto **1,304/1,304 B**.
- `n_alFxPull` (`src/unit_74AB0.c`, 784 B): **190/196** palabras, tamaño exacto **784/784 B** (`0x000..0x2BC` idéntico).
- `Coll_BoxCornersVsWorld` (`src/unit_39C90.c`, 6,472 B): **992/1,618** palabras (6,408/6,472 B, frame `0x278` y offsets de pila exactos).

---

## Sesión 51 (2026-11-25) — REA + Ghidra + C: 5 nuevos MATCH exactos (+11,528 B, 1 unidad completa) → **89.28 % objdiff**
- **Métrica oficial objdiff:** **89.28 %** (**493,488 / 552,756 B**), **1,826 / 1,910 funciones (95.60 %)**, **180 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,807 funciones en C, 470,248 B, todas en su dirección final. `check_no_asm.py`: OK.
- Sin commit aún (5 funciones exactas ganadas desde HEAD `8df6220`, en espera de ~30 funciones o confirmación del usuario con `make clean && make`).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `Hud_ShowTrick` | `src/unit_49CF0.c`, 1,984 B | **MATCH 496/496** | `(s16)turbos` en `sprintf` (evita `li $a2, 1` y permite cross-jump de `D_80100AD0` + `andi $a2, $s5, 0xFF`), `Order3_49CF0_` (`D_80100AC4`), `ElPair_ el[3]` en `HudF0_` |
| `Garage_SetupTexts` | `src/unit_53B30.c`, 1,388 B | **MATCH 347/347** | `else if (i + 1 >= 0x29)` duplicando `prev = i - 1;` en ambas ramas para que `prev` no cruce `slti` y se aloje en `$v0` (P1 y P2) |
| `Coll_BoxVsWorldEdges` | `src/unit_39C90.c`, 3,784 B | **MATCH 946/946** | `u8 CollBox_EdgeSeparatingAxis`, `u8 a, b; if (!a)` en los 6 recortes, `f32 t` compartida con `depth`/`dot` (`$fa1`), ternarios `dxp..dzm` con `neg.s`+`sub.s`, `ldir * seg.len` directo |
| `ModelData_Draw` | `src/unit_13490.c`, 3,572 B | **MATCH 893/893** (completa `unit_13490` 100 %) | `inline SceneNode_DrawSingle`/`ModelData_SetVariant`, `a0Mtx`/`a1Mtx` para la evaluación de `Mtx34_Mul` en hijos (`child->child`), `ev = m->envVtx` y bucle `rand() & 1` de partículas |
| `Password_Encode` | `src/unit_50510.c`, 800 B | **MATCH 200/200** | Promovido de `NON_MATCHING`: asignación directa a `D_80123D2C` y `out[k] = gPasswordChars[D_80123D2C]` dentro de ambas ramas `if (r < 4) / else`, reutilizando `bi` en los bucles de suma y permutación |

Reconstrucciones `NON_MATCHING` congeladas/conservadas en `src/` bajo `#ifdef NON_MATCHING` (0 B oficiales):
- Congeladas tras pasada de permuter: `CarSnd_Update` (`src/unit_01050.c`, **819/824**, 3,296/3,296 B), `Hud_DrawTurbos` (`src/unit_49CF0.c`, **374/382**, 1,524/1,528 B), `Coll_PointsVsWorld` (`src/unit_39C90.c`, **969/1311**, 5,244/5,232 B).
- Trabajadas en esta sesión (con tope de 5 variantes sin subida): `CupResults_SetupTexts` (`src/unit_53B30.c`, **377/379**, 1,516/1,516 B exacto, subido desde 306/380), `Coll_SyncObjects` (`src/unit_39C90.c`, **277/304**, 1,212/1,216 B), `Audio_Init` (`src/unit_01050.c`, **273/291**, 1,164/1,164 B exacto), `Cup_SetupRace` (`src/unit_53B30.c`, **96/127**, 496/508 B, subido desde 88/127), `SndSrc_Play` (`src/unit_01050.c`, **96/105**, 416/420 B), `Mesh_GetMaterialList` (`src/unit_19D40.c`, **91/98**, 388/392 B). Detalle completo en `MATCH_LOG.md`.

---

## Sesión 49 (2026-11-25) — REA + Ghidra + C: 5 nuevos MATCH exactos (+10,920 B, 2 unidades completas) → **84.37 % objdiff**
- **Métrica oficial objdiff:** **84.37 %** (**466,380 / 552,756 B**), **1,814 / 1,910 funciones (94.97 %)**, **176 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,788 funciones en C, 431,544 B, todas en su dirección final. `check_no_asm.py`: OK.
- Sin commit aún (20 funciones exactas ganadas desde HEAD `51af82d`, en espera de ~30 funciones o confirmación del usuario con `make clean && make`).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `Input_Update` | `src/unit_0B140.c`, 736 B | **MATCH 184/184** (completa `unit_0B140` 100 %) | `switch (i)` para `0..7`, casos de `bind` en orden inverso, `f32 dt2` para `u32->f64`, `if (bind & cont.button)` en `default:` digital |
| `Ai_InitRacer` | `src/unit_22440.c`, 2,700 B | **MATCH 675/675** | `Slot85_` unificado, `static inline u32 Ai_Rand_(u32 n)` para emitir `divu` por `201`, `f1FC` tras `z204 = 0` |
| `Car_AirControl` | `src/unit_31E10.c`, 3,120 B | **MATCH 780/780** | `Vec3_Scale_AC_(&car->vE4, ..., 0.98f)`, `sPitch *= pitch` primero, inercias antes de `center`, `Vec3_Neg_` tras `Vec3_Sub_`, `tq1`/`tq2` en `push_AC_` |
| `CollBox_FindSeparatingAxis` | `src/unit_39C90.c`, 3,176 B | **MATCH 794/794** | Hermano de `CollBox_EdgeSeparatingAxis`: `Mat3T_MulVec_FSA_`, `f32 eps = 0.001f;`, sumas asociativas por la derecha en los 15 ejes SAT |
| `SkidMark_AddQuad` | `src/unit_1CC80.c`, 1,188 B | **MATCH 297/297** (completa `unit_1CC80` 100 %) | `surf = D_800F2634[surf];` reutilizando `$a0`, encadenados `= 0.0f`/`= 0xFF`, `maxf_sm_` inline, `alphaVel` tras `alpha` |

Borradores cercanos en `.scratch/s49/` (no integrados, 0 B oficiales): `Mesh_GetMaterialList` (`mg8.c` 91/98, bucle 1 exacto), `SndSrc_Play` (`sp5.c` 96/105). Detalle completo en `MATCH_LOG.md`.

---

## Sesión 48 (2026-11-25) — REA + Ghidra + C: 5 nuevos MATCH exactos (+7,484 B) → **81.37 % objdiff**
- **Métrica oficial objdiff:** **81.37 %** (**449,764 / 552,756 B**), **1,803 / 1,910 funciones (94.40 %)**, **174 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,777 funciones en C, 414,928 B, todas en su dirección final. `check_no_asm.py`: OK.
- Sin commit aún (9 funciones ganadas desde HEAD `8f28b9e`, en espera de ~30 funciones o confirmación del usuario con `make clean && make`).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `RaceAudio_Update` | `src/unit_2C6A0.c`, 1,400 B | **MATCH 350/350** | `audVoicePlayRandom(D_8011852C)` (Ghidra omitía el arg), struct `AudLoop_` en `0x8014C0D0`, `D_801184C0` como `u8`, `p++, i++` |
| `CollBox_EdgeSeparatingAxis` | `src/unit_39C90.c`, 1,220 B | **MATCH 305/305** | `Mat3T_MulVec_` inline con `Vec3f *`, `if (v < 0.0f) v = -v;`, `f32 eps = 0.001f;`, parentización de dot/extents |
| `Cars_Init` | `src/unit_31E10.c`, 2,320 B | **MATCH 580/580** | `RigidBody_Init` 3 floats + `Vec3f`, `svcModelSetTransform(..., (u8 *)&m2C + 0x94)`, `k53C` en ambas ramas, `ox = offset.x` antes de `offset.z += h` |
| `Model_SetPartRotation` | `src/unit_06050.c`, 1,276 B | **MATCH 319/319** | `= 0.0f` encadenado, `D_80138190.partScale` (`MEM_IN_STRUCT_P` fuerza 9 recargas), rotaciones fila a fila con 2 temporales `a, b` |
| `Clouds_Init` | `src/unit_0B140.c`, 1,268 B | **MATCH 317/317** | `Vec2f_ vel` copia de struct, `s32 rv = Rand_Range(100)`, `if (max->z + 1200.0f > 10000.0f)`, `c++, j++`, `gCloudCount` antes de `D_80101EA0[1]` |

Borradores cercanos en `.scratch/s48/` (no integrados, 0 B oficiales): `Hud_ShowTrick` (`ht5_cond_not1.c` 491/497, solo 5 palabras en `sprintf` de `turbos == 1`), `SortQueue_Sort` (`sq_v3.c` 808/816 B), `Hud_DrawTurbos` (`hd_v5.c` 1528/1528 B con rotación `$s4..$s7`). Detalle completo en `MATCH_LOG.md`.

---

## Sesión 47 (2026-11-25) — REA + Ghidra + C: 4 nuevos MATCH exactos (+6,052 B) → **80.01 % objdiff**
- **Métrica oficial objdiff:** **80.01 %** (**442,280 / 552,756 B**), **1,798 / 1,910 funciones (94.14 %)**, **174 / 221 unidades completas** (`report/us/report.json == build/us/report.json`).
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1,772 funciones en C, 411,032 B, todas en su dirección final.
- **Meta del 80 % alcanzada.** Sin commit aún (en espera de alcanzar ~30 funciones nuevas o confirmación del usuario con `make clean && make`).

| Función | Unidad / tamaño | Resultado | Detalle clave |
|---|---|---|---|
| `Bg_DrawImage` | `src/unit_13490.c`, 1,204 B | **MATCH 301/301** | Macros GBI CI8 + `gDPLoadTLUT_pal256(gBgPalette)`, `timg += 0x200` tras TLUT, `(u32)((u32)(n*8) + (u32)gDisplayListHead + 14*8)` |
| `Model_UpdateWheelEffects` | `src/unit_06050.c`, 1,140 B | **MATCH 285/285** | `ModelAll_ gModels` sin cast, color `D_800F0220` como 3 escalares `s32`, `Model_ *m = gModels.models[i]`, `u32 j; u32 i;` |
| `Ai_RestoreSlot` | `src/unit_22440.c`, 2,100 B | **MATCH 525/525** | Unificación de `SlotD_` en `Slot85_` (unión `f24`/`blk` en `+0x08`) sin cast sobre `gAi` |
| `MenuCar_Draw` | `src/unit_53B30.c`, 1,608 B | **MATCH 402/402** | `Vec3f off` por valor, `Vec3f d` antes de `m[3][4]`, `if (id == 0)` en `Menu_DeltaTenths`/`Menu_DeltaSeconds`, `f32 diff` para lerp |

Borradores muy cercanos guardados en `.scratch/s47/` (no integrados, 0 B oficiales): `Hud_DrawTurbos` (`hd_f2.c` 374/382, `hd_m2.c` 1528/1528 B exacto con rotación `$s4..$s7`), `Coll_SyncObjects` (`cs_for.c` 277/304), `Audio_Init` (`audio_init.c` 273/291, 1164/1164 B). Detalle completo en `MATCH_LOG.md`.

---

## Sesión 45 (2026-11-25) — solo investigación, sin match nuevo
- **objdiff sin cambios: 78.67 %** (434.864 / 552.760 B, 1.791 / 1.910 funciones). ROM OK, `make` incremental verde.
- No se integró nada en `src/`: los cinco candidatos siguen a pocas palabras. Borradores en `.scratch/s45/`.
- **Arreglo de herramienta:** `tools/match/try.py` imprimía las líneas `+` desalineadas (indexaba la salida de
  `objdump` por número de línea en vez de por dirección), así que a partir del primer `nop` el diff mostraba
  instrucciones nuestras equivocadas. El recuento `iguales N/M` siempre fue correcto. Añadido `.scratch/s45/sbs.py`
  (diff en paralelo con la palabra hex).

| Función | Unidad / tamaño | Antes | Ahora | Residual |
|---|---|---|---|---|
| `Garage_SetupTexts` | `unit_53B30`, 1.388 B | — (INCLUDE_ASM) | **341/347**, tamaño exacto | `prev` debe ir a `$v0` (6 palabras) |
| `CupResults_SetupTexts` | `unit_53B30`, 1.516 B | — (INCLUDE_ASM) | 306/380 | `pos`/`y` y el bucle de filas vacías (74) |
| `Race_ReadPlayerControls` | `unit_2C6A0`, 844 B | 194/211 | 194/211 (orden del `add` corregido) | `1.0f` en `$f0` y 2 ramas `bc1tl` (17) |
| `GfxPools_Init` | `gfx_pools`, 416 B | — (INCLUDE_ASM) | 82/104, tamaño exacto | mitad Mtx y la constante `1` en `$a2` (22) |
| `Coll_LoadLevel` | `unit_39C90`, 636 B | 155/159 | 155/159 | la doble carga de `gCollLevel` (4) |

Detalle por función, con las hipótesis descartadas, en `MATCH_LOG.md` («Session 45»).
Para el 80 % siguen faltando **7.344 B**.

---


## Estado actual (actualizado al cierre de la sesión 44, 2026-11-25)
- **Métrica oficial: objdiff** (`tools/objdiff_report.py`, regenerado y verificado contra `build/us/report.json`; ver `METRICS.md`).
- **ByteMatch confirmado:** 78.67 % de código (**434.864 / 552.760 B**), 1.791 / 1.910 funciones (93.77 %), 173 / 221 unidades completas.
- **Para el 80 %:** 442.208 B → faltan **7.344 B**.
- **ROM:** SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748` (OK). `verify_funcs.py`: 1.765 funciones en C, 403.604 B, todas en su dirección final.
- **HEAD:** `8f28b9e`. El árbol de trabajo tiene 15 funciones nuevas con MATCH exacto sin commit (se agrupan hacia el objetivo de ~30 funciones por commit según `AGENTS.md` §4).
- Las funciones `NON_MATCHING` no se cuentan: la build normal usa `INCLUDE_ASM` en su lugar.
- Regla de reintentos vigente: 2 sesiones por función sin progreso medible (ver `AGENTS.md`; la mención de "3 intentos" más abajo es histórica).

### Sesión 44 — REA + Ghidra + C (3 matches exactos = +3.696 B, 2 unidades completadas, 1 parcial)
| Función | Unidad / tamaño | Resultado | Detalle |
|---|---|---|---|
| `Bld_StepRacer` | `src/unit_21150.c`, 1.892 B | **MATCH 473/473** (completa `unit_21150` al 100%) | Cross-jumping de `a1/b1` antes de `a->ang = 0.0f; if (flag)` para colocar `sw $zero, 0x14($s0)` en el delay slot de `beqz`; orden `a0/b0` verificado en ROM; `jtbl_800FFA50` en `rodata_refs.txt` |
| `HighScores_SetupTexts` | `src/unit_53B30.c`, 1.080 B | **MATCH 270/270** | `D_800F5F80` identificado como `gMenuObjs[30]`, `case 2:` explícito en el `switch`, `y = 0xA0;` antes del bucle de 5 filas |
| `RaceView_DrawScene` | `src/unit_4F640.c`, 724 B | **MATCH 181/181** (completa `unit_4F640` al 100%) | `if/else` en `svcCameraSetPerspective`, `s32 alpha` para `svcModelSetAlpha`, y locales `t`/`b2`/`mask`/`vh` para hoistear `(h >> 1) + 1` en `$fp` y `(h >> 1) - 1` en `$s7` |
| `Coll_LoadLevel` | `src/unit_39C90.c`, 636 B | NON_MATCHING 155/159 (0 B) | Guardada bajo `#ifdef NON_MATCHING`; residual de 4 palabras por doble carga de `gCollLevel` en `0x80099110` |

### Sesión 41 — REA + Ghidra (4 funciones: 1 match, 3 parciales)
| Función | Unidad / tamaño | Resultado | Residual |
|---|---|---|---|
| `LZ_AdvanceBits` | `src/lz.c`, 168 B | **MATCH 42/42** (cuenta en objdiff) | — (Ghidra tipó `param_1` como `u8`; `register u8 n, register u8 *p`) |
| `Model_SwapPalette` | `unit_06050`, 324 B | NON_MATCHING 72/81 (0 B) | orden de 3 `sll` en el empaquetado |
| `SndCmd_SetEnvelopePreset` | `unit_6E610`, 332 B | NON_MATCHING 79/83 (0 B) | el original copia el puntero en `$a3` (delay slot de `bnez`) |
| `PauseUi_DrawText` | `unit_6AE90`, 352 B | NON_MATCHING ~71/88 (0 B) | cabecera del bucle y movimientos del prólogo |

Detalle en `MATCH_LOG.md` (sección «Sesión 41»). Sesión 40 añadió el match de `Ai_ApplyImpulse` (1.088 B) y tres NON_MATCHING
(`Hud_DrawSlideFromLeft/Right`, `Save_ReadAll`); sesión 39, cuatro NON_MATCHING (`Controls_SetupTexts` 224/228, `HighScores_SetupTexts`,
`Password_Encode` 782/800, `Password_Decode`).

### Nota sobre cifras históricas
Las cifras de abajo (74.77 %, 413.280 B) son objdiff en HEAD `31d1fab`; no son el estado actual.
`objdiff.json` cambió solo en un flag (`complete: true`) al regenerar el informe.

---

# Historial: sesión previa (HEAD 31d1fab / 569d788)

## Estado y Avance Confirmado
- **Último ByteMatch Confirmado:** 74.77% de código (413.280 / 552.760 bytes).
- **Funciones Coincidentes:** 92.88% (1.774 / 1.910 funciones).
- **Unidades C Completas:** 168 / 221 unidades al 100%.
- **SHA-1 ROM:** `94913a07e49005ffd43dfdfa16ff5862bdb93748` (MATCH exacto).
- **Comprobación de enlace (`verify_funcs.py`):** 1.741 funciones en C, 379.320 bytes; todas coinciden en su dirección final.
- **Último Commit:** `569d788` - *Match 16 functions across audio, explosions, car physics and UI (74.77%)*.

---

## Funciones Investigadas e Integradas en Esta Sesión (16 funciones, 100% Match)

1. **`CarSnd_StartEngine` (`src/unit_01050.c`, 732 B):**
   - Inicialización de loops duales de sonido de motor con paneo y Doppler según la posición del coche.
2. **`Explosion_InitDebris` (`src/unit_0D1F0.c`, 1.176 B):**
   - Generación de vectores de velocidad y matriz de rotación procedural para partículas de explosión.
3. **`Explosion_Start` (`src/unit_0D1F0.c`, 1.160 B):**
   - Asignación de luz puntual, escalado y arranque de anillo y escombros.
4. **`Explosion_Draw` (`src/unit_0D1F0.c`, 1.196 B):**
   - Renderizado 3D de escombros de explosión y billboard del anillo.
5. **`Gfx_BeginScene` (`src/unit_103B0.c`, 664 B):**
   - Inicialización de pipeline de renderizado y configuración de buffers de visualización.
6. **`WorldData_Relocate` (`src/unit_13490.c`, 720 B):**
   - Reubicación de punteros internos en la estructura de datos del mundo cargada desde ROM.
7. **`Car_Respawn` (`src/unit_31E10.c`, 1.192 B):**
   - Reubicación de coche tras salida de pista con búsqueda de posición más lejana respecto a los rivales vía `Bld_FindRecord`.
8. **`Car_ApplyAero` (`src/unit_31E10.c`, 776 B):**
   - Aerodinámica: arrastre opuesto a la velocidad, sustentación y downforce escalados por tuning de superficie.
9. **`Car_CheckWrongWay` (`src/unit_31E10.c`, 448 B):**
   - Detección de sentido contrario respecto a los vectores normales de la pista.
10. **`Hud_DrawSpeed` (`src/unit_49CF0.c`, 820 B):**
    - Dibujo de velocímetro y aguja digital en el HUD.
11. **`Garage_Draw` (`src/unit_53B30.c`, 940 B):**
    - Renderizado de vehículos en la interfaz de selección de garaje.
12. **`LoadGame_SyncProfiles` (`src/unit_53B30.c`, 620 B):**
    - Sincronización de perfiles de guardado y desbloqueos en memoria.
13. **`Fsm_Update` (`src/unit_69E80.c`, 648 B):**
    - Máquina de estados finitos del flujo de juego y menús.
14. **`Snd_PlaySongAtMarker` (`src/unit_6E610.c`, 412 B):**
    - Disparo de pistas de música en marcadores de carrera.
15. **`SndSong_Start` (`src/unit_6E610.c`, 524 B):**
    - Inicialización de secuencias MIDI y canales de audio.
16. **`Gfx_LoadTexture` (`src/util.c`, 340 B):**
    - Carga y configuración de texturas por DMA.

---

## Hipótesis Descartadas y Causas Raíz (Funciones Pendientes)

De acuerdo con la regla de 3 intentos máximos por función de `AGENTS.md`, las siguientes funciones fueron investigadas y pasaron a la lista de pendientes (`tools/match/PATTERNS.md`):

### 1. `Model_SetWheel` (`src/unit_06050.c`, 140 B, marco=0x40)
- **Hipótesis 1:** Reordenar declaraciones de parámetros float vs enteros para alterar el emparejamiento de registros en GCC 2.7.2.
  - *Resultado:* GCC 2.7.2 asigna `$fs0-$fs2` en orden inverso a la ROM original, persistiendo `$s1/$s0` invertidos.
- **Hipótesis 2:** Usar variables locales intermedias para forzar el enmascaramiento temprano `andi $s0, $a0, 0xFF`.
  - *Resultado:* El enmascaramiento se realiza correctamente pero la llamada intermedia a `Model_SetWheelContact` altera el orden de guardado de argumentos en pila (`0x10($sp)` vs `0x14($sp)`).
- **Hipótesis 3:** Empaquetar los últimos 4 argumentos en un `struct WheelContactParams` pasado por valor.
  - *Resultado:* Rompe el paso de parámetros en registros temporales `$a2/$a3` esperado por `Model_SetPartRotation`.

### 2. `Quat_Mul` (`src/unit_48FA0.c`, 384 B, marco=0x48)
- **Hipótesis 1:** Implementación de multiplicación de cuaterniones según algoritmo de Shoemake (9 multiplicaciones escalares).
  - *Resultado:* Genera diferencias de scheduling de registros FPR `$fs0-$fs4` y acceso al marco de argumentos del llamador (`0x48($sp)`).
- **Hipótesis 2:** Descomponer en productos vectoriales (dot y cross product) con inline functions.
  - *Resultado:* Desalinea el orden de cargas `ldc1`/`sdc1` de constantes de .rodata.
- **Hipótesis 3:** Reescribir con variables escalares directas `x, y, z, w` y uniones en stack.
  - *Resultado:* Diferencia en 16 instrucciones de reordenamiento de registros callee-saved.

### 3. `SndSrc_Play` (`src/unit_01050.c`, 420 B, marco=0x48)
- **Hipótesis 1:** Reestructuración del `switch-case` con tabla de saltos (`jtbl_800FE870`).
  - *Resultado:* GCC 2.7.2 unifica las cargas del global `gSndSrcList` de forma distinta a la ROM, asignando `$s0` en lugar de `$s1`.
- **Hipótesis 2:** Cargas locales forzadas en cada rama del `switch`.
  - *Resultado:* Incrementa el tamaño del prólogo en 4 instrucciones.
- **Hipótesis 3:** Separar la lógica de asignación de canal de voz en una función inline auxiliar.
  - *Resultado:* El compilador inserta un spill adicional de pila.

### 4. `GfxPools_Init` (`src/gfx_pools.c`, 416 B, marco=0x0)
- **Hipótesis 1:** Inicialización secuencial de las tres estructuras de pools (`Gfx`, `Vtx`, `Mtx`).
  - *Resultado:* GCC 2.7.2 combina los offsets en un solo bloque básico con reutilización de punteros base distinta a la ROM.
- **Hipótesis 2:** Unroll manual de asignaciones de buffers dobles.
  - *Resultado:* Invierte la emisión de las instrucciones `sw` en el slot de retardo.
- **Hipótesis 3:** Inicializar mediante punteros temporales locales a arrays de pools.
  - *Resultado:* Desplaza los registros `$a0` y `$a1` en el bucle principal.

### 5. `PauseUi_DrawText` (`src/unit_6AE90.c`, 352 B, marco=0x40)
- **Hipótesis 1:** Reordenar el paso de callbacks indirectos de dibujo a través de punteros de función.
  - *Resultado:* GCC 2.7.2 guarda registros `$s0-$s7` en orden disimilar al prólogo original.
- **Hipótesis 2:** Iteración de caracteres vía índice vs aritmética de punteros `text++`.
  - *Resultado:* Altera la comparación de fin de cadena `\0` en una instrucción de rama.
- **Hipótesis 3:** Variables locales en registros explícitos (`register char c`).
  - *Resultado:* Mantiene el diff en el bucle de kerning.

### 6. `Hud_DrawSlideFromLeft` (`src/unit_49CF0.c`, 452 B, marco=0x40)
- **Hipótesis 1:** Reestructuración con bucle `do-while` tras comprobación inicial `if (*s != 0)`.
  - *Resultado:* Expansión de prólogo a 476 bytes y bifurcación adicional de salida con inversión persistente de `$s1/$s2`.
- **Hipótesis 2:** Paso de argumentos explícitos (`s32 n, s32 cnt`) y bifurcación explícita con `goto kerning` tras el dibujo del carácter.
  - *Resultado:* Coincidencia de tamaño exacto (452 B, 71/113 instrucciones iguales), pero GCC preserva el cálculo modular `divu` en un orden de basic blocks inverso a la ROM.
- **Hipótesis 3:** Variables en registro explícito (`register u16 x;`) y pre-incremento en condición `(u8)++cnt % (u8)n == 0`.
  - *Resultado:* El compilador optimiza el marco a 440 bytes y omite el spill de `$s4` (`m->y`).

### 7. `Hud_DrawSlideFromRight` (`src/unit_49CF0.c`, 440 B, marco=0x40)
- **Hipótesis 1:** Implementación idéntica/simétrica a la variante 2 de `Hud_DrawSlideFromLeft` con decremento horizontal.
  - *Resultado:* 63/110 instrucciones iguales (420/440 B); el orden de evaluación del límite derecho `m->x >= m->x1` frente a la condición de centrado genera branches alternos.
- **Hipótesis 2:** Desplazamiento aritmético `m->speed` con sub-píxeles antes del salto condicional.
  - *Resultado:* Altera el orden de almacenamiento de coordenadas en el epílogo.
- **Hipótesis 3:** Uso de punteros locales temporales a la marquesina.
  - *Resultado:* Mantiene inversión de registros temporales `$17` y `$18`.

### 8. `Controls_SetupTexts` (`src/unit_53B30.c`, 912 B, marco=0x78)
- **Hipótesis 1:** Asignación interna de puntero base de menús `w = &D_800F5F4C;` dentro del bucle.
  - *Resultado:* GCC 2.7.2 recarga repetidamente la dirección mediante `lui/addiu`, desalineando `$s7` y `$fp`.
- **Hipótesis 2:** Puntero base exterior con reordenamiento de incrementos en la cabecera del `for` (`y += 50, k++, row++`).
  - *Resultado:* Gran avance a 198/231 instrucciones iguales (924/912 B), pero el incremento de fila (`addiu $s5, 0x1`) se emite antes de la bifurcación en lugar de en su delay slot.
- **Hipótesis 3:** Separación de las comprobaciones de remapeo `gControlsRemapP1` y `gControlsRemapP2` en bloques if aislados.
  - *Resultado:* Introduce un spill de pila adicional en `$sp+0x30`.

### 9. `Gfx_SetPerspective` (`src/unit_103B0.c`, 708 B, marco=0x40)
- **Hipótesis 1:** Ajuste del layout de `MtxPool` a 0x90 bytes para igualar el escalado `sll 3; addu; sll 4`.
  - *Resultado:* El bloque de comprobación de memoria del pool coincide exactamente con la ROM original.
- **Hipótesis 2:** Eliminación de variables temporales de visual list y escritura directa sobre `gDisplayListHead++`.
  - *Resultado:* 136/177 instrucciones iguales (696/708 B); GCC 2.7.2 consolida las 6 escrituras en una adición única final `addiu $2, $2, 48` en vez de actualizar el puntero global tras cada macro.
- **Hipótesis 3:** Casting explícito de `D_800F2124` a entero para forzar la emisión de `lw $a3` en el llamado a `guPerspective`.
  - *Resultado:* GCC reutiliza el registro flotante `$f4` mediante `mfc1` al detectar el cálculo previo de `fov`.

### 10. `Save_ReadAll` (`src/unit_50510.c`, 776 B, marco=0x48)
- **Hipótesis 1:** Inicialización secuencial y bucle con punteros directos sobre el buffer de memoria `0xDA0`.
  - *Resultado:* 120/194 instrucciones iguales (760/776 B); GCC omite la generación de instrucciones branch-likely (`beql`, `bnel`) y desalinea los cuatro punteros de inducción (`$s1`, `$s3`, `$s6`, `$s7`).

### 11. `Input_Init` (`src/unit_0B140.c`, 600 B, marco=0x40)
- **Hipótesis 1:** Puntero local `register InputLayout_ *m = &gInputLayoutCount;` y comprobación del botón START mediante `do-while`.
  - *Resultado:* 92/150 instrucciones iguales (572/600 B); GCC asigna `$s3` al puntero `m` en lugar de `$s0`, alterando la preservación de registros callee-saved.
- **Hipótesis 2:** Bucle de inicialización con puntero de slots `SlotC_ *s = gRumbleState; s->time = osGetCount()`.
  - *Resultado:* 110/150 instrucciones iguales (576/600 B); asigna `$s2` al puntero de slots en vez de fijar `$s2` a la dirección `&gInputLayoutCount + 0x54`.
- **Hipótesis 3:** Indexación plana directa y copia estructurada de 24 bytes desde rodata `D_800FEB20` en `0x10($sp)`.
  - *Resultado:* Coincidencia de 120/151 instrucciones iguales (604/600 B). La ROM original reutiliza `$s0` como acumulador de paso `s0 += 0x54` y emite saltos probables `beql` en la detección del botón START que GCC 2.7.2 bifurca en orden inverso.

### 12. `Cup_SetupRace` (`src/unit_53B30.c`, 508 B, marco=0x38)
- **Hipótesis 1:** Asignación secuencial de coches y puesta a cero de vueltas con puntero de perfil `(PlrBB_ *)gProfileP1`.
  - *Resultado:* 85/127 instrucciones iguales (484/508 B); el compilador emite saltos convencionales en lugar de saltos probables y difiere en el orden de evaluación de `sysRandRange(5)`.
- **Hipótesis 2:** Puntero explícito a número de vuelta actual `u8 *lap = &D_80123EC1`.
  - *Resultado:* Reserva `$s4` adicional en pila ($sp+0x30), aumentando el tamaño del marco de callee-saved.
- **Hipótesis 3:** Indexación aritmética directa con desplazamiento de bytes `(D_80123EC1 << 2)`.
  - *Resultado:* 78/127 instrucciones iguales (472/508 B); GCC reorganiza la carga base de `D_80123F8A` y desalinea los registros temporales.

### 13. `Snd3D_GetPan` (`src/unit_01050.c`, 484 B, marco=0x90)
- **Hipótesis 1:** Implementación vectorial completa con producto cruzado de orientación del oyente y cálculo trigonométrico con `sqrtf`.
  - *Resultado:* 42/121 instrucciones iguales (436/484 B); la ROM original utiliza 6 pares de registros FPU callee-saved preservados con `sdc1` (`$fs0-$fs5`), mientras que GCC 2.7.2 utiliza registros temporales `$ft0-$ft5` con marco de 64 bytes.
- **Hipótesis 2:** Desdoblamiento escalar de coordenadas (`cx, cy, cz`, `dx, dy, dz`, `rx, ry, rz`).
  - *Resultado:* 28/121 instrucciones iguales; reduce el marco de pila a 32 bytes y omite el salvado callee-saved en la FPU.
- **Hipótesis 3:** Estructura mixta con almacenamiento explícito de variables temporales en memoria local.
  - *Resultado:* 51/121 instrucciones iguales (416/484 B); persistencia de diferencias en la planificación de registros FPR de GCC 2.7.2 para el producto vectorial 3D.


---

## Directrices para Futuras Sesiones

1. **Continuar desde las funciones pendientes sin repetir los intentos agotados:**
   - Consultar `tools/match/PATTERNS.md` antes de iniciar cualquier candidata para no reintentar variantes descartadas.
   - Si se aborda una función pendiente de `PATTERNS.md`, debe plantearse una hipótesis estructural radicalmente distinta (p. ej. cambio en el layout de structs compartidos en `include/*.h`, flags del compilador en `Makefile`, o macros `#define` globales).
2. **Priorizar funciones pendientes con mayor probabilidad de coincidencia:**
   - `unit_49CF0`: `Hud_DrawSlideFromLeft` (452 B) y `Hud_DrawSlideFromRight` (440 B) — simetría de kerning de fuentes.
   - `unit_53B30`: `Cup_SetupRace` (508 B) y `Controls_SetupTexts` (912 B) — formateo de interfaz y asignación de textos.
   - `unit_103B0`: `Gfx_SetPerspective` (708 B) y `Gfx_SetViewport` (820 B) — cálculos geométricos de proyección.
3. **Reglas de integridad del proyecto:**
   - Confirmar commits únicamente con `./tools/match/commit_unit.sh "<mensaje>" <archivos>` respetando la verificación automática del SHA-1 y `verify_funcs.py`.
   - No realizar `git push`.
   - Mantener el código legible y semánticamente correcto sin trampas, inline asm simulado ni fake matches.

---

## Sesión 58 (2026-11-26)

- **Progreso oficial (`objdiff`):** **90.96 %** (`502,780 / 552,756 B`, `1,836 / 1,910` funciones, `186 / 221` unidades completas; **+1,936 B**, +2 funciones exactas, +1 unidad cerrada: `unit_01050`).
- **REA/Ghidra:** `prepare_elf.py` OK, binario `rea` no disponible en `PATH`; análisis en ASM + `src/`.
- **Matches exactos integrados:**
  1. `SndSrc_Play` (`src/unit_01050.c`, **+420 B**, cierra `unit_01050.c`): cálculo muerto `s32 v = s->v * 128.0f * vol; if (v == 0) v = 1;` entre `Snd_IsPlaying` y `if (busy != 0)` + `extern AudioBlock_ D_800EFD88;`.
  2. `CupResults_SetupTexts` (`src/unit_53B30.c`, **+1,516 B**): `static inline u8 *Cup_AddOff_` evita la reasociación en `D_80123F60[i].lap[D_80123EC1]`, `s32 *d = D_800F60E8; u8 *lap = &D_80123EC1;` fija el hoisting `$t2/$t1/$t0`, y firmas reales `u16 Time_Seconds/Time_Hundredths` y `u8 Cup_GetPosition`.
- **Avance `NON_MATCHING`:** `Hud_DrawTurbos` (`src/unit_49CF0.c`, 1,528 B) pasa de 374/382 (1,524 B) a **382/382 enmascaradas / 377/382 exactas** (1,528/1,528 B); único residual: 5 offsets de spill de `turbos` (`0x37($sp)` vs `0x1F($sp)` por `u8 buf[24]`).
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py` (1,814 funciones OK), `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.

---

## Sesión 59 (2026-11-26)

- **Progreso oficial (`objdiff`):** **90.96 %** (`502,780 / 552,756 B`, `1,836 / 1,910` funciones, `186 / 221` unidades completas; sin cambio oficial en bytes).
- **REA/Ghidra:** Activo vía `tools/rea/rea_query.py` (`~/.nvm/versions/node/v24.13.0/bin/rea`); consultado para todas las funciones objetivo (`Hud_DrawTurbos`, `Hud_DrawSlideFromLeft`/`Right`, `Password_Decode`, `Mesh_BuildDisplayList`, `Cup_SetupRace`).
- **Avance `NON_MATCHING`:**
  - `Cup_SetupRace` (`src/unit_53B30.c`, 508 B): pasa de `96/127` (`496/508 B`) a **126/127 (508/508 B exactos)** mediante `((CupRace_ *)&D_80123EC1)->race` (`MEM_IN_STRUCT_P = 1` para el reload en `$a0`), duplicación de `D_80123F60[2].lap[...] = 0` en ambas ramas de `cup == 2`, y `ofs = i * sizeof(CupRec_)` + `CupSetup_AddOff_` en `else`. Único residual: 1 posición de scheduling (`subu $v1, $v1, $a0` en `0x1A8` vs `0x1B0`).
- **Diagnósticos RTL/permuter sin cambio en `src/`:** `Hud_DrawTurbos` (382/382 enmascaradas, 377/382 exactas), `Hud_DrawSlideFromLeft`/`Right` (84/113 y 80/110), `Password_Decode` (318/326, + rejilla 20 variantes + permuter 35 s), `Mesh_BuildDisplayList` (591/677).
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py` (1,814 funciones OK), `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.

---

## Sesión 60 (2026-11-26)

- **Progreso oficial (`objdiff`):** **91.11 %** (`503,596 / 552,756 B`, `1,837 / 1,910` funciones, `187 / 221` unidades completas; **+816 B**, +1 función exacta, +1 unidad cerrada: `unit_20830`).
- **REA/Ghidra:** Activo vía `tools/rea/rea_query.py` (`~/.nvm/versions/node/v24.13.0/bin/rea`); consultado en una sola llamada para `Cup_SetupRace`, `Hud_DrawTurbos` y `SortQueue_Sort`.
- **Match exacto integrado:**
  1. `SortQueue_Sort` (`src/unit_20830.c`, **+816 B**, cierra `unit_20830.c`): quitar `do { ... } while (0)` de `#define SORTQ_SWAP(i, j)` (evita `NOTE_INSN_LOOP_BEG`/`END` en GCC 2.7.2 que creaban `move $4, $9` y frenaban `addiu $t0, -1`), mover `sp += 2;` antes del `if (r - i + 1 >= j - l)` y ordenar `i = l + 1; j = r; pivot = q->c[l];`.
- **Diagnósticos sin cambio en `src/`:** `Cup_SetupRace` (`126/127`, variantes `i * 11` / casts / permuter 30 s sin mover `subu` de `0x1B0` a `0x1A8`), `Hud_DrawTurbos` (`377/382` exactas, `382/382` enmascaradas; confirmado que cualquier `buf[>=8]` desplaza `0x1F($sp)`).
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py` (1,814 funciones OK), `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.

---

## Sesión 62 (2026-11-26)

- **Progreso oficial (`objdiff`):** **91.54 %** (`505,980 / 552,756 B`, `1,840 / 1,910` funciones, `187 / 221` unidades completas; **+1,608 B** en esta sesión, +2 funciones exactas).
- **REA/Ghidra:** Activo vía `tools/rea/rea_query.py` (`~/.nvm/versions/node/v24.13.0/bin/rea`); consultado en una sola llamada para `Race_ReadPlayerControls`, `Mesh_GetMaterialList` y `Coll_SyncObjects`.
- **Matches exactos integrados:**
  1. `Mesh_GetMaterialList` (`src/unit_19D40.c`, **+392 B**): `u32 m;` compartido entre ambos bucles + separación de `case 0:`..`case 12:` en el `switch` (aumenta refs de `p` antes de `jump2` para que `p` gane `$t0`, `m` quede en `$t1` y `out + count` reciba `$t0` en el segundo bucle).
  2. `Coll_SyncObjects` (`src/unit_39C90.c`, **+1,216 B**): reemplazar `static inline TransformShape_` por `#define TRANSFORM_SHAPE_(sp) { ... }` sin `do { ... } while (0)` compartiendo `s`, `m[12]`, `i`, `src`, `dst` a ámbito de función (**304/304 exacto**).
- **Diagnóstico sin cambio en `src/`:** `Race_ReadPlayerControls` (`src/unit_2C6A0.c`, `194/211`, `844/844 B`); agotado presupuesto (variantes de clamp/helpers + `greg` + permuter 25 s; `b` toma preferencia `$f0` y desplaza `1.0f` a `$f6`).
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py` (1,814 funciones OK), `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.

---

## Sesión 63 (2026-11-26)

- **Progreso oficial (`objdiff`):** **91.54 %** (`505,980 / 552,756 B`, `1,840 / 1,910` funciones, `187 / 221` unidades completas; sin cambio oficial en bytes).
- **REA/Ghidra:** Activo vía `tools/rea/rea_query.py` (`~/.nvm/versions/node/v24.13.0/bin/rea`); consultado en una sola llamada para `Hud_DrawSlideFromLeft`, `Hud_DrawSlideFromRight`, `Mesh_BuildDisplayList` y `Coll_ProbeWheels`.
- **Avance `NON_MATCHING`:**
  - `Coll_ProbeWheels` (`src/unit_39C90.c`, 4,244 B): sube de `835/1061` a **887/1061** (+52 palabras) al corregir `p0 = q0*t + q2*s` y `p1 = q3*s + q1*t`. El volcado `-dL` (`loop.c`) identifica la causa exacta del residual (`0x3F0` vs `0x400`): en Pass 2, `loop.c` agota el presupuesto de hoisting 1 instrucción antes de sacar `id*12` (`Loop 319`) y `$sp+0x10` (`Loop 279`), dejando `($sp+0x10)+id*24` en `$s7` en vez de hacer spill a `0x394($sp)`.
- **Diagnósticos sin cambio en `src/`:**
  - `Mesh_BuildDisplayList` (`src/unit_19D40.c`, `591/677`, `2708/2708 B`): confirmado por remapeo que las **677/677** instrucciones son exactas salvo la rotación `$s0..$s6` (`curTex` en `$s0` vs `$s6`); variantes de tipo/bucle/`register` sin efecto.
  - `Hud_DrawSlideFromLeft`/`Right` (`src/unit_49CF0.c`, `84/113` y `80/110`): `-df`/`-dg` confirma que `e` (`24 refs / 126 insns`) supera a `s` (`14 refs / 71 insns`) en `global_alloc`, invirtiendo `$s1/$s2`.
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py`, `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.

---

## Sesión 65 (2026-11-26)

- **Progreso oficial (`objdiff`):** **91.70 %** (`506,872 / 552,756 B`, `1,842 / 1,910` funciones, `187 / 221` unidades completas; sin cambio oficial en bytes).
- **REA/Ghidra:** Activo vía `tools/rea/rea_query.py` (`~/.nvm/versions/node/v24.13.0/bin/rea`); consultado en una sola llamada para `Coll_ProbeWheels`, `Coll_PointsVsWorld` y `Coll_BoxCornersVsWorld`.
- **Avances `NON_MATCHING` (`src/unit_39C90.c`):**
  - `Coll_PointsVsWorld` (5,232 B): sube de `969/1311` (5,244 B) a **1258/1308 (5,232/5,232 B exactos, +289 palabras)** quitando `do { ... } while (0)` de `VEC3_SCALE_M`/`CHECK_WALL_PVW`, compartiendo `i`/`vp`/`c`, corrigiendo el orden de operandos de `Vec3_Cross_P_`, usando `if (k != 0)` y producto punto directo en `wn * wnorm[k]`. Residual: 50 palabras puramente de asignación de registros (`startVert`/`roomIdx` `$fp`↔`$t4`, `$fv0/$fv1` y `$ft1/$ft3`).
  - `Coll_BoxCornersVsWorld` (6,472 B): sube de `992/1618` (6,408 B) a **1481/1618 (6,472/6472 B exactos, +489 palabras)** quitando `do { ... } while (0)`, cambiando `pd0s`/`pd1s` a `s32`, compartiendo `c` y usando `for (k = 0; k < 4; k++)` con productos punto directos sobre `wnorm[k]` + `if (k != 0)`. `0x0000..0x04C4` y `0x058C..0x0E0C` coinciden al 100 %; residual de 137 palabras puramente de asignación FPR.
- **Diagnóstico sin cambio en `src/`:** `Coll_ProbeWheels` (`887/1061`, 4,228/4,244 B; `DotPW_(&carUp[id], &tmp)` iguala 4,244/4,244 B y cierra Pass 1 al 100 % + hoist de `&carUp[0]`, pero queda en `886/1061`).
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py`, `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.

---

## Sesión 66 (2026-11-26)

- **Progreso oficial (`objdiff`):** **92.65 %** (`512,104 / 552,756 B`, `1,843 / 1,910` funciones, `187 / 221` unidades completas; **+5,232 B**, +1 función exacta).
- **REA/Ghidra:** Activo vía `tools/rea/rea_query.py` (`~/.nvm/versions/node/v24.13.0/bin/rea`); consultado en una sola llamada para `Coll_PointsVsWorld`, `Coll_BoxCornersVsWorld` y `Password_Decode`.
- **Match exacto integrado:**
  1. `Coll_PointsVsWorld` (`src/unit_39C90.c`, **+5,232 B**, `1308/1308` exacto): `dist = Vec3_Dot_P_(&objRel, &pl.n) + pl.d; if (dist < obj->radius)` cierra `0x194..0x20C`, `f32 vn;` compartido entre ambos bucles cierra `vrot2` (`0x1190..0x1254`), y reutilizar `j` en vez de `k` en `for (j = 0; j < 4; j++)` conserva `sltu $v0, $t2, $v0` en `0x22C` e intercambia `roomIdx`/`startVert` (`$fp`↔`$t4`).
- **Avance `NON_MATCHING`:**
  - `Coll_BoxCornersVsWorld` (`src/unit_39C90.c`, 6,472 B): sube de `1481/1618` a **1566/1618** (+85 palabras, `6472/6472 B` exactos) compartiendo `f32 vn;` entre ambos bucles (cierra `vrot1`/`vrot2` al 100 %) y usando `static inline f32 SubAbs_B_` con `wd[idx] = SubAbs_B_(d, Vec3_Dot_B_(&wn, &ax2))` (cierra `ax1`, `ax2` y `0xF1C..0xF24` al 100 %; residual: solo 13 palabras × 4 paredes en `ax0` de `CHECK_WALL_BCW`).
- **Diagnóstico sin cambio en `src/`:** `Password_Decode` (`src/unit_50510.c`, `324/326`; variantes de tipo/retorno/CSE en `Password_FindChar_`/`Password_DecodeChar_`/`Password_Wrap32_` no reducen las 2 palabras residuales).
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py` (1,814 funciones OK), `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.

---

## Sesión 67 (2026-11-26)

- **Progreso oficial (`objdiff`):** **93.82 %** (`518,576 / 552,756 B`, `1,844 / 1,910` funciones, `187 / 221` unidades completas; **+6,472 B**, +1 función exacta).
- **REA/Ghidra:** Activo vía `tools/rea/rea_query.py` (`~/.nvm/versions/node/v24.13.0/bin/rea`); consultado en una sola llamada para `Coll_BoxCornersVsWorld`, `Coll_ProbeWheels` y `Ai_Step`.
- **Match exacto integrado:**
  1. `Coll_BoxCornersVsWorld` (`src/unit_39C90.c`, **+6,472 B**, `1618/1618` exacto): quitar el temporal `f32 d;` de `CHECK_WALL_BCW` y asignar directamente `wd[idx] = Vec3_Dot_B_(&wn, &wv); wd[idx] = SubAbs_B_(wd[idx], ...)` cierra las 52 palabras residuales (13 palabras × 4 paredes en `ax0`).
- **Avances `NON_MATCHING`:**
  - `Coll_ProbeWheels` (`src/unit_39C90.c`, 4,244 B): sube de `887/1061` (`4228/4244 B`) a **899/1061** (`4244/4244 B` exactos) combinando `DotPW_(&carUp[id], &tmp)` en Pass 2 con `dist = ...; if (dist < o->b.rad)` en Pass 3.
  - `Ai_Step` (`src/unit_22440.c`, 13,120 B): borrador inicial completo bajo `#ifdef NON_MATCHING` recuperado con REA/Ghidra (**2512/3280 palabras**, `12712/13120 B`, frame `0x3C8` exacto; 21 bucles sobre `gAi[6]`).
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py` (1,815 funciones OK), `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.

---

## Sesión 68 (2026-11-26)

- **Progreso oficial (`objdiff`):** **93.82 %** (`518,576 / 552,756 B`, `1,844 / 1,910` funciones, `187 / 221` unidades completas; sin cambio oficial en bytes).
- **REA/Ghidra:** Activo vía `tools/rea/rea_query.py` (`~/.nvm/versions/node/v24.13.0/bin/rea`); consultado en una sola llamada para `Ai_Step`, `Hud_DrawTurbos`, `Coll_ProbeWheels` y `Cup_SetupRace`.
- **Avance `NON_MATCHING`:**
  - `Ai_Step` (`src/unit_22440.c`, 13,120 B): sube de `2512/3280` (`12712/13120 B`, `-408 B`) a **3037/3280** (**+525 palabras**, `13072/13120 B`, delta `-48 B`, frame `0x3C8` exacto). Claves: `gAi[i]` directo, firma `u8 Bld_StepRacer(..., PlaneAi_)` (16 B), struct `AiCfg_` (`D_80138D51..D_80138D57`), macro ternaria `FABS_`, `< 0.0f || > 0.0f` en Bucle 14, ramas invertidas en Bucles 4/13/18/20, `u8 willCrash` + `rate != 0.0f` por rama en Bucle 18 y `continue;` en Bucle 21. Bucles 1–2, 5–16 y 20–21 ya coinciden casi al 100 %.
- **Diagnósticos sin cambio en `src/`:**
  - `Hud_DrawTurbos` (`src/unit_49CF0.c`, `377/382`): confirmado que sin `u8 buf[24]` `turbos` cae en `0x1F($sp)` exacto (`-0x50`), mientras que cualquier array de 24 B en `expand` desplaza `$s0` y `turbos` juntos (`+0x18`).
  - `Cup_SetupRace` (`src/unit_53B30.c`, `126/127`): 14 variantes en el bucle `else` no mueven `subu $v1, $v1, $a0` de `0x1B0` a `0x1A8`.
- **Verificación:** `make`, SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`, `verify_funcs.py`, `check_no_asm.py` y `objdiff_report.py` (`report/us/report.json == build/us/report.json`) OK.
