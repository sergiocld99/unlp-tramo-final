// PARALELIZAR LA ORDENACION POR MEZCLA DE UN VECTOR DE N ELEMENTOS

// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/time.h>   // dwalltime
#include <time.h>       // random seed

// Constantes
// #define DEBUG

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializar(double*);
void ordenar(double* V, int left, int right);
void combinar(double* V, int left, int medio, int right);
double dwalltime();

// Variables compartidas
int N;

// Programa principal
int main(int argc, char* argv[]){
    int i;
    
    extraerParams(argc, argv);

    // alocar memoria
    double* V = (double*) malloc(N * sizeof(double));

    // inicializar vector con numeros random
    inicializar(V);

    // mergesort iterativo (para evitar overhead de recursión)
    double t0 = dwalltime();

    ordenar(V, 0, N-1);

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
inline int min(int n1, int n2){
    return (n1 < n2) ? n1 : n2;
}

void ordenar(double* V, int left, int right){
    if (left >= right) return;

    // l = 2, r = 8 -> m = 2 + 3 = 5
    int len = right - left;
    int medio = left + len / 2;

    // ordenar parte izquierda (2 a 5)
    ordenar(V, left, medio);

    // ordenar parte derecha (6 a 8)
    ordenar(V, medio+1, right);

    // combinar vectores ordenados
    combinar(V, left, medio, right);
}

void combinar(double* V, int left, int medio, int right){
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

void inicializar(double* V){
    int i;

    srand(time(NULL));

    for (i=0; i<N; i++){
        V[i] = rand() % 10000;

        #ifdef DEBUG
            printf("V[%d] = %.2f \n", i, V[i]);
        #endif
    }
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

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}