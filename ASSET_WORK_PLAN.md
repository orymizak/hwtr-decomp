# Plan de trabajo de assets

Este documento separa los formatos confirmados de los que siguen en análisis.
Ninguna fase autoriza reemplazar un binario de `assets/files`: las herramientas
escriben únicamente en `assets/workbench`.

## 1. Texturas y UI — completado para los formatos confirmados

- Fondos `.BG`: PNG reversible con paleta original.
- Datos `.CWH`, `.DUP` y tuning: JSON editable y reconstrucción comprobada.
- Fuente `ACTNFNT.OVL`: atlas PNG RGBA5551 de 124 glifos.
- `CLOUD*.TEX`, `ENVMAP1.TEX`: PNG con formato documentado.
- `EFFECTS.TEX`: 30 PNG individuales y atlas; contenedor verificado.

**Verificación:** exportar y revisar los manifiestos de
`assets/workbench/previews_clean`.

## 2. Objetos `.OBJ` — geometría estática completada

- El cargador y el intérprete de comandos del juego fueron usados para
  recuperar cargas de vértices y triángulos.
- Los 22 `.OBJ` se exportan a Wavefront OBJ en
  `assets/workbench/meshes`.
- El exportador se niega a emitir una malla si una variante de comando no es
  compatible, en vez de sustituirla por geometría aproximada.

**Herramientas:** `tools/model_command_inspector.py` y
`tools/obj_mesh_export.py`.

## 3. Vehículos `.CAR` — geometría y texturas completadas (falta GLB)

- `tools/model_mesh_export.py` porta `Mesh_FillVtx` (vértices indexados por
  grupos) y `Mesh_BuildDisplayList` (comandos), y decodifica texturas según
  `func_8007F61C` (RGBA16, I8, RGBA32, CI8). Sirve para CAR y OBJ; reemplaza a
  `obj_mesh_export.py`, que leía mal el opcode 2 y la tabla de posiciones.
- Cada raíz de un CAR es un LOD de carrocería (4) y sus hijas son las ruedas.
  El OBJ exporta el LOD 0; el JSON conserva todos.
- Textura 0 = mapa de entorno (`EnvMap_Apply`: `ENVMAP1.TEX` en alta
  resolución, captura de pantalla en baja), teñido por el color prim: es la
  pintura del coche.
- `tools/car_viewer.py` muestra la malla real con texturas, ruedas que giran y
  selector de LOD (`build/car_viewer.html#NOMBRE`).
- Pendiente: GLB.

**Criterio de aceptación:** cada una de las 41 carrocerías debe abrirse como
una malla con su número de piezas y sin referencias de vértice fuera de rango.

## 4. Edificios `.BLD` — en análisis

- No comparte el contenedor CAR/OBJ ni se exportará como una malla hasta
  identificar su cargador y sus bloques internos.
- Primero: mapa de cabecera, referencias, tamaño de bloques y lista de
  comandos; después: vértices, caras y materiales.

**Criterio de aceptación:** un BLD pequeño se abre como GLB y su conteo de
bloques coincide con el cargador del juego.

## 5. Escenarios `.WLD`, `.SCN`, `.SKY` — pendiente después de BLD

- `.WLD`: geometría/base de pista.
- `.SCN`: instancias y colocación de OBJ/BLD.
- `.SKY`: capas de cielo.
- Entregable: visor que componga los cuatro recursos de una pista.

## 6. Importación y ROM — pendiente después de exportación validada

- Crear importador por formato, con comprobación de tamaño, alineación,
  offsets, paletas y referencias.
- Recomprimir los formatos LZ requeridos y actualizar la tabla de archivos.
- Construir y probar la ROM antes de habilitar modificaciones de vehículos.

## 7. Vehículos custom — último paso

Avance: el garaje de `car_viewer.html` edita pintura (colores prim), texturas,
proporciones y estadísticas; `tools/car_edit_import.py` escribe un `.CAR`
descomprimido y un `.CWH` nuevos (pintura, texturas y estadísticas; la forma
aún no). Falta el compresor LZ y la tabla de archivos para meter el `.CAR` en la
ROM. `car_viewer.py --demo` genera una página sin datos del juego.

- Plantilla CAR + CWH + textura/paleta.
- Usar primero slots disponibles.
- Si se excede el límite, extender tablas y código del juego, seguido de
  pruebas de selección, carrera, daño, físicas y sonido.
