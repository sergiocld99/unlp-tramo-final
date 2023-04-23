#include <stdio.h>
#include <stdlib.h>     // atoi
#include "tiempo.h"     // dwalltime

// macros
#define POR_FILA(i,j) (i*N+j)
#define POR_COL(i,j) (i+j*N)

// NO EXISTEN VARIABLES COMPARTIDAS EN MPI
int getMatrixSize(int argc, char* argv[]);
void matmul(double* A, double *B, double *C, int N);
void verificar(double*, int);

// programa principal
int main(int argc, char* argv[]){

    // variables locales
    int i, j, k;
    int N = getMatrixSize(argc, argv);

    if (N < 1) {
        printf("ERROR: Hay que pasar N positivo\n");
        return 1;
    }

    double* A = (double*) malloc(N*N*sizeof(double));
    double* B = (double*) malloc(N*N*sizeof(double));
    double* C = (double*) malloc(N*N*sizeof(double));

    // inicializarlas
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            A[POR_FILA(i,j)] = 1;   // por filas
            B[POR_COL(i,j)] = 1;   // por columnas
            C[POR_FILA(i,j)] = 0;   // por filas
        }
    }

    double t0 = dwalltime();

    matmul(A, B, C, N);

    double t1 = dwalltime();
    printf("Tiempo secuencial: %f segundos\n", t1 - t0);

    verificar(C, N);

    // liberar recursos
    free(A); 
    free(B); 
    free(C);

    return 0;
}

// control de argumentos
int getMatrixSize(int argc, char* argv[]){
    if (argc < 2) return -1;
    return atoi(argv[1]);
}

void matmul(double* A, double *B, double *C, int N){
    int i,j,k;

    // realizar multiplicacion
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            for (k=0; k<N; k++){
                C[POR_FILA(i,j)] += A[POR_FILA(i,k)] * B[POR_COL(k,j)];
            }
        }
    }
}

void verificar(double* C, int N){
    int i,j;
    
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            if (C[POR_FILA(i,j)] != N) printf("Error en posicion %d, %d \n", i, j);
        }
    }
}