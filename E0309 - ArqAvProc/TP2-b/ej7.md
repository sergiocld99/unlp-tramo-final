# Loop unrolling
Dé brevemente dos formas en las que el desenrollado de bucles puede aumentar el rendimiento y una en la que puede disminuir el rendimiento.

## Ventajas
Al desenrollar más de 1 iteración por bucle...

1) Se realizan menos evaluaciones de condición.
2) Se ejecutan menos instrucciones de salto.
3) Se pueden reordenar las instrucciones, de modo que las cargas independientes se realicen primero, luego las operaciones con dichos datos y por último el almacenamiento en memoria. A esto se le llama <b>fusión de cargas y almacenamientos</b>.

## Desventajas
No todo es color de rosa...

1) Se puede producir un aumento de los reemplazos en la caché de instrucciones, al tratarse de iteraciones con mayor cantidad de instrucciones. Si una instrucción necesitada es reemplazada, se debe volver a buscarla en memoria.