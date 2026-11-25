# Sesión de Decompilación HWTR

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
