# Patrones de GCC 2.7.2 en este juego

Observados al igualar funciones a mano contra la ROM. El ejemplo indica una función donde se comprobó.

## Constantes y .rodata

- **`li.d` va a .rodata desde el ensamblador**, no desde `cc1`: no aparece en la salida de `-S` como dato. `tools/cc.py` lo cambia por `l.d` al símbolo de la ROM.
- **`u32` a `f32` o `f64` usa un doble oculto, 2^32.** El patrón es `cvt.d.w`, `bgez` y `add.d`. En C basta `(f32)x` con `x` de tipo `u32` (`func_800B2F30`).
- **`li.s` con `-G0` no usa .rodata.** Se expande a `lui`/`ori`/`mtc1`, así que las constantes `f32` se pueden escribir en C.
- **`x * 2.0` se compila como `add.d x, x`**, sin constante (`func_800DB2F8`).

## Tablas de saltos

- **Un rango cuenta como un solo caso.** `case 0: case 1: ...` con un mismo cuerpo no llega al mínimo para generar tabla, y GCC puede cambiarlo por aritmética sin saltos. Si la tabla de la ROM repite un destino, hay que escribir un cuerpo por caso: GCC los une después (`func_800BBAB4`).
- **Llamadas iguales en cada caso.** GCC deja solo los argumentos en cada caso y une la llamada al final (`func_800C68CC`).
- **Carga repetida en cada caso.** Si la ROM carga el mismo global en todos los casos, hay que asignarlo a una variable local en cada uno.
- **Etiquetas de la tabla.** Al pasar la función a C desaparecen sus `jlabel`. `tools/cc.py` vuelve a crear las etiquetas globales `jtbl_L...` que nombra la tabla de la ROM.

## Argumentos en pila y estructuras por valor

- **Registros guardados en la zona del llamador.** `sw $a1-$a3` a su propio hueco y `lwc1` después indican una **estructura por valor**, no un fallo de m2c (`func_80063468`, `func_800688F4`).
- **Devolver una estructura.** El puntero oculto llega en `$a0`, y `addu $v0, $a0, $zero` lo devuelve. El primer `f32` visible llega entonces en `$a1` (`func_800A9068`, `func_800A8670`).
- **Argumentos que m2c omite.** Una función que reenvía los cuatro registros y usa una ranura de pila suele tener cinco argumentos, aunque m2c muestre uno (`func_800CD8CC`).
- **Argumentos falsos.** Registros `$a0`/`$a1` que sobran de código anterior pueden parecer argumentos de una llamada. Si eso no cuadra, la función llamada puede no tener ninguno.
- **Lectura de bytes en la pila.** `lbu` en `0x13($sp)` es un `u8` en su ranura de 4 bytes, big-endian.

## Tipos y reparto de registros

- **Copia de estructura.** `*a = b->p` hace todas las cargas y después todos los guardados. Campo a campo, los intercala (`func_8006A368`).
- **`u8` como parámetro.** Con pocos usos se enmascara con `andi` en cada uso. Una variable local lo enmascara una sola vez al entrar.
- **Variable local antes de una llamada.** Calcular `arg + 1` antes cambia qué registro `$s` recibe cada valor (`func_800B32F4`).
- **`D + 2` frente a un símbolo propio.** Un campo de un struct externo puede acabar con la dirección en un registro `$s`. La ROM recarga con `lui`, así que era un símbolo aparte (`func_800BA864`, `D_800F6476`).
- **Un cast de ancho también cambia el código.** `*(u8 *)&D` puede cambiar registros o direcciones. Es mejor corregir el `extern` de la unidad (`func_800BBAB4`).
- **Prototipos nuevos en una unidad.** Afectan también a las funciones ya convertidas, por eso hay que comprobar siempre la ROM enlazada.

## Opciones de compilación

- **Parte de libultra se compiló con `-O3`.** En `__osPiGetAccess` y `__osSiGetAccess`, la función de crear la cola, que está
  en el mismo archivo, va insertada en línea. Con `-O3` en esas unidades (`unit_792B0`, `unit_7FD20`), la ROM sale idéntica.
  En el resto de unidades de libultra con C, `-O2` y `-O3` dan el mismo código, salvo en `unit_7DE70`, que mezcla
  varios archivos de la biblioteca gráfica y debe seguir con `-O2`.

## Comprobación

- **`hand.py` no conserva `#define`.** Al unir una función a su unidad se pierden las macros, así que los
  accesos a registros de hardware van escritos directamente, como `*(volatile u32 *)0xA4600010`
  (`__osEPiRawStartDma`). La comprobación de la ROM enlazada lo detecta.

Comparar sin reubicar no basta. `func_800BA864` y `func_800BBAB4` coincidían aisladas y cambiaban la ROM enlazada, por eso `commit_unit.sh` exige el SHA-1 y `verify_funcs.py`.

## Casi iguales, sin convertir

Solo se distinguen por el reparto de registros o por el orden de una carga: `func_800BA3A8`, `func_800658D4`, `func_80067284`, `func_800D0394`, `func_800D9D70`, `func_80095C60`, `func_800DBB10`, `func_800A8BA4`, `Hud_DrawSlideFromLeft` (`unit_49CF0`), `Snd3D_GetPan` (`unit_01050`), `Cup_SetupRace` (`unit_53B30`), `Controls_SetupTexts` (`unit_53B30`), `Gfx_SetPerspective` (`unit_103B0`).

### SortQueue_Sort (`src/unit_20830.c`)
- **Diff:** Quicksort non-recursive stack & pointer vs index difference, inner loop pointer advancement vs index math.
- **Root cause:** Sedgewick quicksort implementation uses pointer registers for `a0, a3, v1` arrays instead of recalculating indexed offsets, leading to different loop instruction schedule in GCC 2.7.2.

### Model_SetWheel (`src/unit_06050.c`)
- **Diff:** FPR register assignment ($fs0-$fs2 vs $f20-$f24) and GPR callee-saved register order ($s0/$s1 vs $s1/$s0).
- **Root cause:** GCC 2.7.2 caller/callee register spilling order for float arguments passed alongside integer stack arguments.

