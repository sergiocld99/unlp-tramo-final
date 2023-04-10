// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include "tiempo.h"
#include "hilos.h"

// Constantes
#define SIN_ORDEN 0
#define ORDEN_CRECIENTE 1
#define ORDEN_DECRECIENTE 2
#define ORDEN_CONSTANTE 3
#define ELEMENTOS_POR_HILO(N,T) (N / T)

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializarVector();
void* determinar(void* arg);

// Variables compartidas
pthread_barrier_t barrera;      // cada hilo debe chequear su orden
int N, T;
int* ords;                      // el resultado de cada hilo
double* V;

// Programa principal
int main(int argc, char* argv[]){
    int orden, i;

    extraerParams(argc, argv);
    pthread_barrier_init(&barrera, NULL, T);

    // alocar memoria
    V = (double*) malloc(N * sizeof(double));
    ords = (int*) malloc(T * sizeof(int));
    inicializarVector();

    // operacion a medir
    double t0 = dwalltime();

    create_and_join(&determinar, T);

    // Si el hilo 1 dice constante y el 2 constante, queda constante
    // Si el hilo 1 dice constante y el 2 creciente, es creciente (nuevo)
    // Si el hilo 1 dice creciente y el 2 constante, es creciente (anterior)
    // Si el hilo 1 dice creciente y el 2 creciente, es creciente (anterior)
    // Si el hilo 1 dice creciente y el 2 decreciente, ya no es monotonico
    // Si algun hilo dice sin orden, ya no es monotonico

    // buscar el primer orden no constante
    orden = ords[0];
    i = 1;

    while ((orden == ORDEN_CONSTANTE) && (i < T)){
        orden = ords[i++];
    }

    // comprobar que el resto de hilos dijeron lo mismo (ignoro constantes)
    while ((orden != SIN_ORDEN) && (i < T)){
        if (ords[i] != ORDEN_CONSTANTE){
            if (ords[i] != orden) orden = SIN_ORDEN;
        }

        i++;
    }

    double t1 = dwalltime();
    printf("Para N=%d, mide %f segundos\n", N, t1 - t0);

    // mostrar resultado y verificar
    if (orden == SIN_ORDEN || orden == ORDEN_CONSTANTE) printf("Resultado: El vector no es monotónico\n");
    else printf("Resultado: El vector SÍ ES monotónico\n");

    if (orden != ORDEN) printf("ERROR: se obtuvo orden %d y se esperaba %d \n", orden, ORDEN);

    // Liberar recursos
    pthread_barrier_destroy(&barrera);
    free(ords);
    free(V);

    return 0;
}

// Implemento funciones
void* determinar(void* arg){
    int id = *(int*) arg;
    int left = id * ELEMENTOS_POR_HILO(N,T);
    int right = (id+1) * ELEMENTOS_POR_HILO(N,T);

    // Etapa 1: determinar orden en mi porción
    int i = left + 1;
    int orden = ORDEN_CONSTANTE;

    while (orden == ORDEN_CONSTANTE && i < right){
        if (V[i] < V[i-1]) orden = ORDEN_DECRECIENTE;
        else if (V[i] > V[i-1]) orden = ORDEN_CRECIENTE;
        i++;
    }

    // Etapa 2: comprobar que el resto siga ese orden
    if (orden == ORDEN_DECRECIENTE){
        while (orden == ORDEN_DECRECIENTE && i < right){
            if (V[i] > V[i-1]) orden = SIN_ORDEN;
            i++;
        }
    } else if (orden == ORDEN_CRECIENTE){
        while (orden == ORDEN_CRECIENTE && i < right){
            if (V[i] < V[i-1]) orden = SIN_ORDEN;
            i++;
        }
    }
    
    // Etapa 3: verificar en bordes con otros hilos
    pthread_barrier_wait(&barrera);

    // chequear borde izquierdo (si no soy primer hilo)
    if (id > 0){
        if (orden == ORDEN_DECRECIENTE){
            if (V[left] > V[left-1]) orden = SIN_ORDEN;
        } else if (orden == ORDEN_CRECIENTE){
            if (V[left] < V[left-1]) orden = SIN_ORDEN;
        }
    }

    // el borde derecho es el izquierdo del hilo id+1
    ords[id] = orden;

    pthread_exit(NULL);
}

void inicializarVector(){
    int i;

    #if ORDEN == ORDEN_CRECIENTE
        for (i=0; i<N; i++) V[i] = i / 4;
    #elif ORDEN == ORDEN_DECRECIENTE
        for (i=0; i<N; i++) V[i] = N-i;
    #elif ORDEN == ORDEN_CONSTANTE
        for (i=0; i<N; i++) V[i] = 800;
    #else
        for (i=0; i<N; i++) V[i] = i % ELEMENTOS_POR_HILO(N,T);
    #endif
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
