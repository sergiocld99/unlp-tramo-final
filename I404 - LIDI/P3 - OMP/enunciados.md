* Para compilar: gcc -fopenmp -O3 ejercicio1.c -o ej1.out

## Ej 1
El programa ejercicio1.c inicializa una matriz de NxN de la siguiente manera: A[i,j]=i*j, para todoi,j=0..N-1. Compile y ejecute. ¿Qué problemas tiene el programa? Corríjalo

* dice resultado erróneo

- A es compartida -> bien
   
1) ¿son las iteraciones independientes entre sí? (...)
2) ¿el numero de iteraciones ya es conocido? (Sí, es N)
3) ¿la variable indice "j" la modifican los hilos? (No la modifican)
4) ¿hay algún break? (No)

- PROBLEMA: private(i) hace que siempre valga 0 dentro del for
- CAUSA: no se le asigna un valor a la copia local de cada hilo (por defecto 0)
- NO CONFUNDIR: el bucle for(i=0..N-1) exterior NO INTERFIERE (la copia no toma su valor)
- SOLUCIÓN: shared(i) o directamete no indicarla (shared por defecto)


## EJ 2
Analice y compile el programa ejercicio2.c. Ejecute varias veces y compare los resultados de salida para diferente número de threads. ¿Cuál es el problema? ¿Se le ocurre una solución? Nota: al compilar, agregue el flag -lm.

- PROBLEMA: x siempre da distinto.
- CAUSA: las iteraciones NO son independientes entre sí
- OBSERVACIÓN: sqrt(i*scale) es independiente
- SOLUCIÓN: DIRECTIVA ORDERED para asignar x