### Model_SwapPalette (`src/unit_06050.c`)
- **Diff:** Unused stack stores (sp0..sp2 debug/local variables optimized into stack frame) and bitwise shift optimization vs individual sub-expressions.
- **Root cause:** GCC 2.7.2 combined bitwise expressions vs original developer using local debug variables or a color struct that forced 8 bytes of stack.

### PauseUi_DrawText (`src/unit_6AE90.c`)
- **Diff:** Callee-saved register allocation ($s0-$s7, $fp) and temporary stack spills for argument variables vs in-register preservation.
- **Root cause:** GCC 2.7.2 register assignment differences when multiple helper function pointers are called in loops across basic blocks.

### SndCmd_SetEnvelopePreset (`src/unit_6E610.c`)
- **Diff:** Double constant symbol matching against .rodata (D_801015D0 vs emitted literal) and division control flow structure.
- **Root cause:** Function is listed in targets.py under `.rodata doubles: 13 functions`, requires rodata double matching infrastructure.

### Mesh_GetMaterialList (`src/unit_19D40.c`)
- **Diff:** Leaf function stack frame allocation (-8 bytes) and jump table label ordering / loop exit condition.
- **Root cause:** GCC 2.7.2 jump table emission and leaf register pressure leading to an 8-byte stack frame.

### SndSrc_Play (`src/unit_01050.c`)
- **Diff:** Callee-saved register assignment ($s0 vs $s1) in early global base load and array offset math for gSndSrcList - 0x378.
- **Root cause:** Register allocator in GCC 2.7.2 schedules loading globals and local struct copy sp18 differently across the initial switch-case basic blocks.

### LZ_AdvanceBits (`src/lz.c`)
- **Diff:** Temporary register assignment ($a2 vs $a1) under -O0 with register keyword.
- **Root cause:** GCC 2.7.2 -O0 allocates argument copies into consecutive caller registers, swapping $a1 and $a2 usage during addition.

### GfxPools_Init (`src/gfx_pools.c`)
- **Diff:** Instruction interleaving of pool structure field initializations across the 3 pool pairs (Gfx, Vtx, Mtx).
- **Root cause:** GCC 2.7.2 schedules array loads/stores and constant offsets in a single combined basic block with different register reuse patterns than separated per-pool assignments.

### Quat_Mul (`src/unit_48FA0.c`)
- **Diff:** Complex 4D quaternion product scheduling, caller argument stack spills, and 0.5f multiplication vs division by 2.0f.
- **Root cause:** Function writes to caller argument area (listed in `targets.py: writes to caller argument area: 4 functions`) and uses Shoemake 9-multiplication algorithm with extensive FPR callee saves.

### Hud_DrawSlideFromRight (`src/unit_49CF0.c`)
- **Diff:** Symmetrical partner to `Hud_DrawSlideFromLeft` (already documented), register allocation and inner loop scheduling differences in GCC 2.7.2.
- **Root cause:** Font character measurement loop with kerning and sub-pixel sliding arithmetic creates register pressure differences across GCC 2.7.2 basic blocks.

### Mesh_FillVtx (`src/unit_19D40.c`)
- **Diff:** Offset indexing arithmetic in inner vertex packing loop and register allocator spilling `vtxDst` into dual pointers ($10, $11) vs single base pointer ($t2).
- **Root cause:** GCC 2.7.2 groups structure field offsets and negative constants into callee-saved registers differently when pointer increments are intertwined with sub-byte field packing.

### Scroller_DrawLine (`src/unit_27A40.c`)
- **Diff:** Symmetrical text-measurement and character-draw loops with complex stack frame (0x70 bytes) passing 8 arguments to `Menu_DrawChar`.
- **Root cause:** Stack argument layout in callers with > 4 parameters combined with kerning lookahead loop register scheduling in GCC 2.7.2.

### _n_loadOutputBuffer (`src/unit_74AB0.c`)
- **Diff:** Audio microcode DMA command assembly, double constant reference (`D_801017A0`), fixed-point float to integer truncation and clipping.
- **Root cause:** Listed under `.rodata doubles: 13 functions` and stack arguments; N_Audio driver internal buffer streaming routine.

### ParticlePool_Update (`src/unit_1CC80.c`)
- **Diff:** Double precision constant (`D_800FF9F0`) physics timestep accumulation and particle pool linked list node updates.
- **Root cause:** Listed under `.rodata doubles: 13 functions` in `targets.py`.

### Lap_GetTimes (`src/unit_482C0.c`)
- **Diff:** 104/161 instrucciones iguales tras 3 variantes.
- **Root cause:** Operaciones de estimación de tiempo de carrera con división entera usando el multiplicador recíproco `0xCCCCCCCD`, conversiones flotante/entero con dobles ocultos de .rodata (`D_80100A50`, `D_80100A58`), y preservación de registros callee-saved ($s0-$s4, $fs0).

### unit_0B140:  (600 B, marco=0x40)
- **Diff:** 98/167 instrucciones iguales tras 3 variantes.
- **Causa raíz:** Múltiples bucles complejos de sondeo de mandos y rumble pak con desenrollado parcial y saltos con / generados por GCC 2.7.2. El registro base  comparte cálculo de dirección con  desplazado 0x54 bytes.

### unit_0B140: Input_Init (600 B, marco=0x40)
- **Diff:** 98/167 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Bucles de sondeo de mandos y rumble pak con desenrollado parcial y saltos con branches probables (beql/bnel) generados por GCC 2.7.2. El registro base $s0 comparte calculo de direccion con $s2 desplazado 0x54 bytes.

### unit_39C90: Coll_LoadLevel (636 B, marco=0x138)
- **Diff:** 127/160 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Scheduling de asignaciones de campos y bucles `do-while` sobre `rooms` con recarga de puntero global `gCollLevel` y registros callee-saved ($s0-$s2).

### unit_103B0: Gfx_UpdateLights (676 B, marco=0x28)
- **Diff:** 121/169 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Desenrollado y copia manual de 0x78 bytes de la estructura de luces `D_800F1DE0` más emisión directa a nivel de microcódigo de comandos `gSPLight` y `gSPNumLights` con scheduling de registros callee-saved $s0, $s1.

