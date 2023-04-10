// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include "tiempo.h"

// Constantes
#define SIN_ORDEN 0
#define ORDEN_CRECIENTE 1
#define ORDEN_DECRECIENTE 2
#define ORDEN_CONSTANTE 3

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializarVector();

// Variables compartidas
int N;
double* V;

// Programa principal
int main(int argc, char* argv[]){
    int orden = ORDEN_CONSTANTE;
    int i = 1;
    
    extraerParams(argc, argv);

    // alocar memoria
    V = (double*) malloc(N * sizeof(double));
    inicializarVector();

    // operacion a medir
    double t0 = dwalltime();

    // determinar orden a partir de los primeros elementos
    while (orden == ORDEN_CONSTANTE && i < N){
        if (V[i] > V[i-1]) orden = ORDEN_CRECIENTE;
        else if (V[i] < V[i-1]) orden = ORDEN_DECRECIENTE;

        i++;
    }

    // chequear que los siguientes elementos respeten dicho orden
    if (orden == ORDEN_CRECIENTE){
        while (orden == ORDEN_CRECIENTE && i < N){
            if (V[i] < V[i-1]) orden = SIN_ORDEN;
            i++;
        }
    } else if (orden == ORDEN_DECRECIENTE){
        while (orden == ORDEN_DECRECIENTE && i < N){
            if (V[i] > V[i-1]) orden = SIN_ORDEN;
            i++;
        }
    }

    double t1 = dwalltime();
    printf("Para N=%d, mide %f segundos\n", N, t1 - t0);

    // mostrar resultado y verificar
    if (orden == SIN_ORDEN) printf("Resultado: El vector no es monotónico\n");
    else printf("Resultado: El vector SÍ ES monotónico\n");

    if (orden != ORDEN) printf("ERROR: se obtuvo orden %d y se esperaba %d \n", orden, ORDEN);

    free(V);

    return 0;
}

// Implemento funciones
void inicializarVector(int orden){
    int i;

    #if ORDEN == ORDEN_CRECIENTE
        for (i=0; i<N; i++) V[i] = i / 4;
    #elif ORDEN == ORDEN_DECRECIENTE
        for (i=0; i<N; i++) V[i] = N-i;
    #elif ORDEN == ORDEN_CONSTANTE
        for (i=0; i<N; i++) V[i] = 800;
    #else
        for (i=0; i<N; i++) V[i] = i % 2;
    #endif
}

void extraerParams(int argc, char* argv[]){
    if (argc < 2) {
        printf("Especificar N\n");
        exit(1);
    }

    N = atoi(argv[1]);

    if (N < 2) {
        printf("N debe ser mayor o igual a 2\n");
        exit(2);
    }
}
