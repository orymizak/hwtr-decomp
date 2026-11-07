# Mecánicas de Hot Wheels Turbo Racing (N64)

Valores leídos del código y de los archivos del juego, no estimados. **La compilación ya usa todos estos valores**, porque el código y los archivos reconstruyen la ROM byte a byte. Esta hoja los documenta; cambiar uno rompería el SHA-1.

Fuentes:
- `Tuning.prm` y los `.CWH`/`.DUP`, en `assets/files/`.
- Pseudocódigo de Ghidra en `ghidra_work/decomp.jsonl`, exportado de tu proyecto, porque la herramienta de Craft Anything no decodifica MIPS.
- Ensamblador en `asm/`, donde había dudas.

Lo marcado como *interpretación* es mi lectura del uso del valor; el número y la dirección sí están comprobados.

`mechanics.json` recoge solo constantes escritas en el código: `tools/match/gen_mechanics.py` comprueba que cada una aparece como `f32` inmediato en su función. Los valores de `Tuning.prm`, `.CWH` y `.DUP` son datos que el juego lee de sus archivos, y están en las tablas de abajo.

## Unidades

| Dato | Valor | Fuente |
|---|---|---|
| Longitud | pulgadas (*interpretación*: gravedad en pulgadas/s²) | `func_80095970` |
| Gravedad | 386 | `func_80095970`, `func_800949E4` |
| Pulgadas/s por mph | 17,6 | `func_8008D060`, `func_800946DC` |
| Pulgadas a pies | 0,083333 | `func_800946DC` |
| Densidad del aire | 2,377 / 1000 | `func_800946DC` |

## Tuning.prm

256 bytes en la ROM en `0xBAF620`. Se carga en `D_80138D38` (`func_800C9E70`), y solo acepta el archivo si mide 0x100. `D` es la dificultad, de 0 a 255, leída en `+0x9C` de la configuración de carrera. Una pareja se interpola así: `(D/255) * (fin - inicio) + inicio`.

| Bytes | Valores | Uso | Función |
|---|---|---|---|
| 0x00–0x01 | 85 → 121 | Multiplicador de velocidad de la IA: valor / 100 | `func_800853F4`, `func_800858F0` |
| 0x02–0x03 | 82 → 97 | Mínimo de la corrección de la IA: valor / 100 | `func_800853F4` |
| 0x0C–0x0D | 122 → 112 | Máximo de la corrección de la IA: valor / 100 | `func_800853F4` |
| 0x04–0x05 | 15 → 5 | Separación objetivo de la IA: valor / 10 | `func_800853F4` |
| 0x2E–0x2F | 15 → 9 | Margen alrededor de esa separación: valor / 10 | `func_800853F4` |
| 0x34 | 10 → 0 | Ajuste de separación en la última vuelta: valor / 10 | `func_800853F4` |
| 0x06–0x07 | 18 → 1 | Valor por coche de la IA (`D_80114AD0`); uso sin leer | `func_800858F0` |
| 0x08 | 30 mph | Velocidad desde la que se reduce la dirección | `func_8008D060` |
| 0x09, 0x3A | 30, 50 | Porcentaje de dirección a velocidad máxima y al empezar la reducción | `func_8008D060` |
| 0x0A | 40 mph | Viento añadido al cálculo aerodinámico con la bandera `+0x3E7` | `func_800946DC` |
| 0x0B | 40 mph | Límite de velocidad en una comprobación; contexto sin leer | `func_8008EA6C` |
| 0x0E–0x0F | 90 → 100 | Resistencia de la IA (`+0x53C`), en % | `func_80091210` |
| 0x10–0x11 | 190 → 110 | Resistencia del jugador (`+0x53C`), en % | `func_80091210` |
| 0x12 | 100 | Paso de la dirección de la IA al reaccionar, limitada a ±80 | `func_80084B9C` |
| 0x13, 0x14 | 105, 60 | Acelerador de la IA: máximo, y mínimo = valor − 100 (−40) | `func_80084B9C` |
| 0x15–0x18 | 1, 7, 1, 4 | Pasos de ese acelerador según el lado del contacto | `func_80084B9C` |
| 0x1A | 36 | Corrección mínima de la dirección de la IA | `func_80081840` |
| 0x1B | 20 | Periodo de la corrección de la IA: valor × 10 ms de `func_800A76D8` (*interpretación*: 200 ms) | `func_80081840` |
| 0x1C–0x1F | 2, 4, 2, 3 | Pasos de la corrección de la IA, / 100 | `func_80081840` |
| 0x33 | 18 | % de `masa × normal` aplicado sobre superficie de tipo 6 | `func_800949E4` |
| 0x35–0x37 | 35, 35, 10 | Tiempos antes de corregir un vuelco, por eje | `func_80096954` |
| 0x38, 0x39 | 25, 100 | Probabilidad y espera de recuperación | `func_80096954` |

