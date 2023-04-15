// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <time.h>       // random
#include <math.h>       // log2

#include "tiempo.h"
#include "hilos.h"

// prototipos
void extraerParams(int argc, char* argv[]);
void* sumar(void*);

// shared
int N, T;
int *V;
double *res;
pthread_barrier_t* barreras;

// programa principal
int main(int argc, char* argv[]){
    // variables locales
    int i;
    double promedio;

    extraerParams(argc, argv);
    srand(time(NULL));

    // Una vez sabido N...
    V = (int*) malloc(N * sizeof(int));
    for (i=0; i<N; i++) V[i] = rand() % 1000;
    
    // Una vez sabido T...
    barreras = (pthread_barrier_t*) malloc(log2(T) * sizeof(pthread_barrier_t));
    for (i=0; i<log2(T); i++) pthread_barrier_init(&barreras[i], NULL, T / pow(2,i));
    res = (double*) malloc(T * sizeof(double));

    // Medir
    double t0 = dwalltime();

    create_and_join(&sumar, T);
    promedio = (res[0] + res[T/2]) / N;
    
    double t1 = dwalltime();
    printf("N = %d, promedio = %f, tiempo segundos = %f\n", N, promedio, t1 - t0);

    // verificacion
    double suma = 0.0;

    for (i=0; i<N; i++) suma += V[i];
    if (promedio != suma / N) printf("ERROR: Promedio no coincide\n");

    // liberar recursos
    for (i=0; i<log2(T); i++) pthread_barrier_destroy(&barreras[i]);
    free(barreras);
    free(res);
    free(V);

    return 0;
}

// funciones
void* sumar(void* arg){
    const int id = *(int*) arg;           // Constante entre etapas
    const int left = id * (N / T);        // Constante entre etapas
    const int right = (id+1) * (N / T);
    int i, porciones;
    double suma = 0.0;

    // Etapa 1: contar localmente
    for (i=left; i<right; i++){
        suma += V[i];
    }

    res[id] = suma;    // visible para el resto de hilos

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
    if (argc < 3) {
        printf("Especificar N y T\n");
        exit(1);
    }

    N = atoi(argv[1]);
    T = atoi(argv[2]);

    if (N < 1 || T < 1) {
        printf("N y T deben ser positivos\n");
        exit(2);
    }
}