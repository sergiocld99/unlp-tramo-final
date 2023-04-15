// PARALELIZAR LA ORDENACION POR MEZCLA DE UN VECTOR DE N ELEMENTOS

// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include "tiempo.h"
#include <time.h>       // random seed

// Constantes
// #define DEBUG

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializar();
void ordenarIterativo();
void ordenarPar(int, int);
void combinar(int left, int medio, int right);

// Variables compartidas
int N;
double* V;

// Programa principal
int main(int argc, char* argv[]){
    int i;
    
    extraerParams(argc, argv);

    // N...
    V = (double*) malloc(N * sizeof(double));
    inicializar();

    // mergesort iterativo (para evitar overhead de recursión)
    double t0 = dwalltime();

    ordenarIterativo();

    double t1 = dwalltime();
    printf("Para N=%d, mide %f segundos\n", N, t1 - t0);

    // Verificar ordenacion (menor a mayor)
    double ant = V[0];
    double act;

    for (i=1; i<N; i++){
        act = V[i];
        if (act < ant) printf("Error: V[%d] = %.2f es menor que V[%d] = %.2f \n", i, act, i-1, ant);
        ant = act;
    }

    // liberar memoria
    free(V);

    return 0;
}

// Implemento funciones
static inline int min(int n1, int n2){
    return (n1 < n2) ? n1 : n2;
}

void ordenarIterativo(){
    int lenTrabajo, L, M, R;

    // Ordenar pares
    for (L=0; L < N-1; L+=2){
        ordenarPar(L, L+1);
    }

    // 4, 8, 16
    for (lenTrabajo=4; lenTrabajo <= N; lenTrabajo *= 2){
        // En ultimo len: L = 0, 16 // M = 7, 23 // R = 15, 29
        for (L=0; L < N-1; L += lenTrabajo){
            M = L + lenTrabajo/2 - 1;
            if (M >= N-1) break;    // ya está ordenado

            R = min(L + lenTrabajo - 1, N-1);
            combinar(L, M, R);
        }
    }

    // Si N no es potencia de 2...
    int lastLenTrabajo = lenTrabajo / 2;    // 16
    if (lastLenTrabajo != N) {
        M = lastLenTrabajo - 1;             // 15
        combinar(0, M, N-1);       
    }
}

void ordenarPar(int p1, int p2){
    double aux;
    
    if (V[p1] > V[p2]){
        aux = V[p1];
        V[p1] = V[p2];
        V[p2] = aux;
    }
}

void combinar(int left, int medio, int right){
    int len1 = (medio - left) + 1;
    int len2 = (right - medio);
    int i = 0, j = 0, k;

    // crear arreglos temporales
    double* L = (double*) malloc(len1 * sizeof(double));
    double* R = (double*) malloc(len2 * sizeof(double));

    // copiar datos
    for (k=0; k<len1; k++) L[k] = V[left+k];
    for (k=0; k<len2; k++) R[k] = V[medio+k+1];

    // realizar ordenacion por cada L[i] y R[j]
    for (k=left; k<=right; k++){
        if (i >= len1) V[k] = R[j++];       // si se acabaron los de L, copiar los de R
        else if (j >= len2) V[k] = L[i++];  // si se acabaron los de R, copiar los de L
        else if (L[i] < R[j]) V[k] = L[i++];    // copiar el menor entre L y R
        else V[k] = R[j++];

        #ifdef DEBUG
            printf("V[%d] = %.2f \n", k, V[k]);
        #endif
    }

    // liberar memoria temporal
    free(L);
    free(R);
}

void inicializar(){
    int i;

    srand(time(NULL));
    for (i=0; i<N; i++) V[i] = rand() % 10000;
}

void extraerParams(int argc, char* argv[]){
    if (argc < 2) {
        printf("Especificar N\n");
        exit(1);
    }

    N = atoi(argv[1]);

    if (N <= 0) {
        printf("N debe ser positivo\n");
        exit(2);
    }
}