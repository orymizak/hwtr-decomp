# Herramientas de matching

Se ejecutan desde la raíz del repo, con `TMP_DIR` apuntando a un directorio temporal.

- `driver.py`: recorre los archivos probables que siguen en `asm/*.s` y llama a `apply.py`.
- `apply.py <nombre> <rom_ini> <rom_fin> [-O2]`: convierte un archivo con `auto.py`, compila y hace commit solo si la ROM es idéntica.
- `auto.py`: m2c, compilar y comparar por función; lo que no coincide queda en `INCLUDE_ASM`.
- `bound.py <asm.s>`: lista los archivos probables (función alineada a 16 con relleno antes).
- `pass2.py [unidades...]`: segunda pasada sobre lo que sigue en `INCLUDE_ASM`; prueba variantes de m2c y de anchos, y hace commit por unidad si la ROM es idéntica.
- `perm.py [--iters N] [unidades...]`: permutador sencillo sobre lo que `pass2.py` no iguala.
- `chk.py <vram_ini> <vram_fin> [--show nombre]`: compara funciones con la ROM. Necesita `funcs.json`, el mapa de funciones generado desde un árbol con todo en asm.
- `ExportDecomp.java` y `gen_context.py`: exportan firmas de Ghidra a `ghidra_work/` y generan el contexto de m2c que usa `pass2.py`.
- `DecompileNamed.java`: script de Ghidra para imprimir el pseudocódigo de funciones por nombre.
