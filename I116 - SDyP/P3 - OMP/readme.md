# OpenMP

## Respecto a SP
La práctica es exactamente la misma! No hagamos nada

## Ejercicio 3
Resulta mejor paralelizar el bucle de i que el de j. En caso de error, usar ```gcc -O1 -fopenmp ...```
* N = 2048, T = 4 arroja 5.10s para la primera versión y 5.14s para la segunda.

## Ejercicio 4
Así como está hay desbalance de carga:
```
user09@hpc:~/sergio-calderon/SDyP/P3$ ./e4a 8192 4
Tiempo en segundos para el thread 3: 0.238113
Tiempo en segundos para el thread 2: 1.192731
Tiempo en segundos para el thread 1: 1.926701
Tiempo en segundos para el thread 0: 2.165309
Resultado correcto
```
Usando schedule(dynamic):
```
user09@hpc:~/sergio-calderon/SDyP/P3$ ./e4a 8192 4
Tiempo en segundos para el thread 1: 2.833984
Tiempo en segundos para el thread 3: 2.833984
Tiempo en segundos para el thread 0: 2.833985
Tiempo en segundos para el thread 2: 2.833985
Resultado correcto
```
