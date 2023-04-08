// Esta es la versión secuencial de la multiplicacion de matrices
// Para calcular Speedup deberia usar "el mejor secuencial"

// Cabeceras
#include <stdio.h>      // para printf
#include <stdlib.h>     // para exit
#include <sys/time.h>   // para dwalltime

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializarMatrices(double *A, double *B, double *C);
double dwalltime();

// Variables compartidas
int N;

// Programa principal
int main(int argc, char* argv[]){
    int i, j, k;
    double *A, *B, *C;      // Matrices

    extraerParams(argc, argv);

    A = (double*) malloc(N * N * sizeof(double));
    B = (double*) malloc(N * N * sizeof(double));
    C = (double*) malloc(N * N * sizeof(double));

    inicializarMatrices(A, B, C);

    // Realizar multiplicacion
    double t0 = dwalltime();

    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            for (k=0; k<N; k++){
                // A por filas, B por columnas
                C[i*N+j] += A[i*N+k] * B[k+j*N];
            }
        }
    }

    // Imprimir medicion
    double t1 = dwalltime();
    printf("Para N=%d mide %f segundos\n", N, t1 - t0);

    // Verificar resultado
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            if (C[i*N+j] != N) printf("Error en posicion %d, %d\n", i, j);
        }
    }

    // Liberar memoria
    free(A);
    free(B);
    free(C);

    return 0;
}

// Implemento funciones
void extraerParams(int argc, char* argv[]){
    // El tamaño de matriz (N) y la cantidad de hilos (T) son argumentos
    // El primer argumento (indice 0) es la ruta del propio programa
    if (argc < 2)
    {
        printf("Capo, necesito el tamaño de matriz!\n");
        exit(1);
    }

    // Extraer argumentos
    N = atoi(argv[1]);

    // Controlar validez de N y T
    if (N < 1)
    {
        printf("Tenes que pasarme N positivo!!\n");
        exit(2);
    }
}

void inicializarMatrices(double *A, double *B, double *C){
    int i, j;

    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            A[i*N+j] = 1;   // Por filas
            B[i+j*N] = 1;   // Por columnas
            C[i*N+j] = 0;   // Por filas
        }
    }
}

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}