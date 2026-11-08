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

Solo se distinguen por el reparto de registros o por el orden de una carga: `func_800BA3A8`, `func_800658D4`, `func_80067284`, `func_800D0394`, `func_800D9D70`, `func_80095C60`, `func_800DBB10`, `func_800A8BA4`.
