// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <time.h>       // random
#include "tiempo.h"

// prototipos
void extraerParams(int argc, char* argv[]);

// shared
int N;
int* V;

// programa principal
int main(int argc, char* argv[]){
    // variables locales
    int i;
    double suma = 0.0, prom;

    extraerParams(argc, argv);
    srand(time(NULL));

    // N...
    V = (int*) malloc(N * sizeof(int));
    for (i=0; i<N; i++) V[i] = rand() % 1000;

    // Medir
    double t0 = dwalltime();

    for (i=0; i<N; i++) suma += V[i];
    prom = suma / N;

    double t1 = dwalltime();

    printf("N = %d, promedio = %f, tiempo = %f segundos\n", N, prom, t1 - t0);

    // liberar recursos
    free(V);

    return 0;
}

void extraerParams(int argc, char* argv[]){
    if (argc < 2) {
        printf("Especificar N\n");
        exit(1);
    }

    N = atoi(argv[1]);

    if (N < 1) {
        printf("N debe ser positivo\n");
        exit(2);
    }
}