### unit_13490: Gfx_ClearFrame (668 B, marco=0x30)
- **Diff:** 126/190 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Emision por slots directos de 12 comandos RDP (pipe sync, scissor, fill rect, etc.) intercalada con llamadas a osVirtualToPhysical y manejo de argumentos de entrada en pila ($s1..$s6).

### unit_50510: Save_ReadAll (776 B, marco=0x48)
- **Diff:** 92/196 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Bucles desenrollados de copia de bloques (0x880 y 0x140 bytes) entre el búfer dinámico y los arreglos globales `gSaves` / `gSaveSlots`, más alta presión de registros callee-saved ($s0-$s7, $fp) y llamadas indirectas a la tabla `sysSaveRead`.

### unit_50510: Password_Encode (800 B, marco=0x18)
- **Diff:** 65/200 instrucciones iguales tras 1 variante.
- **Causa raiz:** Operaciones de empaquetado bit a bit (5 bits por carácter) con uniones en stack de 12 bytes temporales, variables escalares de 8 bits (u8/s8) con sign extension / andi 0xFF constante en MIPS, y permutación final de pares de bytes con arrays en .rodata (`D_800F601C`/`D_800F601D`).

### unit_4F640: RaceView_DrawScene (724 B, marco=0xB0)
- **Diff:** 140/181 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Persistencia de constantes flotantes en registros callee-saved FPR ($fs0, $fs1) a lo largo del bucle de vistas más llamada a `svcScreenSetViewport` con bifurcaciones optimizadas y 8 argumentos para `RaceCam_Get`.

### unit_31E10: Car_ComputeMass (752 B, marco=0x0)
- **Diff:** 70/202 instrucciones iguales tras 1 variante.
- **Causa raiz:** Función hoja (leaf function, marco=0) con aritmética pura en punto flotante MIPS y campos float/int embebidos en el struct `Car` a offsets 0x144-0x5FC con paso de 0x70 bytes por rueda. Requiere el layout exacto de campos de `CarWheel` para evitar desalineación de registros FPR.

### unit_13490: SceneNode_DrawMesh (700 B, marco=0x30)
- **Diff:** 149/175 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Micro-diferencia en comparación `sltiu` vs `slti` con 1 en la guarda de asignación de buffers de vértices/display lists de `Mesh_`, más la secuencia de emisión de los comandos GBI (`gSPMatrix`, `gSPVertex`, `gSPDisplayList`) que avanza `gDisplayListHead` por pasos de 8 bytes.

### unit_2C6A0: Race_ReadPlayerControls (844 B, marco=0x30)
- **Diff:** 130/223 instrucciones iguales tras 2 variantes.
- **Causa raiz:** 13 llamadas consecutivas a puntero de función indirecto `sysInputGetControl` a través del registro $s2/$s0 con argumentos escalares s32 y normalización `(val & 0xFF) / 255.0f`, más aritmética float de sensibilidad de dirección a alta velocidad con clamping cuádruple contra constantes de velocidad.

### unit_53B30: HighScores_SetupTexts (1080 B, marco=0x98)
- **Diff:** 144/270 instrucciones iguales tras 1 variante.
- **Causa raiz:** Rutina de división entera rápida mediante multiplicación unsigned (`multu` por 0xAAAAAAAB para dividir por 3) y módulo 3 para indexar la matriz 2D `D_800F9FE0[track/3][track%3]`, más switch-case con tabla/saltos para formateo de tiempos y nombres de records en `gActiveSave` con 12 argumentos por llamada a `Menu_SetText`.

### unit_22440: Ai_ApplyImpulse (1088 B, marco=0x40)
- **Diff:** 251/278 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Aritmética vectorial 3D (producto escalar del vector de impulso contra las 3 columnas de la matriz de rotación del coche en el stack `car->rot.m`), con cálculo de dirección base e indexado de `gAi[car->id]` (stride 0x2A8) en cada rama condicional y selección `MIN`/`MAX` con branches probables (`bc1tl`/`bc1fl`) de GCC 2.7.2.

### unit_39C90: Coll_SyncObjects (1216 B, marco=0x30)
- **Diff:** 175/325 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Transformación de vértices por matriz afín 3x4 en la pila (0x30 bytes) sobre la lista enlazada `gCollAll`, con iteración dual de punteros fuente/destino (`srcPoints`/`dstPoints`), lógica de umbral de velocidad lineal/angular con temporizador de suspensión (`phys->sleepTimer`) y bifurcación condicional según tipo de objeto de colisión (tipos 2, 3 y dinámicos).

### unit_01050: Audio_Init (1164 B, marco=0x18)
- **Diff:** 233/291 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Inicialización masiva en .bss de tablas de audio `gSongTable` y `gSfxTable` (125 entradas asignadas secuencialmente) con reutilización de constantes escalares compartidas en registros temporales ($t0, $v1, $a2, $a3) y preparación de punteros de wavebank en $a0/$a1 intercalada con los últimos almacenes en memoria.

### unit_06050: Model_UpdateWheelEffects (1140 B, marco=0xE0)
- **Diff:** 119/319 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Bucles anidados de efectos de rueda sobre coches activos con marco de pila masivo (0xE0 bytes), preservación de 10 registros callee-saved ($s0-$s7, $fp, $ra) y paso de 17 argumentos a `SkidMark_AddQuad` (13 argumentos en el caller stack) que fuerza desbordamiento de punteros locales a ranuras temporales de la pila.

### unit_39C90: CollBox_EdgeSeparatingAxis (1220 B, marco=0x98)
- **Diff:** 85/305 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Test de Separating Axis Theorem entre bordes y OBB 3D calculando vector delta relativo, proyección local mediante producto matricial y 7 comprobaciones condicionales de separación por eje retornando índices de eje (1, 2, 3, 5, 7, 10, 13) o 0.

