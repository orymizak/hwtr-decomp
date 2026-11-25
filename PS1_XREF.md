# Referencia cruzada con la versión PS1

La versión de PlayStation (`CCCPSX.EXE`, Stormfront 1998) comparte buena parte del código fuente con la de N64.
Su export de Ghidra (`CCCPSX.EXE.c`, nombres `FUN_8xxxxxxx`, sin símbolos) sirve como **segunda opinión** para
entender funciones difíciles: misma lógica, distinto compilador y CPU (PS1 usa punto fijo donde N64 usa `f32`).
No se copia código de ahí; se usa para entender control de flujo, tablas y campos.

Los archivos de PS1 no están en el repo (no son distribuibles). Regenerar con:

```sh
tools/ps1_xref.py <carpeta con CCCPSX.EXE y CCCPSX.EXE.c>                 # anclas
tools/ps1_xref.py <carpeta> 800b0000,800b0d00,80069400,8006a200            # ventana lado a lado
```

## Pares confirmados

| N64 | PS1 | Cómo se confirmó |
|---|---|---|
| `Password_Encode` (pendiente) | `FUN_80069ae4` | orden + mismas constantes (`0x13,0x45,0x25,0x2A,0x0B,0x12`), 18 caracteres + 2 de checksum |
| `Password_Decode` (pendiente) | `FUN_80069d78` | cadena `"TWJM"` + tabla de cheats |
| helpers de password (inline en N64) | `FUN_80069a18` (índice en charset), `FUN_80069a60` / `FUN_80069aa0` (rotación con estado `D_80123D2C`) | mismo charset `0123456789BCDFGHJKLMNPQRSTVWXYZ_` |
| `Hud_ShowTrick` (pendiente) | `FUN_80064dec` | cadenas `"%d %s"`, `"+%d %s"` |
| `Hud_Init` | `FUN_80061e04` | `actnfnt`, `actnovl1` |
| `Bld_Load` | `FUN_8007b19c` | `%s%d.bld`, `%s.bld` |
| `Pickup_TypeFromName` / `Pickup_GetHudIcon` | `FUN_80067dfc` | nombres de pickups |
| `Pickup_PickDef` | `FUN_80067b64` | `random`, `uncar1` |
| `CarNames_Load`, `Keyboard_LoadCharset`, `Lang_Load`, `Tuning_Load` | `FUN_80088204`, `FUN_8007fd44`, `FUN_8007fbcc`, `FUN_800800d0` | extensiones `.cdt`, `.chm`, `.hwt`, `Tuning.prm` |

Tentativo: `Hud_DrawResults` ↔ zona `FUN_800638ec`–`FUN_80063ab4` (PS1 separa el formateo de tiempo en su propia función).
El módulo HUD está en `0x80061e04`–`0x80065400` de PS1 en el mismo orden general que `unit_49CF0`, con funciones extra en PS1.

## Passwords de cheats

PS1 compara la entrada contra `"TWJM"` (maestro: activa el bit `0x20` del perfil) y contra una tabla de 24 códigos
`{nombre, máscara_unlocks, máscara_flags}`. N64 conserva solo algunos (columna N64 = la cadena existe en la ROM):

| Código | unlocks | flags | N64 |
|---|---|---|---|
| PWDTST | 0xFFFFFF | 0xFFFF | sí |
| WH1T3C4R | 0xFFFFFF | 0xFFFF | sí |
| J_D0GGY | 0x8000 | 0x200 | sí |
| F4TM4N | 0x2000 | 0 | sí |
| R4LF13 | 0x10000 | 0 | sí |
| 1H03 | 0x20 | 0 | sí |
| SW41TS | 0x20 | 0 | sí |
| V0LC4N0, C0L0MB0, M0_D, NZM4N, G10L1T0, 1G0R, TH4T_R0CKS, DA_H00D, H1GH_NRG, K_D0GG, THR4K4, N0SL33P, C1NDY, CR0NST4RR, M1N14CT10N, HW_T3ST3RS, SH0RTY_L | varios | varios | no |

Las máscaras de N64 pueden diferir; verificar contra la tabla de `Password_Decode` antes de documentarlas en `MECHANICS.md`.

## Qué no aporta

- Porcentaje de match directo: otra arquitectura y compilador.
- Nombres: el ejecutable PS1 no trae símbolos del juego (solo PsyQ: `CdInit`, `ApplyMatrixLV`, …).
- El `hotwheels.exe` del port es una recompilación estática a x86, no una decompilación.
