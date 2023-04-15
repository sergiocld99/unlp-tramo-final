// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <time.h>       // random
#include <math.h>       // log2

#include "tiempo.h"
#include "hilos.h"

// prototipos
void extraerParams(int argc, char* argv[]);
void* contar(void*);

// shared
int N, X, T;
int *V, *res;
pthread_barrier_t* barreras;

// programa principal
int main(int argc, char* argv[]){
    // variables locales
    int i, ocurrencias;

    extraerParams(argc, argv);
    srand(time(NULL));

    // Una vez sabido N...
    V = (int*) malloc(N * sizeof(int));
    for (i=0; i<N; i++) V[i] = rand() % 1000;
    
    // Una vez sabido T...
    barreras = (pthread_barrier_t*) malloc(log2(T) * sizeof(pthread_barrier_t));
    for (i=0; i<log2(T); i++) pthread_barrier_init(&barreras[i], NULL, T / pow(2,i));
    res = (int*) malloc(T * sizeof(int));

    // Medir
    double t0 = dwalltime();

    create_and_join(&contar, T);
    ocurrencias = res[0] + res[T/2];
    
    double t1 = dwalltime();
    printf("N = %d, ocurrencias = %d, tiempo segundos = %f\n", N, ocurrencias, t1 - t0);

    // verificacion
    for (i=0; i<N; i++){
        if (V[i] == X) ocurrencias--;
    }

    if (ocurrencias < 0) printf("ERROR: contaste %d ocurrencias de más\n", -ocurrencias);
    else if (ocurrencias > 0) printf("ERROR: te faltaron contar %d ocurrencias\n", ocurrencias);

    // liberar recursos
    for (i=0; i<log2(T); i++) pthread_barrier_destroy(&barreras[i]);
    free(barreras);
    free(res);
    free(V);

    return 0;
}

// funciones
void* contar(void* arg){
    const int id = *(int*) arg;           // Constante entre etapas
    const int left = id * (N / T);        // Constante entre etapas
    const int right = (id+1) * (N / T);
    int i, porciones, count = 0;

    // Etapa 1: contar localmente
    for (i=left; i<right; i++){
        if (V[i] == X) count++;
    }

    res[id] = count;    // visible para el resto de hilos

    // printf("ETAPA i=0 - Hilo %d contó %d ocurrencias en su porción\n", id, res[id]);

    // Etapa 2: combinar de a pares
    pthread_barrier_wait(&barreras[0]);

    for (i=1; i<log2(T); i++){
        porciones = pow(2,i);
        if (id % porciones != 0) break;

        // Me toca trabajar...
        res[id] = res[id] + res[id+i];    // merge
        res[id+i] = 0;

        // printf("ETAPA i=%d - Hilo %d combinó %d ocurrencias\n", i, id, res[id]);
        pthread_barrier_wait(&barreras[i]);
    }

    // Ejemplo Etapa 2 para T = 8 (3 barreras)

    // X X X X X X X X  (fin i=0)
    // X 0 X 0 X 0 X 0  (fin i=1)
    // X 0 0 0 X 0 0 0  (fin i=2)
    // X 0 0 0 0 0 0 0  (fin i=3, en el master)

    pthread_exit(NULL);
}

void extraerParams(int argc, char* argv[]){
    if (argc < 4) {
        printf("Especificar N, X y T\n");
        exit(1);
    }

    N = atoi(argv[1]);
    X = atoi(argv[2]);
    T = atoi(argv[3]);

    if (N < 1 || T < 1) {
        printf("N y T deben ser positivos\n");
        exit(2);
    }

    if (X < 0 || X > 999){
        printf("X entre 0 y 999\n");
        exit(3);
    }
}