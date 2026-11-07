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
| total | 585/1917 | 38040/554576 | 6.86 % |
| juego | 444/1373 | 30956/446896 | 6.93 % |
| audio | 61/188 | 2684/32608 | 8.23 % |
| sistema | 39/87 | 1864/10064 | 18.52 % |
| libultra | 41/244 | 2536/60640 | 4.18 % |
| libc | 0/25 | 0/4368 | 0.00 % |
<!-- /progress -->

`make progress` actualiza esta tabla.

- **Compilador:** GCC 2.7.2 compatible con KMC reproduce exactamente las funciones probadas. Las opciones
  de compilación aún son provisionales, y varían por archivo (`-O2` y `-O0`).
- **Librerías:** libultra (probablemente SDK 2.0J), libc y un driver de audio están separados del juego en
  el mapa de la ROM. Los nombres de libultra se asignaron por análisis del código.
- **Por hacer:** el resto del código, y la comparación del SHA-1 contra No-Intro.
