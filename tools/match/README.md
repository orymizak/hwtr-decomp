# Herramientas de matching

Se ejecutan desde la raíz del repo, con `TMP_DIR` apuntando a un directorio temporal.

- `driver.py`: recorre los archivos probables que siguen en `asm/*.s` y llama a `apply.py`.
- `apply.py <nombre> <rom_ini> <rom_fin> [-O2]`: convierte un archivo con `auto.py`, compila y hace commit solo si la ROM es idéntica.
- `auto.py`: m2c, compilar y comparar por función; lo que no coincide queda en `INCLUDE_ASM`.
- `bound.py <asm.s>`: lista los archivos probables (función alineada a 16 con relleno antes).
- `pass2.py [unidades...]`: segunda pasada sobre lo que sigue en `INCLUDE_ASM`; prueba variantes de m2c y de anchos, y hace commit por unidad si la ROM es idéntica.
- `perm.py [--iters N] [unidades...]`: permutador sencillo sobre lo que `pass2.py` no iguala.
- `dperm.py [--secs N] [--max-ratio R] [unidades...]`: pasa lo casi igual por `tools/decomp-permuter` (se clona aparte, no se versiona).
- `chk.py <vram_ini> <vram_fin> [--show nombre]`: compara funciones con la ROM. Necesita `funcs.json`, el mapa de funciones generado desde un árbol con todo en asm.
- `ExportDecomp.java` y `gen_context.py`: exportan firmas de Ghidra a `ghidra_work/` y generan el contexto de m2c que usa `pass2.py`.
- `DecompileNamed.java`: script de Ghidra para imprimir el pseudocódigo de funciones por nombre.

Lo que sigue trabaja sobre la compilación con `tools/cc.py`, que resuelve dobles y tablas de saltos contra `rodata_refs.txt`:

- `targets.py`: lista lo pendiente con tablas de saltos, argumentos en pila o dobles de .rodata.
- `show.py <función>...`: prepara una función para escribirla a mano (tamaño y salida de m2c).
- `hand.py <lote.c> [--apply]`: comprueba un lote escrito a mano y pasa a `src/` lo que coincide.
- `insert.py <lote.c> <unidad>`: pasa un lote de `hand.py` a la unidad poniendo antes los typedefs; para cuando `hand.py --apply` falla por el orden de los tipos. Hay que compilar y comprobar la ROM después.
- `declperm.py <lote.c> [func]`: tries every order of a function's local declarations (GCC 2.7.2 allocates registers partly by declaration order) and saves the first match to `<lote>.ok.c`.
- `ultra_id.py [--version K] [--all]`: identifica funciones de libultra/libaudio compilando ultralib (clon local en `tools/ultralib`, solo como referencia, sin versionar); `ultra_names.py` les pone nombre.
- `try.py <unidad> <función> [archivo.c]`: compara un candidato instrucción a instrucción.
- `gen_rodata_refs.py`: genera `rodata_refs.txt` mientras el código sigue en ensamblador.
- `rodata_owners.py`, `split_rodata.py`, `rodata_cuts.txt`: dividen .rodata por unidad.
- `verify_funcs.py`: comprueba cada función en C ya enlazada, en su dirección final.
- `commit_unit.sh "<mensaje>" <archivos>`: confirma como mucho cada 5 minutos, con ROM idéntica tras `make clean && make`.
- `PATTERNS.md`: patrones de GCC 2.7.2 encontrados.