### unit_1CC80: SkidMark_AddQuad (1188 B, marco=0xB0)
- **Diff:** 115/297 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Función receptora de 17 argumentos (13 en caller stack frame), copia en bloque de estructura de vértice Vtx[4] (0x40 bytes) con bucle no enrollado de 4 palabras, aritmética de distancia/longitud cuadrática 3D entre 4 vértices calculando radio máximo mediante `sqrtf`, y rotación de anillo en buffer circular de partículas (`gParticlesAlt`) con comprobaciones de solapamiento de punteros head/tail.

### unit_13490: Bg_DrawImage (1204 B, marco=0x28)
- **Diff:** 92/301 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Generador procedural de macrocomandos F3DEX2 DisplayList para dibujo de fondo en tiles de 64x32 píxeles con comprobación de desbordamiento en `gGfxPools`, ajuste de paleta TLUT de 256 colores, y bucles anidados en X/Y emitiendo comandos directos de carga de textura por DMA (`G_SETTIMG`, `G_SETTILE`, `G_LOADTLUT`, `G_TEXTURE_ENABLE`, `G_TEXRECT`).

### unit_0B140: Clouds_Init (1268 B, marco=0x40)
- **Diff:** 108/317 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Inicialización y distribución procedural de partículas de nubes en matriz 2D con llamadas repetidas a `Rand_Range`, cálculos de división float / 6.0f con clamping a 14000.0f, conversión de entero a float con sesgo de signo en double `D_800FEA30`, y bucle de carga secuencial de texturas con `sprintf(path, "cloud%d.raw", i)` y `File_Load`.

### unit_06050: Model_SetPartRotation (1276 B, marco=0x88)
- **Diff:** 142/319 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Transformación de coordenadas de piezas de modelo 3D con matrices afines 3x4 en la pila (0x30 bytes), rotaciones concatenadas en Y y X calculando cosf/sinf con ángulos negativos (`-arg2`, `-arg4`), traslación inversa respecto al centro de rotación (`sp40, sp44, sp48`), recorrido de jerarquía de sub-piezas en lista enlazada (`unk4C`), y escalado condicional si `D_80138AB1 == 1`.

### unit_50510: Password_Decode (1304 B, marco=0x40)
- **Diff:** 121/326 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Descodificador de contraseñas de desbloqueo con tabla de transposición de caracteres (`D_800F601C`/`D_800F601D`), suma de comprobación modular contra `gPasswordChars`, desempaquetado de secuencias de 5 bits a 8 bits con rotación/máscaras de bits (`D_800F6030`), y activación de banderas en bitfield `gUnlockBitMap` repartidas entre `arg1->unk14` y `arg1->unk18`.

### unit_6E610: SndVoice_ProcessEvents (1356 B, marco=0x40)
- **Diff:** 158/339 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Procesador de máquina de estados para secuencias MIDI/SND con tabla de despachadores de comandos de 7 bits (`gSndSeqCmds`), cálculo de envolventes ADSR con constante double `D_801015D8`, actualización de pitch/pan con interpolación de voz en libultra (`n_alSynSetVol`, `n_alSynSetFXMix`, `SndVoice_KeyOn`), y desbordamiento de variables de estado local en registros callee-saved.

### unit_53B30: Garage_SetupTexts (1388 B, marco=0x50)
- **Diff:** 206/347 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Configuración de textos del garaje para selección de coches de P1 y P2 (3 textos por jugador con `Menu_SetText`), con cálculo de coche previo/siguiente modular en rango [0, 40] usando bifurcaciones condicionales `bgez` y `slti`, verificación de desbloqueo (`Car_IsUnlocked`), y compartición de puntero estático de color $s5=0x64 reutilizado en las 12 llamadas directas.

### unit_2C6A0: RaceAudio_Update (1400 B, marco=0xB8)
- **Diff:** 112/350 instrucciones iguales tras 1 variante.
- **Causa raiz:** Despacho dinámico de callbacks de audio de carreras (`D_8014C138`, `audSfxPlay`, `audVoicePlayRandom`), temporizadores de espera de SFX/voces en `D_80118534`, saltos con delay slots optimizados en bucles de comprobación de volumen de motores y efectos de ambiente, e interpolación de volumen/frecuencia entre coches activos.

### unit_39C90: CollShape_InitCar (1500 B, marco=0x88)
- **Diff:** 148/375 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Inicialización geométrica de la envolvente de colisión del vehículo asignando dinámicamente memoria para puntos locales y transformados (`arg0->unk40`, `arg0->unk44`, `arg0->unk48`), generación de 8 vértices del OBB caja mediante combinaciones de signos (+/- half extents), transformación por matriz afín 3x4 (`Mtx34f`) para consulta espacial inicial con `Coll_FindRoom`, y cálculo de radio esférico envolvente `sqrtf`.

### unit_53B30: CupResults_SetupTexts (1516 B, marco=0x90)
- **Diff:** 162/379 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Presentación de resultados de copa con acumulación de puntos de torneo y tiempos por participante, división de formato según número de jugadores (`gProfileP1->unk22 == 2` vs individual), formateo secuencial de cadenas con `sprintf` (posición, nombre del piloto, puntuación, tiempo MM:SS:HH), y bucle de limpieza con texto vacío `D_80100CC8` para posiciones 3 a 5 en modo 2 jugadores.

### unit_49CF0: Hud_DrawResults (1520 B, marco=0x78)
- **Diff:** 134/380 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Dibujo de la pantalla de resultados de carrera con llamadas dinámicas a punteros de servicio gráfico (`svcDraw2DSetColor`, `svcFontDrawChar`), cálculo de kerning carácter por carácter (`Font_GetKerning`), lógica de formateo condicional según cuenta atrás (`gHudCountdown`), cálculo de tiempos parciales con división/módulo (minutos, segundos, centésimas) e iteración sobre `gHudRacerCount`.

### unit_49CF0: Hud_DrawTurbos (1528 B, marco=0x58)
- **Diff:** 149/382 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Renderizado de barras e iconos de turbos disponibles por jugador, lectura de estado del coche con `Car_Get`, consulta de temporizador de carrera `RaceClock_Get`, selección de paleta de colores RGB (`D_800F3B5C`/`D_800F3B5D`/`D_800F3B5E`), bucles de dibujo de pips activos y en recarga (iconos 0x3D a 0x47/0x48), y sonido de alerta `audSfxPlay(0x3C)` al llenar los 10 turbos.

