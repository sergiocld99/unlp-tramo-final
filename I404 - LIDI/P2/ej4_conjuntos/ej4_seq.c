// Desarrolle un algoritmo paralelo que obtenga la intersección de dos conjuntos de números enteros.

#include <stdio.h>
#include <stdlib.h>

// Librería P-threads
#include <pthread.h>

// Cabecera para dwall time
#include <sys/time.h>

// ******************************* DWALL TIME *********************** //

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

// ----------------------------------------------------------------------

static void init_conjunto(double*,int,int);
static void intersectar(double, int);

// variables compartidas
double* A;
double* B;
double* C;          // conjunto resultado
int C_size = 0;     // indice donde insertar en C

int main(int argc,char*argv[])
{
    int i;                  // indices
    double timetick;        // medida inicial de tiempo
    int N;                  // tamaño de los conjuntos

    // leer argumento: cantidad de threads
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ){
        printf("\nError: hay que especificar N (tamaño de conjuntos)\n");
        exit(1);
    }

    // reservar memoria
    A = (double*) malloc(sizeof(double)*N);
    B = (double*) malloc(sizeof(double)*N);
    C = (double*) malloc(sizeof(double)*N);

    // inicializar conjuntos A y B
    init_conjunto(A, N, 1);
    init_conjunto(B, N, 2);

    // marcamos inicio de tiempo
    timetick = dwalltime();

    for (i=0; i<N; i++){
        intersectar(B[i], N);
    }

    // marcamos fin de tiempo
    printf("N=%d, C_size=%d, Tiempo en segundos %f\n", N, C_size, dwalltime() - timetick);

    // liberar memoria
    free(A);
    free(B);
    free(C);

    return 0;
}

static void init_conjunto(double* Z, int len, int inc){
    int i=0;

    for (i=0; i<len; i++){
        Z[i] = i * inc;
    }
}

static void intersectar(double B_elem, int N){

    int i, encontrado = 0;

    for (i=0; i<N && !encontrado; i++){
        if (A[i] == B_elem) encontrado = 1;
    }

    // si se encontró el elemento de B en el conjunto A
    if (encontrado){
        C[C_size] = B_elem;
        C_size++;
    }
}