// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <time.h>       // random
#include "tiempo.h"

// prototipos
void extraerParams(int argc, char* argv[]);

// shared
int N, X;
int* V;

// programa principal
int main(int argc, char* argv[]){
    // variables locales
    int i, ocurrencias = 0;

    extraerParams(argc, argv);
    srand(time(NULL));

    V = (int*) malloc(N * sizeof(int));

    for (i=0; i<N; i++){
        V[i] = rand() % 1000;
    }

    // Medir
    double t0 = dwalltime();

    for (i=0; i<N; i++){
        if (V[i] == X) ocurrencias++;
    }

    double t1 = dwalltime();

    printf("N = %d, mide %f segundos\n", N, t1 - t0);
    printf("Ocurrencias = %d \n", ocurrencias);

    // liberar recursos
    free(V);

    return 0;
}

void extraerParams(int argc, char* argv[]){
    if (argc < 3) {
        printf("Especificar N y X\n");
        exit(1);
    }

    N = atoi(argv[1]);
    X = atoi(argv[2]);

    if (N < 1) {
        printf("N debe ser positivo\n");
        exit(2);
    }

    if (X < 0 || X > 999){
        printf("X entre 0 y 999\n");
        exit(3);
    }
}