### unit_13490: ModelData_Load (1556 B, marco=0x48)
- **Diff:** 153/389 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Cargador y reubicador en memoria de estructuras de modelos 3D (`WorldData_Relocate`, `EnvMap_Apply`, `PointLight_Alloc`), fijación de punteros relativos con offsets base (`arg0 + unk`), y construcción inline de DisplayLists F3DEX2 en memoria dinámica para mallas reflectantes (`unk12C`), sombras (`unk130`), y texturas de entorno (`Gfx_LoadTexture`).

### unit_53B30: MenuCar_Draw (1608 B, marco=0x58)
- **Diff:** 148/402 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Interpolación de posición/orientación y renderizado de coche en el menú (`Menu_InitViewMtx`, `Menu_DeltaTenths`, `Menu_DeltaSeconds`), cálculo de rotación periódica con `sinf`/`cosf` acumulativa sobre `gMenuCarPos`/`gMenuCarTarget` y constantes de ángulo de cámara (`-1.3962624f`), aplicación de matriz de vista (`sysCameraApply`) y dibujo (`sysModelDrawWithLookAt`).

### unit_21150: Bld_StepRacer (1892 B, marco=0x48)
- **Diff:** 135/473 instrucciones iguales tras 1 variante.
- **Causa raiz:** Máquina de estados con tabla de saltos (`jr $tX`) para la animación y físicas de corredores en vallas publicitarias/objetos del escenario, interpolación de coordenadas angulares y de escala con comprobación de solapamiento de límites espaciales, y despacho de efectos de sonido según el estado del corredor.

### unit_49CF0: Hud_ShowTrick (1984 B, marco=0x48)
- **Diff:** 188/496 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Animación y visualización en HUD de acrobacias/trucos realizados (`Trick_GetName`, `RaceClock_Get`, `Car_Get`), formateo de puntuaciones y bonus de turbo (`sprintf`, `strcpy`), medición de ancho y kerning carácter a carácter con ordenación burbuja de cadenas por longitud en la pila (`sp10`, `sp11`, `sp12`), y ajuste de coordenadas horizontales para centrado de texto dinámico.

### unit_22440: Ai_RestoreSlot (2108 B, marco=0x18)
- **Diff:** 185/527 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Restauración en memoria del contexto completo de un corredor IA (`gAi[car->id]`, stride 0x2A8), copia en bloque de 0x80 bytes con registros $a2/$a3, inicialización masiva de 25 campos de control/físicas a 0, y bucles de interpolación de puntos waypoint de trayectoria con stride 0xC sobre `D_80114918` y `D_80114994`.

### unit_06050: World_Draw (2120 B, marco=0xF8)
- **Diff:** 172/530 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Bucle principal de renderizado del escenario 3D con inversión condicional de perspectiva si `D_80138AB0 == 1`, transformación de matriz de cámara con cosf/sinf de ángulo fijo `0xBFC90FD0` (LookAt), dibujo de cielo/nubes (`SceneNode_Draw`, `Clouds_Draw`), pasadas duales de iluminación (`Gfx_SetAllLightColors`) y dibujo de modelos normales y con variantes (`ModelData_Draw`).

### unit_19D40: Mesh_BuildMaterialDL (2132 B, marco=0x48)
- **Diff:** 160/533 instrucciones iguales tras 1 variante.
- **Causa raiz:** Construcción procedural de macrocomandos F3DEX2 DisplayList para materiales con tabla de saltos (`jr $tX`) según tipo de material (textura mapeada, color plano, environment mapping, multitextura), emitiendo comandos directos de combiner, blender y asignación de texturas por DMA.

### unit_19D40: PakMenu_Run (2200 B, marco=0x70)
- **Diff:** 210/550 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Gestor interactivo del Controller Pak de N64 en frame buffer temporal (0x80300000), detección de errores de memoria y reparación (`nuContPakOpen`, `nuContPakRepairId`, `nuContPakFileNum`), bucle de eventos con lectura directa de palancas y botones (`nuContDataGetAll`), y renderizado bitmap de texto de depuración (`Debug_DrawString`).

### unit_31E10: Cars_Init (2320 B, marco=0xA0)
- **Diff:** 198/580 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Inicialización y spawn de los coches de la carrera en el array global `gCars` (stride 0x77C), obtención de punto de salida de pista (`Coll_GetStartPos`), cálculo de masa del cuerpo rígido (`RigidBody_Init`), conversión de cuaternión a matriz de rotación 3x3 (`Quat_ToMtx3`), configuración de IA o entrada de jugador humano (`Ai_InitRacer`), y adición al motor de colisiones (`Coll_AddCar`).

### unit_39C90: Coll_GetRoomAxis (2476 B, marco=0x98)
- **Diff:** 224/619 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Determinación de ejes geométricos y normales de salas adyacentes (`Coll_GetRoomAxis`), con lectura de planos de colisión (`gCollLevel->unkC8`, factor de escala 4096.0f), clasificación de paredes/suelos/techos (`unk-4` vs umbral 0.5f), ponderación de normales proporcionales a las distancias de penetración y normalización final del vector resultante con `sqrtf`.

### unit_22440: Ai_InitRacer (2700 B, marco=0x40)
- **Diff:** 215/675 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Configuración inicial del estado del corredor de IA en `gAi` (stride 0x2A8), cálculo de nivel de agresividad según dificultad (`arg2->unk9C`), generación de offsets pseudoaleatorios LCG (`D_80115854 * 0x19660D + 0x3C6EF35F`), inicialización enlazada con billboard (`Bld_InitRacer`), y relleno de matrices de waypoints de velocidad y dirección.

### unit_19D40: Mesh_BuildDisplayList (2708 B, marco=0x50)
- **Diff:** 182/677 instrucciones iguales tras 1 variante.
- **Causa raiz:** Construcción masiva de DisplayLists de geometría con tabla de saltos múltiple para tipos de primitivas (`G_TRI1`, `G_TRI2`, strips, quads), optimización de carga de vértices con caché de 32 vértices (`G_VTX`), y enlace de punteros a segmentos de memoria de materiales.

