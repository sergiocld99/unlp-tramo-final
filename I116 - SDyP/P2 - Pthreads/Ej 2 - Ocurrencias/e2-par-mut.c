// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <time.h>       // random
#include "tiempo.h"
#include <pthread.h>

// prototipos
void extraerParams(int argc, char* argv[]);
void* contar(void*);

// shared
int N, X, T;
int* V;

pthread_mutex_t mutex;
int ocurrencias = 0;

// programa principal
int main(int argc, char* argv[]){
    // variables locales
    int i;

    extraerParams(argc, argv);
    srand(time(NULL));

    // Una vez sabido N...
    V = (int*) malloc(N * sizeof(int));
    for (i=0; i<N; i++) V[i] = rand() % 1000;
    
    // Una vez sabido T...
    pthread_t hilos[T];
    int ids[T];
    pthread_mutex_init(&mutex, NULL);

    // Medir
    double t0 = dwalltime();

    for (i=0; i<T; i++){
        ids[i] = i;
        pthread_create(&hilos[i], NULL, &contar, &ids[i]);
    }
    
    for (i=0; i<T; i++){
        pthread_join(hilos[i], NULL);
    }

    double t1 = dwalltime();
    
    printf("N = %d, mide %f segundos\n", N, t1 - t0);
    printf("Ocurrencias = %d \n", ocurrencias);

    // verificacion
    for (i=0; i<N; i++){
        if (V[i] == X) ocurrencias--;
    }

    if (ocurrencias != 0) printf("ERROR: conteo mal realizado\n");

    // liberar recursos
    pthread_mutex_destroy(&mutex);
    free(V);

    return 0;
}

// funciones
void* contar(void* arg){
    int id = *(int*) arg;
    int left = id * (N / T);
    int right = (id+1) * (N / T);
    int i, count = 0;

    // Etapa 1: contar localmente
    for (i=left; i<right; i++){
        if (V[i] == X) count++;
    }

    // Etapa 2: actualizar contador global
    pthread_mutex_lock(&mutex);
    ocurrencias += count;
    pthread_mutex_unlock(&mutex);

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