// Buscar promedio, minimo y máximo de un vector con semaforos

#include <stdio.h>
#include <stdlib.h>

// Librería P-threads
#include <pthread.h>        // hilos
#include <semaphore.h>      // semaforos

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

// variables compartidas
static double *v;
static double minimo = 9999.9, maximo = -9999.9, sumaParcial = 0.0;
static sem_t semaforo;

// estructura de params
typedef struct {
    int startIndex;
    int querySize;
} pthread_params_t;

// Prototipos de funcion
void* resolver(void* arg);
static void init_vector(double* v, int N);

int main(int argc,char*argv[]){

    // variables
    int N, T, i;
    double timetick;

    // lectura de argumentos
    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) ){
        printf("\nError: hay que especificar N y T (tamaño del vector y cantidad de hilos)\n");
        exit(1);
    }

    // reservar memoria
    v = (double*) malloc(sizeof(double)*N);

    // rellenar vector
    init_vector(v, N);

    // variables para los threads
    pthread_attr_t attr;
    pthread_t threads[T];
    pthread_params_t params[T];

    // inicializar atributos
    pthread_attr_init(&attr);

    // inicializar semaforo
    sem_init(&semaforo, NULL, 1);   // sem semaforo = 1;

    // marcar inicio de tiempo
    timetick = dwalltime();

    // crear hilos
    for (i=0; i<T; i++){
        params[i].startIndex = i*(N/T);
        params[i].querySize = N/T;

        pthread_create(&threads[i], &attr, resolver, &params[i]);
    }

    // Esperamos a que termine cada hilo
    for (i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }

    // marcamos fin de tiempo
    printf("N=%d, T=%d, min: %f, max: %f, prom: %f. Tiempo en segundos %f\n", N, T, minimo, maximo, sumaParcial / N, dwalltime() - timetick);

    // liberar memoria
    free(v);

    return 0;
}

static void init_vector(double* v, int N){
    int i;

    // nueva seed
    // srand(time(NULL));

    for (i=0; i<N; i++){
        v[i] = (i % 64) + 1.0;
    }
}

void* resolver(void* arg){

    int i;
    double aux, minimoLocal = 9999.9, maximoLocal = -9999.9, sumaLocal = 0.0;
    pthread_params_t* params = (pthread_params_t*) arg;

    int startIndex = (*params).startIndex;
    int querySize = (*params).querySize;

    for (i=0; i<querySize; i++){
        aux = v[startIndex+i];
        if (aux > maximoLocal) maximoLocal = aux;
        if (aux < minimoLocal) minimoLocal = aux;
        sumaLocal += aux;
    }

    // actualizar variables compartidas
    sem_wait(&semaforo);                                // P(semaforo)
    if (minimoLocal < minimo) minimo = minimoLocal;
    if (maximoLocal > maximo) maximo = maximoLocal;
    sumaParcial += sumaLocal;
    sem_post(&semaforo);                                // V(semaforo)

    return NULL;   
}