### unit_28C10: Trick_Evaluate (2876 B, marco=0x58)
- **Diff:** 205/719 instrucciones iguales tras 1 variante.
- **Causa raiz:** Máquina de evaluación y detección de trucos acrobáticos aéreos (flips, spins, rolls, barani) con tabla de saltos masiva (`jr $tX` en línea 602), cálculo de ángulos de cabeceo/alabeo acumulados y transiciones de estado de trucos con temporizadores de caída libre.

### unit_39C90: Coll_FindGroundPlanes (2888 B, marco=0x90)
- **Diff:** 241/722 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Búsqueda y ponderación de planos de apoyo/suelo para todos los vehículos activos (`gCollCars`), cálculo de producto vectorial normalizado (`sqrtf`) entre triángulos de polígonos de colisión de escenario (`gCollLevel`), y proyecciones de penetración contra la dirección vertical local del vehículo (`unk80`, `unk84`, `unk88`).

### unit_6C240: Pause_Input (2920 B, marco=0x28)
- **Diff:** 260/730 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Control y sondeo de entradas del menú de pausa para mandos 1 y 2 (`sysInputIsKeyHeld`, `sysInputUpdate`, `sysInputIsPad1Connected`), detección de flanco ascendente en botones direccionales y de acción (`D_800FA244`, `D_800FA248`, `D_800FA254`), reproducción de SFX de confirmación/cancelación (`audSfxPlay`) y transiciones de máquina de estados FSM (`Fsm_Trigger(&gPauseFsm)`).

### unit_31E10: Car_AirControl (3120 B, marco=0x110)
- **Diff:** 265/780 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Físicas de control acrobático aéreo del vehículo al despegar de rampas con gran stack frame (0x110 bytes), atenuación por rozamiento (`0.98f`), pares de torsión y momentos angulares proyectados según rotación del coche en los 3 ejes, restricciones de balanceo en cuerpo rígido (`RigidBody_ConstrainSpin`) y actualización de velocidades angulares acumuladas en `unk130`, `unk134`, `unk138`.

### unit_39C90: CollBox_FindSeparatingAxis (3176 B, marco=0x80)
- **Diff:** 305/794 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Implementación completa del teorema del eje separador (SAT) 3D entre dos cajas orientadas (OBBs), calculando matriz de rotación relativa 3x3 en la pila (`sp28` a `sp48`), vector de traslación relativo (`sp70`, `sp74`, `sp78`), y evaluando secuencialmente los 15 ejes potenciales (3 ejes de la caja A, 3 ejes de la caja B y 9 productos vectoriales cruzados entre aristas A x B) con un epsilon de `0.001f`.

### unit_01050: CarSnd_Update (3296 B, marco=0x90)
- **Diff:** 290/824 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Actualización en tiempo real de motores, derrapes y efectos 3D de vehículos (`CarSnd_Update`), con cálculo de distancias al oyente (`Vec3_DistSq`), atenuación acústica (`Snd3D_GetVolume`), paneo estéreo (`Snd3D_GetPan`) y efecto Doppler (`Snd3D_GetDoppler`), ajuste simultáneo de pitch/volumen para canales de sonido (`gCarSnd`, `D_800EFF3C`, `D_800EFF40`, `D_800EFF44`), y recorrido de lista ligada de emisores ambientales `gSndSrcList`.

### unit_39C90: Coll_ClipCamera (3408 B, marco=0xC8)
- **Diff:** 278/852 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Algoritmo de evitación de colisión y recorte de cámara respecto a la geometría del circuito (`Coll_ClipCamera`), con cálculo de salas conectadas (`gCollLevel->unkC0`), proyección sobre planos de pared y techo con umbral de seguridad de 48.0f unidades (`temp_fv1 < 48.0f`), desplazamiento acumulado del punto de vista y amortiguación elástica de cámara en los 3 ejes.

### unit_13490: ModelData_Draw (3600 B, marco=0x118)
- **Diff:** 312/900 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Bucle de dibujo de mallas de vehículos y objetos (`ModelData_Draw`) con cálculo de niveles de detalle (LOD 0 a 3: `gLodDist0` a `gLodDist3`), frustum culling de esferas envolventes (`Frustum_TestSphere`), dibujo de sombras dinámicas proyectadas (`ModelData_DrawShadow`), asignación de matrices en display list (`Mtx_New`, `Mtx34_Mul`), y encolado en cola de ordenación translúcida (`SortQueue_Add`).

### unit_31E10: Car_Simulate (3616 B, marco=0x88)
- **Diff:** 320/904 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Bucle de integración física y dinámica vehicular (`Car_Simulate`) con cálculo de sustentación y resistencia aerodinámica (`Car_ApplyAero`), promedio de puntos de contacto de ruedas contra el suelo (`Car_AverageGroundContacts`, `Car_UpdatePoints`), descomposición de fuerzas normales, de fricción lateral y de frenado/tracción por cada rueda (`unk3E4`), e integración de momentos angulares y fuerzas lineales.

### unit_39C90: Coll_BoxVsWorldEdges (3784 B, marco=0xE8)
- **Diff:** 345/946 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Detección y resolución de colisión entre cajas OBB de objetos dinámicos y aristas del mundo (`CollBox_EdgeSeparatingAxis`), proyección de vértices extremos y aristas tangentes contra los planos de corte de cada arista de celda de colisión (`gCollLevel->unkCC`), amortiguación de penetración y generación de contactos en buffer circular global `gCollContacts`.

### unit_39C90: Coll_ProbeWheels (4244 B, marco=0x3B0)
- **Diff:** 380/1061 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Raycasting y suspensión de ruedas contra la geometría del circuito (`Coll_ProbeWheels`) para todos los corredores en `gCollRacers` con marco de pila enorme (0x3B0 bytes), interpolación de recorrido de suspensión (compresión y rebote) contra normales de terreno en `unk174`-`unk18C`, cálculo de penetración límite `sqrtf` y amortiguación elástica de muelles.