### Valores fijos en el circuito `tcup`

Si la bandera `0x100` está activa y el nombre es `tcup`, se ignoran las curvas anteriores (`func_800853F4`): velocidad 1,0; mínimo 1,0; máximo 1,04; separación 0,4; margen 2,0; ajuste 0; `D_80115838` = 1,0.

## Conducción

**Entradas** (`func_8008D060`, cada fotograma; código de entrada a la izquierda):

| Código | Campo | Efecto |
|---|---|---|
| 1 − 0 | `+0x10` | Dirección, de −1 a 1, por el ángulo máximo `+0x4CC` |
| 2 | `+0x14` | Acelerador, de 0 a 1 |
| 3 | `+0x18` | Freno, de 0 a 1 |
| 4 − 5 | `+0x1C` | Eje en el aire (*interpretación*) |
| 6 − 7 | `+0x20` | Eje en el aire (*interpretación*) |
| 8, 9, 10 | `+0x24`, `+0x25`, `+0x26`/`+0x27` | Botones |
| 11, 12 | `func_80090550`, `func_800AC7C8` | Acciones sin identificar |

**Dirección según la velocidad** (`func_8008D060`). `v` es `+0xBC` y `U = byte0x08 * 17,6`, es decir 528. Por encima de `U`:

```
f = limitar(1 - (v - U) / (2304 - U), 0, 1)
dirección *= (f * (50 - 30) + 30) / 100
```

2304 equivale a 130,9 mph.

**Aerodinámica** (`func_800946DC`), con `v` en pies/s:

```
q = v² * 2,377 * 0,5 / 1000
arrastre = q * ancho_pies * alto_pies * Cd(+0x4E4)
carga    = q * área(+0x4F0) * Cl(+0x4E8)     // eje 1
carga    = q * área(+0x4F4) * Cl(+0x4EC)     // eje 2
```

Solo se calcula con `v > 1` pulgada/s antes de convertir. `ancho`/`alto` salen de `+0x5E8` y `+0x5F0`. Con las banderas `0x380` del coche, las cargas se multiplican por los bytes 0x21 y siguientes de Tuning.prm, en %.

**Suspensión** (`func_80095970`):

```
carga_rueda = masa(+0x4D0) * 386 / número_de_ruedas
rigidez_1 = carga_rueda / +0x4F8
rigidez_2 = carga_rueda / +0x510
```

**Empuje** (`func_800949E4`): `masa * 386 * +0x4DC * entrada`, repartido así: `+0x4E0` a las ruedas sin la bandera `0x1000000` y `1 - +0x4E0` a las que la tienen. En coches de 6 ruedas, las segundas lo reducen a la mitad.

**Choques que destrozan el coche** (`func_800A66C0`). `r` es la resistencia `+0x53C` limitada a [0,25, 4] más la componente vertical del contacto:

```
r < 1:  límite = (r - 0,25) * 938,6667 - 704 + 2112
r >= 1: límite = (r - 1) * 821,3333 + 2112
```

Si la velocidad hacia la superficie supera `límite`, se llama a `func_800961B0`. 2112 equivale a 120 mph. La resistencia final se multiplica por el % de Tuning.prm y se limita a 4,0 (`func_80091210`).

## Datos de cada coche (.CWH)

Hay 41 coches. Cada campo es un `f32` en big-endian, salvo donde se indica. El campo `+0x4CC` del coche empieza en el byte 8 del archivo. Los bytes 4–7 son enteros (`+0x4C8`–`+0x4CB`). Los números están leídos del archivo, pero los nombres de la columna "Uso" son *interpretación* a partir de las fórmulas anteriores, igual que `ancho` y `alto` en la aerodinámica.

