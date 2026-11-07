#!/bin/sh
# Deja mupen64plus dentro de tools/emu sin instalarlo en el sistema: baja el paquete de Homebrew, corrige
# sus rutas, lo firma para este Mac y compila el mando virtual. El emulador es GPL y no se versiona.
set -e
EMU=tools/emu/mupen64plus/2.6.0
if [ ! -d "$EMU" ]; then
    HOMEBREW_NO_AUTO_UPDATE=1 brew fetch --force-bottle mupen64plus >/dev/null
    mkdir -p tools/emu
    tar -xzf "$(HOMEBREW_NO_AUTO_UPDATE=1 brew --cache --force-bottle mupen64plus)" -C tools/emu
fi
PREFIX=$(brew --prefix)
for lib in "$EMU"/lib/libmupen64plus.dylib "$EMU"/lib/mupen64plus/*.dylib; do
    otool -L "$lib" | awk '/@@HOMEBREW_PREFIX@@/ {print $1}' | while read -r old; do
        new=$(printf '%s' "$old" | sed "s|@@HOMEBREW_PREFIX@@|$PREFIX|")
        install_name_tool -change "$old" "$new" "$lib" 2>/dev/null
    done
    install_name_tool -id "$(cd "$(dirname "$lib")" && pwd)/$(basename "$lib")" "$lib" 2>/dev/null
    codesign -f -s - "$lib" 2>/dev/null
done
clang -shared -O1 -o tools/emu/input_script.dylib tools/emu_harness/input_script.c
codesign -f -s - tools/emu/input_script.dylib 2>/dev/null
echo listo