### unit_39C90: Coll_PointsVsWorld (5232 B, marco=0x230)
- **Diff:** 420/1308 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Verificación masiva de vértices de colisión de cuerpos rígidos (`gCollBodies`) contra la malla espacial de celdas (`Coll_PointsVsWorld`), transformación por matriz afín de coordenadas locales a mundiales, test punto-plano contra todos los polígonos de celda activa, acumulación de impulsos de choque y desvío elástico (`-var_t1->unkB8`).

### unit_39C90: Coll_BoxCornersVsWorld (6472 B, marco=0x210)
- **Diff:** 460/1618 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Algoritmo exhaustivo de colisión entre los 8 vértices esquinas de cajas orientadas de vehículos (`gCollPathCars`) y planos del escenario, cálculo de vectores de separación con normalización `sqrtf`, generación de puntos de contacto en `gCollContacts` (hasta 16 contactos), comprobación de daño estructural y corrección de penetración en coordenadas mundiales.

### unit_22440: Ai_Step (13120 B, marco=0x378)
- **Diff:** 680/3280 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Función monolítica masiva de IA y conducción autónoma para hasta 6 corredores (`Ai_Step`), con marco de pila de 0x378 bytes, navegación de splines/rutas (`Bld_StepRacer`), detección de colisiones inminentes y maniobras evasivas, control de aceleración/frenado según agarre de neumáticos, nivelación tras saltos (`Mtx3_Orthonormalize`), rubber-banding respecto al jugador humano según `RaceClock_Get` y constantes `D_80138D53`.

### unit_06050: Model_SetWheel (140 B, marco=0x40)
- **Diff:** 16/35 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Reparto de registros FPR ($fs0-$fs2 frente a $f20-$f24) y orden de guardado/restauración de registros GPR callee-saved ($s0/$s1 frente a $s1/$s0) al recibir y desempaquetar argumentos pasados en la pila del llamador (`0x50` a `0x64`).

### unit_06050: Model_SwapPalette (324 B, marco=0x8)
- **Diff:** 21/83 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Variables locales temporales no usadas (`sp0`..`sp2`) guardadas en el marco de pila de 8 bytes de depuración en la ROM original, y combinación de desplazamientos/máscaras de color de 16 bits (RGB5551) que GCC 2.7.2 optimiza o reordena distinto a la implementación manual.

### unit_6AE90: PauseUi_DrawText (352 B, marco=0x40)
- **Diff:** 58/92 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Reparto de registros callee-saved ($s0-$s7, $fp) y orden de cálculo del ancho de texto centrado con llamadas consecutivas a punteros de función (`svcFontGetCharWidth`, `Font_GetKerning`, `svcDraw2DSetColor`, `svcFontDrawChar`).

### unit_48FA0: Quat_Mul (384 B, marco=0x48)
- **Diff:** Producto de cuaterniones 4D con algoritmo de Shoemake (9 multiplicaciones), paso de argumentos flotantes en registros y pila de llamador, y escritura en la zona de argumentos propios ($a1, $a2, $a3 guardados en `0x4C`, `0x50`, `0x54`).
- **Causa raiz:** Alta presión de registros flotantes callee-saved ($fs0-$fs5) y división por 2.0f constante inmediata que GCC 2.7.2 compila con scheduling específico de FPU.

### unit_19D40: Mesh_GetMaterialList (392 B, marco=0x0)
- **Diff:** Función hoja con tabla de saltos (`jtbl_800FF8A0`) de 13 entradas para comandos de materiales de malla.
- **Causa raiz:** `m2c` no soporta la tabla de saltos sin etiquetas rodata enlazadas; en compilación manual el orden de casos y la optimización de saltos genera ramas diferentes a la ROM.

### gfx_pools: GfxPools_Init (416 B, marco=0x0)
- **Diff:** 58/104 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Inicialización secuencial plana de 3 parejas de estructuras de pool (`gGfxPools`, `gVtxPools`, `gMtxPools`). GCC 2.7.2 entrelaza las cargas y guardados de constantes en una sola pasada de asignación de registros globales, reordenando los punteros base `D_80134FD0`, `D_801347C0`, `D_801480A0`.

### unit_01050: SndSrc_Play (420 B, marco=0x48)
- **Diff:** Función con tabla de saltos (`jtbl_800FE870`) de audio 3D para fuentes de sonido y control de canales.
- **Causa raiz:** Tabla de saltos en rodata con llamadas dinámicas de audio y cálculo de volumen y paneo.

### unit_49CF0: Hud_DrawSlideFromRight (440 B, marco=0x40)
- **Diff:** 76/110 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Bucle de medición y renderizado de texto deslizante con kerning (`Font_GetKerning`) y sub-píxeles; simétrica a `Hud_DrawSlideFromLeft`. El bucle `do-while` con división modular de tiempo (`(u8)cnt % (u8)step`) genera ramas condicionales que GCC 2.7.2 ordena de forma invertida.

### unit_19D40: Mesh_FillVtx (488 B, marco=0x18)
- **Diff:** Bucle de empaquetado y descompresión de vértices 3D con colores de iluminación y coordenadas de textura.
- **Causa raiz:** Aritmética de índices y empaquetado de campos de 8 bits con operaciones de desplazamiento (`srav`, `sll`, `sra`), donde el asignador de registros de GCC 2.7.2 desborda la estructura de vértices de destino en pares de punteros.

### unit_27A40: Scroller_DrawLine (492 B, marco=0x70)
- **Diff:** 95/125 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Marco de pila de 0x70 bytes pasando 8 argumentos a `Menu_DrawChar`, con doble pasada sobre la cadena para medir ancho centrado y dibujar cada glifo con espaciado proporcional y kerning.

### unit_53B30: Cup_SetupRace (508 B, marco=0x38)
- **Diff:** 96/127 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Copia de bloques de 5 bytes no alineados con `lwl`/`lwr`/`lb` y `swl`/`swr`/`sb` en la pila (`D_80100E98`, `D_80100EA0`), seguido de aleatorización de parrilla y puesta a cero de vueltas con direccionamiento indexado en `D_80123F60`.

