// Dado un vector de N elementos, contar las ocurrencias de un numero dado

#include <stdio.h>
#include <stdlib.h>

// Librería P-threads
#include <pthread.h>

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
static int *v;
static const int X = 10;
static int ocurrencias = 0;
static pthread_mutex_t mutex;

// estructura de params
typedef struct {
    int startIndex;
    int querySize;
} pthread_params_t;

// Prototipos de funcion
void* resolver(void* arg);
static void init_vector(int* v, int N, int max);

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
    v = (int*) malloc(sizeof(int)*N);

    // rellenar vector
    init_vector(v, N, 2*X);

    // variables para los threads
    pthread_attr_t attr;
    pthread_t threads[T];
    pthread_params_t params[T];

    // inicializar atributos
    pthread_attr_init(&attr);

    // inicializar mutex
    pthread_mutex_init(&mutex, NULL);

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
    printf("N=%d, T=%d, x=%d, ocurrencias=%d. Tiempo en segundos %f\n", N, T, X, ocurrencias, dwalltime() - timetick);

    // liberar memoria
    free(v);

    return 0;
}

static void init_vector(int* v, int N, int max){
    int i;

    // nueva seed
    // srand(time(NULL));

    for (i=0; i<N; i++){
        // asignar numero entre 0 y 2X
        //v[i] = rand() % max;
        v[i] = i % max;
    }
}

void* resolver(void* arg){

    int i, ocurrenciasLocal = 0;
    pthread_params_t* params = (pthread_params_t*) arg;
    int startIndex = (*params).startIndex;
    int querySize = (*params).querySize;

    for (i=0; i<querySize; i++){
        if (v[startIndex+i] == X) ocurrenciasLocal++;
    }

    // actualizar variable compartida
    pthread_mutex_lock(&mutex);
    ocurrencias += ocurrenciasLocal;
    pthread_mutex_unlock(&mutex);

    return NULL;   
}