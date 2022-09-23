# Apuntes de OMP

- Basado en directivas
- Tiene variables de entorno
- Mayor abstracción que Pthreads: OpenMP es traducido a Pthreads por el compilador.

## Modelo Fork-Join
Se tiene un solo hilo master, que al encontrar un constructor paralelo crea un grupo de hilos (acción llamada Fork), que ejecutan el bloque encerrado por el constructor en paralelo. Luego, cuando terminan de ejecutar el bloque, se sincronizan y terminan, continuando solo el hilo master (acción llamada Join)

## Constructor parallel
- Master: id 0
- cada hilo tiene un ID único

* Sintaxis: #pragma omp parallel [clausulas] {...}

### Clausulas
- private(lista de variables): crea copias locales a cada hilo de cada variable indicada
- shared(lista de variables): todos los hilos las pueden leer y escribir (variable global)
- default(shared|private|none)

- num_threads(T): especifica la cant de hilos a crear al hacer Fork, sino es OMP_NUM_THREADS

- if: paralelizar si se supera cierto umbral, ya que el secuencial es mejor para N chico

## Directiva For
- requiere iteraciones independientes
- numero de iteraciones ya conocido
- variable indice no modificable por hilos
- prohibido usar break

* Sintaxis: #pragma omp for [clausulas] {...}

### Clausulas
- private, firstprivate y shared...
- lastprivate(lista var): similar a private, pero la variable original queda asignada con el valor de la última iteración del bucle.
- reduction(operador:variable): para combinar, por ejemplo reduction(+:variableSuma)
- nowait: permite que los hilos continuen aunque queden iteraciones pendientes de otros
- ordered: permite directiva ordered.
- schedule(politica, chunk)...

#### Schedule
El tamaño de bloque quedará definido por...

- static: tamaño fijo, N/T por defecto
- dynamic: bajo demanda, 1 por defecto
- guided: bajo demanda, grande a chico
- runtime: var entorno OMP_SCHEDULE

## Directiva Sections
* Sintaxis: #pragma omp sections [clausulas]
* Sintaxis sección: #pragma omp section {...}

## Directiva Parallel Sections
Combinación de las anteriores.

## Directiva single
Se utiliza para ejecutar un codigo pero solo por parte de un hilo dentro de un parallel.

* Sintaxis: #pragma omp single {...}

## Directiva barrier
Para sincronización, evitar usar muchos.

* Sintaxis: #pragma omp barrier {...}

## Directiva critical
* Sintaxis: #pragma omp critical [name] {...}

## Directiva atomic
Mejora de rendimiento frente a critical.

* Sintaxis: #pragma omp atomic {...}

## Directiva ordered
Para ejecutar como si fuera secuencial dentro de un parallel for.

* Sintaxis: #pragma omp ordered {...}

## Variables de entorno
- OMP_NUM_THREADS: numero de hilos por defecto
- OMP_SCHEDULE: politica iteraciones por hilo
- OMP_NESTED: permite anidacion de directivas

## Funciones
- omp_get_thread_num(): devuelve mi id de hilo
- omp_get_num_threads(): cant hilos creados
- omp_set_num_threads(): cant hilos default