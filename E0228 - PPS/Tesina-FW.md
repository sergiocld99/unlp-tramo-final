# Floyd-Warshall algorithm optimized for Intel Xeon Phi Knights Landing

Pasos completados para la compilación y ejecución.

## Parte A
En base al Makefile:

    1.1) Listo, compilador ICX
    1.2) Listo, flags de GCC comentados
    1.3) Listo, SIMD_WIDTH = 512, es decir, el ancho de registros del SIMD es 512
    1.4) Listo, TYPE_SIZE = 32, es decir, el tamaño del tipo de dato es 32 (int y float)
    1.5) Listo, se decide omitir el cómputo de la matriz P

