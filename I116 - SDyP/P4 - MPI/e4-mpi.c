#include <stdio.h>
#include <stdlib.h>     // atoi
#include <math.h>       // log2
#include <time.h>       // srand time
#include "tiempo.h"
#include <mpi.h>

// macros
#define min(a,b) (a < b ? a : b)

// NO EXISTEN VARIABLES COMPARTIDAS EN MPI
int getSize(int argc, char* argv[]);
void p0(int N, int cp);
void p1(int N, int cp);

// prototipos comunes
void ordenarVector(double* V, int size);
void combinar(double* V, int left, int medio, int right);

// programa principal
int main(int argc, char* argv[]){

    // siempre primero esto!!
    MPI_Init(&argc, &argv);

    // variables locales
    int id;
    int cantProcesos;
    int N = getSize(argc, argv);

    // obtener id y cant procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    if (N < 1 || N % cantProcesos != 0) {
        printf("ERROR: N debe ser multiplo de cant procesos\n");
        MPI_Finalize();
        return 1;
    }

    if (id == 0) p0(N, cantProcesos);
    else p1(N, cantProcesos);

    // siempre por ultimo esto!!
    MPI_Finalize();

    return 0;
}

// funcion que ejecuta el proceso con id=0
void p0(int N, int cp){
    double* V = (double*) malloc(N*sizeof(double));

    // generar numeros
    srand(time(NULL));

    for(int a=0; a<N; a++){
        V[a] = rand() % 100;
    }

    // repartir porcion a cada proceso
    const int sendcnt = N / cp;
    double t0 = dwalltime();

    // Scatter(sendbuf, sendcount, sendtype, recbuf, reccount, rectype, root, comm)
    MPI_Scatter(V, sendcnt, MPI_DOUBLE, V, sendcnt, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // realizar ordenación de mi porcion asignada
    ordenarVector(V, sendcnt);

    // juntar subvectores ordenados por cada proceso
    // Gather(sendbuf, sendcount, sendtype, recbuf, reccount, rectype, root, comm)
    MPI_Gather(V, sendcnt, MPI_DOUBLE, V, sendcnt, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // mergesort con proceso vecino (master trabaja siempre)
    int i;
    
    // en la etapa i=1 hay cp vectores a juntar y resultan cp/2
    // en la etapa i=2 hay cp/2 vectores a juntar y resultan cp/4
    // en la etapa i=3 hay cp/4 vectores a juntar y resultan cp/8
    for (i=1; i<=log2(cp); i++){
        int porciones = pow(2,i);
        int len = porciones * sendcnt;

        for (int j=0; j<cp/porciones; j++){
            int inicio = j * len;
            int fin = (j+1) * len - 1;
            int medio = (inicio+fin) / 2;

            combinar(V, inicio, medio, fin);
        }
    }

    // vector ordenado!
    printf("Tiempo del proceso root: %f segundos \n", dwalltime() - t0);

    // verificar ordenacion
    int ordenados = 1;

    for (i=1; i<N; i++){
        if (V[i] >= V[i-1]) ordenados++; 
        else printf("Error en posicion %d \n", i);
    }

    printf("Elementos ordenados: %d de %d \n", ordenados, N);

    // liberar recursos
    free(V);
}

// procesos secundarios
void p1(int N, int cp){
    double* vacio;
    
    // repartir porcion a cada proceso
    const int sendcnt = N / cp;

    double* V = (double*) malloc(sendcnt * sizeof(double));

    // Scatter(sendbuf, sendcount, sendtype, recbuf, reccount, rectype, root, comm)
    MPI_Scatter(vacio, 0, MPI_DOUBLE, V, sendcnt, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // realizar ordenación de mi porcion asignada
    ordenarVector(V, sendcnt);

    // enviar mi trabajo realizado al root
    // Gather(sendbuf, sendcount, sendtype, recbuf, reccount, rectype, root, comm)
    MPI_Gather(V, sendcnt, MPI_DOUBLE, vacio, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // liberar recursos
    free(V);
}

// funciones comunes
void ordenarVector(double* V, int size){

    // empezar ordenando de a 2, luego de 4, de a 8 y asi...
    int len, left, medio, right;
    int fin = size - 1;

    for (len=2; len <= size; len *= 2){
        for (left=0; left < size; left += len){
            medio = left + len/2 - 1;
            right = min(left + len - 1, fin);
            
            if (medio >= fin) break;   // ya ordenado
            combinar(V, left, medio, right);
        }
    }

    // ordenar ultima parte sobrante si N no es potencia de 2
    if (len != 2*size){
        medio = len/2 - 1;
        combinar(V, 0, medio, fin);
    }
}

void combinar(double* V, int left, int medio, int right){
    int len1 = (medio - left) + 1;
    int len2 = (right - medio);
    int k;

    // arreglos temporales
    double* L = (double*) malloc(len1 * sizeof(double));
    double* R = (double*) malloc(len2 * sizeof(double));

    // copiar
    for (k=0; k<len1; k++) L[k] = V[left+k];
    for (k=0; k<len2; k++) R[k] = V[medio+k+1];

    // ordenar eligiendo menor entre L y R
    int i=0, j=0;
    
    for (k=left; k<=right; k++){
        if (i >= len1) V[k] = R[j++];
        else if (j >= len2) V[k] = L[i++];
        else if (L[i] < R[j]) V[k] = L[i++];
        else V[k] = R[j++];
    }

    // liberar temporales
    free(L); free(R);
}

// control de argumentos
int getSize(int argc, char* argv[]){
    if (argc < 2) return -1;
    return atoi(argv[1]);
}