# OpenMP

## Respecto a SP
La práctica es exactamente la misma! No hagamos nada

## Ejercicio 3
Resulta mejor paralelizar el bucle de i que el de j. En caso de error, usar ```gcc -O1 -fopenmp ...```
* N = 2048, T = 4 arroja 5.10s para la primera versión y 5.14s para la segunda.
