#include <stdio.h>
#include <stdlib.h>     // atoi
#include "tiempo.h"
#include <mpi.h>

// NO EXISTEN VARIABLES COMPARTIDAS EN MPI
int getMatrixSize(int argc, char* argv[]);
void getMinMaxSum(double* mat, int filas, int N, double* MIN, double* MAX, double* SUM);
void crearB(double* A, double* B, int filas, int N, double* globales);
void p0(int, int);

// programa principal
int main(int argc, char* argv[]){

    // siempre primero esto!!
    MPI_Init(&argc, &argv);

    // variables locales
    int id;
    int cantProcesos;
    int N = getMatrixSize(argc, argv);

    // obtener id y cant procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    if (N < 1) {
        printf("ERROR: Hay que pasar N positivo\n");
        MPI_Finalize();
        return 1;
    }

    if (id == 0) p0(N, cantProcesos);

    // siempre por ultimo esto!!
    MPI_Finalize();

    return 0;
}

// funcion que ejecuta el proceso con id=0
void p0(int N, int cp){
    int filas = N / cp;
    int i,j;

    double *A = (double*) malloc(N*N*sizeof(double));
    double *B = (double*) malloc(N*N*sizeof(double));
    double *AL = (double*) malloc(filas*N*sizeof(double));
    double *BL = (double*) malloc(filas*N*sizeof(double));

    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            A[i*N+j] = (i % 2 == 0 ? 10 : 30);
        }
    }

    // Scatter(sendbuf, sendcount, sendtype, recbuf, reccount, rectype, root, comm)
    MPI_Scatter(A, filas*N, MPI_DOUBLE, AL, filas*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // encontrar min, max y prom locales
    double min, max, suma;
    getMinMaxSum(AL, filas, N, &min, &max, &suma);
    printf("Locales: min %.2f, max %.2f, suma %.2f \n", min, max, suma);

    // Reduce(sendbuf, recbuf, count, type, op, root, comm)
    double MIN, MAX, SUM;
    MPI_Reduce(&min, &MIN, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&max, &MAX, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&suma, &SUM, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    printf("Globales: min %.2f, max %.2f, suma %.2f \n", MIN, MAX, SUM);

    double globales[3] = {MIN, MAX, SUM / (N*N)};
    MPI_Bcast(globales, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Crear porción de matriz B
    crearB(AL, BL, filas, N, globales);

    // Gather(sendbuf, sendcount, sendtype, recbuf, reccount, rectype, root, comm)
    MPI_Gather(BL, filas*N, MPI_DOUBLE, B, filas*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Verificar resultados
    int pos;
    double esperado;

    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            pos = i*N+j;
            
            if (A[pos] < globales[2]) esperado = globales[0];
            else if (A[pos] > globales[2]) esperado = globales[1];
            else esperado = globales[2];

            if (B[pos] != esperado){ 
                printf("Error: A[%d] = %.2f, B = %.2f\n", pos, A[pos], B[pos]);
            }
        }
    }

    // Liberar recursos
    free(A); free(B);
    free(AL); free(BL);
}

// Funcion comun
void getMinMaxSum(double* mat, int filas, int N, double* MIN, double* MAX, double* SUM){
    int i,j;
    
    double min = mat[0];
    double max = mat[0];
    double suma = 0.0;
    double elem;

    for (i=0; i<filas; i++){
        for (j=0; j<N; j++){
            elem = mat[i*N+j];
            suma += elem;
            if (elem < min) min = elem;
            if (elem > max) max = elem;
        }
    }

    // retornar resultados
    *MIN = min;
    *MAX = max;
    *SUM = suma;
}

void crearB(double* A, double* B, int filas, int N, double* globales){
    int i, j, pos;

    const double min = globales[0];
    const double max = globales[1];
    const double prom = globales[2];

    printf("Para crear B: min %.2f, max %.2f, prom %.2f \n", min, max, prom);

    for (i=0; i<filas; i++){
        for (j=0; j<N; j++){
            pos = i*N+j;
            if (A[pos] < prom) B[pos] = min;
            else if (A[pos] > prom) B[pos] = max;
            else B[pos] = prom;
        }
    }
}


// control de argumentos
int getMatrixSize(int argc, char* argv[]){
    if (argc < 2) return -1;
    return atoi(argv[1]);
}