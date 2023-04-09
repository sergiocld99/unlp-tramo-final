// PARALELIZAR LA ORDENACION POR MEZCLA DE UN VECTOR DE N ELEMENTOS

// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/time.h>   // dwalltime
#include <time.h>       // random seed
#include <pthread.h>

// Constantes
#define ELEMENTOS_POR_HILO(N,T) (N/T)
// #define DEBUG

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializar(double*);
void ordenar(int left, int right);
void combinar(int left, int medio, int right);
void crearBarreras();

void* mergeSort(void*);
double dwalltime();

// Variables compartidas
int N, T;
double* V;
pthread_barrier_t* barreras;

// Programa principal
int main(int argc, char* argv[]){
    int i;
    
    extraerParams(argc, argv);

    // Una vez extraido T...
    pthread_t hilos[T];
    int ids[T];

    // Si trabajan T, luego T/2, T/4... 1, son 2^T - 1 barreras
    barreras = (pthread_barrier_t*) malloc(T * sizeof(pthread_barrier_t));
    for (i=1; i<=T; i*=2) pthread_barrier_init(&barreras[i-1], NULL, T/i);

    // Una vez extraido N...
    V = (double*) malloc(N * sizeof(double));
    inicializar(V);

    // mergesort
    double t0 = dwalltime();

    for (i=0; i<T; i++){
        ids[i] = i;
        pthread_create(&hilos[i], NULL, &mergeSort, &ids[i]);
    }

    for (i=0; i<T; i++){
        pthread_join(hilos[i], NULL);
    }

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

    // liberar recursos
    for (i=1; i<=T; i*=2) pthread_barrier_destroy(&barreras[i-1]);
    
    free(barreras);
    free(V);

    return 0;
}

// Implemento funciones
inline int min(int n1, int n2){
    return (n1 < n2) ? n1 : n2;
}

// Primero se divide el vector en T partes
// Cada hilo ordena completamente su porción asignada (T hilos trabajando)
// Luego se combinan las porciones ordenadas de a pares (T/2 hilos trabajando)
// Se repite lo anterior hasta ordenar el vector completo (1 hilo trabajando)
void* mergeSort(void* arg){
    int id = *(int*) arg;
    int left = id * ELEMENTOS_POR_HILO(N, T);
    int right = (id+1) * ELEMENTOS_POR_HILO(N, T) - 1;
    int medio;

    #ifdef DEBUG
        printf("Hilo %d, left %d, right %d \n", id, left, right);
    #endif

    // Etapa 1: ordenar porción asignada
    ordenar(left, right);
    pthread_barrier_wait(&barreras[0]);

    // Etapa 2: combinar de a pares
    int i;      // i representa la cantidad de porciones a combinar

    for (i=2; i<=T; i*=2){
        if (id % i == 0){
            // Combinar entre mi inicio y el fin de quien fue mi siguiente
            right = (id + i) * ELEMENTOS_POR_HILO(N, T) - 1;
            medio = left + (right - left) / 2;

            #ifdef DEBUG
                printf("Hilo %d, left %d, right %d \n", id, left, right);
            #endif

            combinar(left, medio, right);

            if (i != T) pthread_barrier_wait(&barreras[i-1]);     // fin sincronizado
        }
    }


    // pthread_barrier_wait(&barrera);
    // if (id == 0) combinar(0, (N-1)/2, N-1);

    pthread_exit(NULL);
}

void ordenar(int left, int right){
    if (left >= right) return;

    // l = 2, r = 8 -> m = 2 + 3 = 5
    int len = right - left;
    int medio = left + len / 2;

    // ordenar parte izquierda (2 a 5)
    ordenar(left, medio);

    // ordenar parte derecha (6 a 8)
    ordenar(medio+1, right);

    // combinar vectores ordenados
    combinar(left, medio, right);
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
    }

    // liberar memoria temporal
    free(L);
    free(R);
}

void crearBarreras(){
    int i;

    pthread_barrier_init(&barreras[0], NULL, T);
}

void inicializar(double* V){
    int i;

    srand(time(NULL));

    for (i=0; i<N; i++){
        V[i] = rand() % 10000;
    }
}

void extraerParams(int argc, char* argv[]){
    if (argc < 3) {
        printf("Especificar N y T\n");
        exit(1);
    }

    N = atoi(argv[1]);
    T = atoi(argv[2]);

    if (N < 1 || T < 1) {
        printf("N debe ser positivo\n");
        exit(2);
    }

    if (N % T != 0){
        printf("Por favor, utilice N multiplo de T\n");
        exit(3);
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