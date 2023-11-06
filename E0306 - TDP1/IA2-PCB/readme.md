# Avance 2 - Corrección Esquemático y PCB

## Relación Esquemático - Huella
Primero en el PCB Layout, se debe estar en Package Mode (icono de MCU) y...

* Opción 1: crear huella desde cero vía las figuras geométricas
* Opción 2: colocar un modelo existente, descomponerlo y editarlo.

Luego, se construye el modelo con un nombre nuevo vía Make Package. Ahora, en la hoja esquemático:

1. Clic secundario en componente a relacionar
2. Packaging Tool
3. Borrar el packaging actual (Delete)
4. Botón Add
5. Buscar nombre del nuevo modelo y elegirlo
6. Controlar que los pines se correspondan
7. Assign Package > Save Package
8. Si pregunta por actualizar o reemplazar, decirle nomás que si

Por último, en el PCB Layout, en Component Mode colocar el componente actualizado. Debería aparecer el modelo nuevo.

## Troubleshoot
Los íconos de herramientas PCB no aparecen...

![image](https://github.com/sergiocarp10/unlp-tramo-final/assets/66924320/a4074585-33c2-48b3-8003-b7672937ff8f)

* Solución rápida: Configuración de Windows > Display > Escala 100% (en mi caso estaba a 125%)
* Solución completa: https://youtu.be/TiQW7cBWO8c?si=-Qc3tJbYS7ZyUtuh&t=112
