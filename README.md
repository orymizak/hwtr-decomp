# Hot Wheels Turbo Racing (N64, USA): decompilación

Reconstrucción en C del código de *Hot Wheels Turbo Racing* para Nintendo 64. El objetivo es que el
código compile a una ROM idéntica byte a byte a la original.

**Este repositorio no contiene la ROM, assets ni código del juego ya compilado.** Necesitas tu propia
copia legal de la ROM: de ella se extrae todo lo demás en tu equipo. No compartas la ROM, `assets/`,
`asm/` ni `build/`.

## ROM requerida

| | |
|---|---|
| Archivo | `baserom.z64` (big-endian) |
| Tamaño | 12.582.912 bytes |
| SHA-1 | `94913a07e49005ffd43dfdfa16ff5862bdb93748` |
| ID interno | `NHWE` |

## Requisitos

- Python 3 con las dependencias de `requirements.txt`, por ejemplo en un entorno virtual
- GNU Make y `curl`
- Binutils MIPS:
  - macOS: `brew install mips-linux-gnu-binutils`
  - Debian/Ubuntu: `apt install binutils-mips-linux-gnu`
- GCC 2.7.2 para MIPS y binutils 2.6, que instala `make toolchain`

En Mac con Apple Silicon, el compilador es un binario x86_64 y necesita Rosetta:
`softwareupdate --install-rosetta`.

## Compilar

```sh
python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
cp /ruta/a/tu/rom.z64 baserom.z64
make toolchain
make split
make
```

`make` construye `build/hotwheelsturbo.z64` y comprueba su SHA-1.

`python3 main.py` compila, comprueba la ROM y cada función en C, y muestra el avance. No es el juego.

Para extraer y descomprimir los 201 archivos del juego en `assets/files/`:

```sh
make extract
```

## Estructura

| Ruta | Contenido |
|---|---|
| `src/` | código C reconstruido |
| `include/` | cabeceras y macros de ensamblador |
| `hotwheelsturbo.yaml` | mapa de la ROM para [splat](https://github.com/ethteck/splat) |
| `symbol_addrs.txt` | símbolos identificados |
| `tools/extract_files.py` | extractor y descompresor de archivos |

`make split` genera a partir de tu ROM `asm/`, `assets/` y el script de enlazado. Nada de eso se versiona.

## Estado

La ROM se reconstruye idéntica, pero casi todo el código sigue en ensamblador generado.

<!-- progress -->
| Zona | Funciones | Bytes de código | Avance |
|---|---|---|---|
| total | 934/1917 | 109560/554576 | 19.76 % |
| juego | 589/1373 | 46344/446896 | 10.37 % |
| audio | 89/188 | 5724/32608 | 17.55 % |
| sistema | 46/87 | 2436/10064 | 24.21 % |
| libultra | 210/244 | 55056/60640 | 90.79 % |
| libc | 0/25 | 0/4368 | 0.00 % |
<!-- /progress -->

`make progress` actualiza esta tabla.

- **Compilador:** GCC 2.7.2 compatible con KMC reproduce exactamente las funciones probadas. Las opciones
  de compilación aún son provisionales, y varían por archivo (`-O2` y `-O0`).
- **Librerías:** libultra (probablemente SDK 2.0J), libc y un driver de audio están separados del juego en
  el mapa de la ROM. Los nombres de libultra se asignaron por análisis del código.
- **.rodata:** empieza en la ROM en `0x9F3B0` y está dividida por unidad en `asm/data/`. `tools/cc.py` hace que el C use esos símbolos en lugar de duplicarlos.
- **Por hacer:** el resto del código, y la comparación del SHA-1 contra No-Intro.

## License

The original tooling, build scripts and documentation in this repository are released under the [MIT License](LICENSE).
Decompiled code is provided for preservation and research purposes; the game and the Nintendo 64 SDK remain the property of their respective owners.
No ROM or game assets are included: you need your own legally obtained copy of the game to build it.