### unit_1CC80: ParticlePool_Update (536 B, marco=0x8)
- **Diff:** Actualización del pool circular de partículas con constante doble `D_800FF9F0`, integración de aceleración/velocidad (`unk40`-`unk58`) y eliminación por tiempo de vida o compresión de nodos.
- **Causa raiz:** Constante de doble precisión en .rodata y bucle de copia en bloque de 0x70 bytes (`do { *dst++ = *src++; } while (...)`) que GCC 2.7.2 desdobla en 4 palabras por iteración.

### unit_482C0: Lap_GetTimes (624 B, marco=0x30)
- **Diff:** 104/161 instrucciones iguales tras 3 variantes.
- **Causa raiz:** Estimación de tiempos de vuelta con división entera mediante el multiplicador de punto fijo recíproco `0xCCCCCCCD`, conversiones `cvt.d.w` con dobles ocultos de .rodata (`D_80100A50`, `D_80100A58`), y distribución de registros callee-saved ($s0-$s4, $fs0).

### lz: LZ_AdvanceBits (168 B, marco=0x0)
- **Diff:** Asignación de registros temporales ($a1 frente a $a2) en lectura de bits empaquetados LZSS bajo `-O0`.
- **Causa raiz:** Función compilada sin optimización o con orden de evaluación estricto donde GCC 2.7.2 preserva argumentos en el orden inverso al de la ROM original.

### unit_6E610: SndCmd_SetEnvelopePreset (332 B, marco=0x0)
- **Diff:** Comando MIDI de envolvente ADSR con referencia a doble oculto en .rodata (`D_801015D0`).
- **Causa raiz:** Función listada en `targets.py` bajo `.rodata doubles: 13 funciones`. El generador de código emite la constante en la sección de datos de sólo lectura con alineación de 8 bytes no igualable sin modificar rodata global.

### unit_0B140: Input_Update (736 B, marco=0x38)
- **Diff:** Lectura y filtrado de mandos 1-4 (`nuContDataGetAll`), cálculo de zonas muertas del stick analógico y detección de flancos de subida.
- **Causa raiz:** Desenrollado de bucle sobre los 4 puertos de mando donde GCC 2.7.2 combina las cargas de punteros y los registros callee-saved ($s0-$s4) de forma diferente a los bucles con branches probables de la ROM.

### unit_20830: SortQueue_Sort (816 B, marco=0x40)
- **Diff:** Algoritmo Quicksort no recursivo sobre cola de primitivas translúcidas para ordenamiento de profundidad (Z-sort).
- **Causa raiz:** Implementación de Quicksort con pila explícita en registros/memoria; el asignador de registros de GCC 2.7.2 rota los punteros de partición `$a0`, `$a3`, `$v1` en el bucle interno provocando diferencias de scheduling respecto al código original.

### unit_103B0: Gfx_SetViewport (820 B, marco=0x68)
- **Diff:** Configuración y escalado de viewport 3D (`Vp`) y matriz de perspectiva (`guPerspectiveF`) con cálculos de aspect ratio y frustum culling.
- **Causa raiz:** Múltiples divisiones y multiplicaciones flotantes con constantes inmediatas (`4.0f`, `0.5f`, `100.0f`) que el compilador reordena entre registros flotantes temporales `$ft0` a `$ft5`.

### libultra & rutinas ASM de bajo nivel (SDK N64)
- **Funciones:** `bcmp`, `osInvalICache`, `bzero`, `__osSetCompare`, `osMapTLBRdb`, `__osSetFpcCsr`, `__osSetSR`, `__osSetWatchLo`, `osInvalDCache`, `osWritebackDCacheAll`, `func_800E2D40`, `func_800E2D50`, `func_800E2D60`, `func_800E2D70`, `func_800E2D80`, `func_800E2D90`, `n_alFxPull`, `_n_loadOutputBuffer`, `__osException`, `send_mesg`, `__osEnqueueAndYield`, `__osEnqueueThread`, `__osDispatchThread`, `__osProbeTLB`, `osSetIntMask`, `__osGetCause`, `osWritebackDCache`, `bcopy`, `__osDisableInt`, `__osRestoreInt`, `osGetCount`, `__osGetSR`, `osUnmapTLBAll`.
- **Causa raiz:** Rutinas estándar del SDK de Nintendo 64 (libultra / n_audio) escritas originalmente en ensamblador manual MIPS con instrucciones de coprocesador 0 (`mfc0`, `mtc0`), manipulación directa de registros de estado CP0 (SR, Cause, Compare, WatchLo, TLB), manejo de interrupciones, excepciones y rutinas de memoria optimizadas con desenrollado manual (`bcopy`, `bzero`, `bcmp`). No forman parte de la lógica del juego y están implementadas como rutinas ensamblador ensambladas directamente.

### libc & runtime CRT (`asm/1000.s`, `asm/libc.s`)
- **Funciones:** `entrypoint` (60 B), `ldiv` (132 B), `lldiv` (308 B), `memset` (160 B), `func_800E6BF0` (36 B), `rand` (52 B), `srand` (12 B), `__sincos` (544 B), `sin` (224 B), `cos` (152 B), `tan` (212 B), `strcat` (176 B), `strcmp` (260 B), `strcasecmp` (132 B), `strlwr` (96 B), `tolower` (36 B), `toupper` (36 B), `__muldi3` (52 B), `func_800E74E8` (252 B), `func_800E75E4` (200 B), `func_800E76B0` (356 B), `__udivdi3` (24 B), `__umoddi3` (64 B), `func_800E7880` (284 B), `func_800E79A0` (248 B).
- **Causa raiz:** Biblioteca estándar de C de terceros / Runtime GCC (`libgcc` / `libc` embebida) y punto de entrada IPL3 (`entrypoint`). Estas funciones contienen rutinas aritméticas de 64 bits en software (`__udivdi3`, `__umoddi3`, `__muldi3`), funciones trigonométricas y de cadena precompiladas que no pertenecen a las unidades C del juego (`src/*.c`) y permanecen en sus archivos de ensamblador de enlace.