| Campo | Deora | Twin Mill | Uso |
|---|---|---|---|
| `+0x4CC` | 0,663 | 0,698 | Dirección máxima (*interpretación*: radianes) |
| `+0x4D0` | 4,21 | 4,21 | Masa |
| `+0x4D4`, `+0x4D8` | 0,18, −1,4 | 0,07, −1,35 | Posición del centro de masa |
| `+0x4DC` | 3,8 | 5,0 | Empuje, en múltiplos de la gravedad |
| `+0x4E0` | 0,45 | 0,13 | Reparto del empuje entre ejes |
| `+0x4E4` | 0,7 | 0,2 | Coeficiente de arrastre |
| `+0x4E8`, `+0x4EC` | −1,1, −1,8 | −1,9, −1,7 | Coeficientes de carga |
| `+0x4F0`, `+0x4F4` | 24, 24 | 21, 15 | Áreas de carga |
| `+0x4F8`, `+0x510` | 204, 114 | 408, 108 | Recorrido estático de suspensión, por eje |
| `+0x4FC`, `+0x514` | 6,3, 10,3 | 8,8, 11,7 | Agarre, por eje (*interpretación*: los multiplica Sticky Tires) |
| `+0x534` | 0,95 | 1,015 | Ritmo de la IA |
| `+0x53C` | 3,0 | 3,825 | Resistencia a choques |
| `+0x540` | `0x0300014F` | `0x020001FF` | Banderas |

## Ritmo de la IA

- **Velocidad base** (`func_800858F0`): `((+0x534 * 100 - 100) + 100) * multiplicador - 100`.
- **Tiempos estimados al terminar** (`func_800A7C8C`): si un coche de la IA no ha acabado, cada vuelta restante vale `D_800F3914 / +0x534 + azar(D_800F3914 / 20)`.
- **Corrección de la IA** (`func_80081840`): solo con `D_80115845 == 1`, y cada periodo:

```
gap = progreso_jugador - progreso_coche
si es la última vuelta: gap += ajuste
si gap > objetivo + margen:        corrección -= 0,02
si gap < objetivo - margen:        corrección += 0,03
si no, y corrección > 0:           corrección -= 0,02
si no:                             corrección += 0,04
limitar la corrección a [mínimo, máximo]
```

Sin esa condición, la corrección vale 1,0. El progreso parte de `vueltas * D_8011480C / 1000` (`func_800853F4`, `func_800858F0`).

## Potenciadores (.DUP)

Los lee `func_80088010`/`func_80088558`. El byte `+0x2C` es un entero (10) cuyo uso no he leído, y `+0x34` es la máscara de bandera.

| Archivo | Máscara | Multiplicadores |
|---|---|---|
| BRAKES | 0x01 | Ninguno, solo bandera |
| TURBO | 0x04 | Ninguno, solo bandera |
| 4X4 | 0x08 | Ninguno, solo bandera |
| STEEL | 0x10 | Ninguno, solo bandera |
| RUBBER | 0x20 | Ninguno, solo bandera |
| GYRO | 0x40 | `+0x60`–`+0x68` × 2,0 |
| STICKY | 0x00 | Agarre `+0x4FC` y `+0x514` × 1,8 |
| HANDLING | 0x00 | Masa × 0,9; empuje × 1,3; agarre × 1,3; `+0x508` y `+0x520` × 1,2; `+0x60`–`+0x68` × 1,1; `+0x8C` × 1,3; `+0x94` × 1,4 |
| UNCAR1, UNCAR2 | `0x01000000`, `0x02000000` | Ninguno; por su texto, desbloquean un coche |

## Configuración de carrera

Campos del bloque que recibe `func_800853F4`/`func_80091210`:

| Offset | Uso |
|---|---|
| `+0x00` | Banderas; `0x100` activa la comparación con el nombre del circuito |
| `+0x0D` | Nombre del circuito |
| `+0x18` | Número de coches |
| `+0x19` | Vueltas |
| `+0x1C` | Banderas |
| `+0x24 + 0x14 * i` | Nombre del coche `i` |
| `+0x30 + 0x14 * i` | Tipo: 0 IA, 1 y 2 jugador (*interpretación*) |
| `+0x9C` | Dificultad |

## Sin leer todavía

Busqué estos datos solo en las funciones anteriores y no los tengo. No son estimaciones, simplemente faltan:

- Curva del motor y caja de cambios.
- Efecto en juego de TURBO, 4X4, STEEL y RUBBER, que solo activan una bandera.
- Cámara y campo de visión.
- Vueltas de cada circuito, puntuación y reglas de campeonato.
- La interfaz en pantalla.
