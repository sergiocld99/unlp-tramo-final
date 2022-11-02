# Floyd-Warshall algorithm optimized for Intel Xeon Phi Knights Landing

Pasos completados para la compilación y ejecución.

## Parte A
En base al Makefile:

    1.1) Listo, compilador ICX
    1.2) Listo, flags de GCC comentados
    1.3) Listo, SIMD_WIDTH = 512, es decir, el ancho de registros del SIMD es 512
    1.4) Listo, TYPE_SIZE = 32, es decir, el tamaño del tipo de dato es 32 (int y float)
    1.5) Listo, NO SE OMITE el cómputo de la matriz P (caminos mínimos). Si quisiera omitirlo la línea NO tiene que estar comentada.
    
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
 
## Parte B
Se crearon las siguientes carpetas para que se pueda compilar:
- obj/floyd_versions
- bin/BS-32
- bin/BS-64
- bin/BS-128

También ocurría un error de "implicit declaration" para ```_mm_malloc``` y ```_mm_free```. La compilación fue exitosa cambiando la línea 25 del Makefile (la original se dejó comentada en la 26).

## Parte C
Para poder correr el input generator se debio crear la carpeta ```input/files/input```

## Parte D
Pruebo correr un programa cualquiera. Debo especificar N y T de manera obligatoria. Por ejemplo, para ```./bin/BS-32/opt_7_8 4096 56```:

    Excecuting Floyd-Warshall test with the following arguments:
    N = 4096
    BS = 32
    T = 56
    GD = 70%
    P matrix computed: Yes
    =============================================================================
    Matrices of 4096x4096. Each one (2) occupies in memory: 64.00 MB
    Total occupied memory space: >= 128.00 MB
    =============================================================================
    Preparing graphs:
        Loading adjacency matrix from disk . . . Done
        Preparing P matrix in memory . . . Done
    Executing Floyd Warshal Opt-7_8 (with loop unroll): . . .
    Execution completed. Test results: 0.5 secs, 276.880 GFLOPS

## Pasar de float a double
No hace falta repetir todos los pasos anteriores:

    A 1.4) TYPE = 64
    A 2.1) #define TYPE double
    B) Correr "./compile_with_bs.sh"
    C) Correr "./input/input_files_generator"
    D) Correr "run_mixed_volley.sh"
