# Floyd-Warshall algorithm optimized for Intel Xeon Phi Knights Landing

Pasos completados para la compilación y ejecución.

## Parte A
En base al Makefile:

    1.1) Listo, compilador ICX
    1.2) Listo, flags de GCC comentados
    1.3) Listo, SIMD_WIDTH = 512, es decir, el ancho de registros del SIMD es 512
    1.4) Listo, TYPE_SIZE = 32, es decir, el tamaño del tipo de dato es 32 (int y float)
    1.5) Listo, se decide omitir el cómputo de la matriz P
    
Luego en include/defs.h:

    2.1) Tipo de dato usado en el grafo: #define TYPE float
    2.2) Densidad del grafo (en porcentaje): #define GRAPH_DENSITY 70
    
Luego en /input/input_files_generator.c:
    
    3.1) Creamos el vector de tamaños a testear: const int n_array[N_COUNT] = {4096, 8192, 16384, 32768};
    3.2) Definimos el tamaño del vector anterior: #define N_COUNT 4
    3.3) Tamaños de bloque que usaremos para testear: const int bs_array[BS_COUNT] = {32, 64, 128};
    3.4) #define BS_COUNT 3
    
 Por último en el script compile_with_bs:
 
    4.1) Nuevamente los tamaños de bloque: bs_array=(32 64 128)
 
