// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/time.h>   // dwalltime
#include <pthread.h>

// Constantes
#define SIN_ORDEN 0
#define ORDEN_CRECIENTE 1
#define ORDEN_DECRECIENTE 2
#define ELEMENTOS_POR_HILO(N,T) (N / T)

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializarVector(int orden);
double dwalltime();

void* determinar(void* arg);

int verificarSinOrden();
int verificarCreciente();
int verificarDecreciente();

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
    pthread_t hilos[T];
    int ids[T];

    // alocar memoria
    V = (double*) malloc(N * sizeof(double));
    ords = (int*) malloc(T * sizeof(int));
    inicializarVector(ORDEN);

    // operacion a medir
    double t0 = dwalltime();

    for (i=0; i<T; i++){
        ids[i] = i;
        pthread_create(&hilos[i], NULL, &determinar, &ids[i]);
    }

    for (i=0; i<T; i++){
        pthread_join(hilos[i], NULL);
    }

    // Si el hilo 1 dice sin orden y el 2 sin orden, queda sin orden
    // Si el hilo 1 dice sin orden y el 2 creciente, es creciente (nuevo)
    // Si el hilo 1 dice creciente y el 2 sin orden, es creciente (anterior)
    // Si el hilo 1 dice creciente y el 2 creciente, es creciente (anterior)
    // Si el hilo 1 dice creciente y el 2 decreciente, ya no es monotonico

    // buscar el primer orden creciente o decreciente
    orden = ords[0];
    i = 1;

    while ((orden == SIN_ORDEN) && (i < T)){
        orden = ords[i++];
    }

    // comprobar que el resto de hilos dijeron el mismo orden
    while ((orden != SIN_ORDEN) && (i < T)){
        if (ords[i++] != orden) orden = SIN_ORDEN;
    }

    double t1 = dwalltime();
    printf("Para N=%d, mide %f segundos\n", N, t1 - t0);

    // mostrar resultado y verificar
    if (orden == SIN_ORDEN) printf("Resultado: El vector no es monotónico\n");
    else printf("Resultado: El vector SÍ ES monotónico\n");

    switch(orden){
        case SIN_ORDEN:
            if (!verificarSinOrden()) printf("ERROR: El vector si tiene un orden\n");
            break;
        case ORDEN_CRECIENTE:
            i = verificarCreciente();
            if (i > 0) printf("ERROR: El vector no es creciente, hay %d incoherencias\n", i);
            break;
        case ORDEN_DECRECIENTE:
            i = verificarDecreciente();
            if (i > 0) printf("ERROR: El vector no es decreciente, hay %d incoherencias\n", i);
            break;
    }

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
    int i = left + 1, orden = SIN_ORDEN;

    while (orden == SIN_ORDEN && i < right){
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

void inicializarVector(int orden){
    int i;

    if (orden == ORDEN_CRECIENTE) {
        for (i=0; i<N; i++) V[i] = i / 4;
    } else if (orden == ORDEN_DECRECIENTE) {
        for (i=0; i<N; i++) V[i] = N-i;
    } else {
        for (i=0; i<N; i++) V[i] = i % 2;
    }
}

int verificarSinOrden(){
    int erroresC = verificarCreciente();
    int erroresD = verificarDecreciente();

    // Decimos que no tiene orden si no es creciente ni decreciente
    // O bien, si es ambos al mismo tiempo (todos sus elementos son iguales)
    return (erroresC > 0 && erroresD > 0) || (erroresC == 0 && erroresD == 0);
}

int verificarCreciente(){
    int i, errores = 0;

    for (i=1; i<N; i++){
        if (V[i] < V[i-1]) errores++;
    }

    return errores;
}

int verificarDecreciente(){
    int i, errores = 0;

    for (i=1; i<N; i++){
        if (V[i] > V[i-1]) errores++;
    }

    return errores;
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