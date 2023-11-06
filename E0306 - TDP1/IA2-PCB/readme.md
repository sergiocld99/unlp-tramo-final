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

## Desde plantilla ejemplo
Si copiaste directamente el PCB Layout de Ejemplo Proyecto PCB en tu proyecto de Proteus, podemos actualizar los modelos sin perder sus ubicaciones (coordenadas) exactas. Caso conector:

1. Clic secundario sobre los pads > Copy to Clipboard
2. Pegarlo fuera del poncho, para editarlo libremente.
3. Decompose, editarlo y Make Package, bajo un nuevo nombre.
4. Ir al esquemático, clic secundario en componente > Packaging Tool > ... los pasos que ya sabés
5. Volver al PCB Layout > Component Mode > posicionarlo sobre el conector viejo (el que está en el poncho, no afuera)

Listo! De esta forma se reemplazó el modelo anterior con el nuevo que creaste. Podés borrar el que quedó afuera sin miedo

## Troubleshoot
Los íconos de herramientas PCB no aparecen...

![image](https://github.com/sergiocarp10/unlp-tramo-final/assets/66924320/a4074585-33c2-48b3-8003-b7672937ff8f)

* Solución rápida: Configuración de Windows > Display > Escala 100% (en mi caso estaba a 125%)
* Solución completa: https://youtu.be/TiQW7cBWO8c?si=-Qc3tJbYS7ZyUtuh&t=112
