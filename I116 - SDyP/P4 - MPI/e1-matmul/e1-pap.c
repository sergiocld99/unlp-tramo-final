#include <stdio.h>
#include <stdlib.h>     // atoi
#include "tiempo.h"     // dwalltime
#include <mpi.h>

// macros
#define POR_FILA(i,j) (i*N+j)
#define POR_COL(i,j) (i+j*N)

// constantes
#define TAG 0

// NO EXISTEN VARIABLES COMPARTIDAS EN MPI
int getMatrixSize(int argc, char* argv[]);
void matmul(int cp, double* A, double *B, double *C, int N);
void verificar(double*, int);
void p0(int, int);
void p1(int, int);

// programa principal
int main(int argc, char* argv[]){

    // siempre primero esto!!
    MPI_Init(&argc, &argv);

    // variables locales
    int id, i, j, k;
    int cantProcesos;
    int elementoInicio;
    int N = getMatrixSize(argc, argv);
    double *A, *B, *C;

    // obtener id y cant procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    if (N < 1) {
        printf("ERROR: Hay que pasar N positivo\n");
        MPI_Finalize();
        return 1;
    }

    // realizar operacion
    double T0 = dwalltime();

    if (id == 0) p0(cantProcesos, N);
    else p1(cantProcesos, N);

    double T1 = dwalltime();
    printf("TIEMPO TOTAL DEL PROCESO %d: %f segundos\n", id, T1 - T0);

    // siempre por ultimo esto!!
    MPI_Finalize();

    return 0;
}

// control de argumentos
int getMatrixSize(int argc, char* argv[]){
    if (argc < 2) return -1;
    return atoi(argv[1]);
}

// funcion que ejecuta el proceso con id=0
void p0(int cp, int N){
    int i, j, k, elementoInicio;
    MPI_Status status;

    int elementosProceso = (N/cp) * N;
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

    // enviar porción de A y B completa a los otros procesos
    for (i=1; i<cp; i++){
        elementoInicio = i * elementosProceso;
        MPI_Send(A+elementoInicio, elementosProceso, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
        MPI_Send(B, N*N, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
    }

    // realizar mi parte
    matmul(cp, A, B, C, N);

    // recibir resultados del resto
    for (i=1; i<cp; i++){
        elementoInicio = i * elementosProceso;
        MPI_Recv(C+elementoInicio, elementosProceso, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD, &status);
    }

    double t1 = dwalltime();
    printf("Comunicación + cómputo del master: %f segundos\n", t1 - t0);

    verificar(C, N);

    // liberar recursos
    free(A); 
    free(B); 
    free(C);
}

void p1(int cp, int N){
    int i, j, k;
    int filas = N / cp;
    MPI_Status status;

    double* A = (double*) malloc(filas*N*sizeof(double));
    double* B = (double*) malloc(N*N*sizeof(double));
    double* C = (double*) malloc(filas*N*sizeof(double));

    // inicializarlas
    for (i=0; i<filas; i++){
        for (j=0; j<N; j++){
            C[POR_FILA(i,j)] = 0;   // por filas
        }
    }

    double t0 = dwalltime();

    // recibir porción de A y B completa
    MPI_Recv(A, filas*N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
    MPI_Recv(B, N*N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

    // realizar multiplicacion
    matmul(cp, A, B, C, N);

    // enviar resultados
    MPI_Send(C, filas*N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);

    double t1 = dwalltime();
    printf("Comunicación + cómputo de proceso secundario: %f segundos\n", t1 - t0);

    // liberar recursos
    free(A);
    free(B);
    free(C);
}


void matmul(int cp, double* A, double *B, double *C, int N){
    int i,j,k;
    int filas = N/cp;

    // realizar multiplicacion
    for (i=0; i<filas; i++){
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