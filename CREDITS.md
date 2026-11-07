# Archivos originales

Esta copia de trabajo usa archivos del juego original, sacados de la copia de la ROM de su dueño. Ninguno está en git: los excluye `.gitignore`. Las rutas del juego son relativas a la raíz del repositorio.

| Archivo en esta copia | Procede de | Uso |
|---|---|---|
| `baserom.z64` | `baserom.z64` (SHA-1 `94913a07e49005ffd43dfdfa16ff5862bdb93748`) | ROM de referencia: la compilación debe darla idéntica |
| `asm/` | Extraído de `baserom.z64` con `make split` (splat) | Código y datos aún sin convertir a C |
| `assets/ipl3.bin`, `assets/padding.bin`, `assets/rsp_text.bin` | Extraídos de `baserom.z64` | Arranque, relleno y microcódigos RSP, enlazados tal cual |
| `assets/rom_files/` | Extraídos de `baserom.z64` | Los 201 archivos del juego, enlazados tal cual |
| `assets/files/` | Descomprimidos de esos 201 archivos con `make extract` | Lectura de `Tuning.prm`, `.CWH` y `.DUP` para `MECHANICS.md` |
| `build/hotwheelsturbo.z64` | Compilado desde esta copia | ROM recompilada, idéntica a la original |
| `ghidra_work/` | Exportado con Ghidra del proyecto original | Contexto de tipos para m2c y pseudocódigo de referencia |

Herramientas de terceros usadas sin modificar y sin versionar: GCC 2.7.2 y binutils 2.6 de decompals, en `tools/gcc_2.7.2/`.
