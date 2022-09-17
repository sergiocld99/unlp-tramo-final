// Desarrolle un algoritmo paralelo que obtenga la intersección de dos conjuntos de números enteros.

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

static void init_conjunto(double*,int,int);
static void intersectar(double,int);
static void insertar(double elem, double* conjunto);

// Prototipo de funcion llamada por hilos
void* resolver(void* arg);

// variables compartidas
double* A;
double* B;
double* C;          // conjunto resultado
int C_size = 0;     // indice donde insertar en C
pthread_mutex_t mutex;

// estructura de parametros
typedef struct {
    int elemInicio;
    int cantElem;
    int N;
} pthread_params_t;

int main(int argc,char*argv[])
{
    int i;                  // indices
    double timetick;        // medida inicial de tiempo
    int N;                  // tamaño de los conjuntos
    int T;                  // numero de threads

    // leer argumento: cantidad de threads
    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) ){
        printf("\nError: hay que especificar N y T (tamaño de conjuntos y cantidad de hilos)\n");
        exit(1);
    }

    // reservar memoria
    A = (double*) malloc(sizeof(double)*N);
    B = (double*) malloc(sizeof(double)*N);
    C = (double*) malloc(sizeof(double)*N);

    // inicializar conjuntos A y B
    init_conjunto(A, N, 1);
    init_conjunto(B, N, 2);

    // variables de pthreads
    pthread_attr_t attr;
    pthread_t threads[T];
    pthread_params_t params[T];

    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);

    // marcamos inicio de tiempo
    timetick = dwalltime();

    // Creamos los hilos
    for (i=0; i<T; i++){
        params[i].elemInicio = i*(N/T);
        params[i].cantElem = N/T;
        params[i].N = N;

        // cada thread realiza una interseccion con un subconjunto de B
        pthread_create(&threads[i], &attr, resolver, &params[i]);
    }

    // Esperamos a que termine cada hilo
    for (i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }

    // marcamos fin de tiempo
    printf("T=%d, C_size=%d, Tiempo en segundos %f\n", T, C_size, dwalltime() - timetick);

    // liberar memoria
    free(A);
    free(B);
    free(C);

    return 0;
}

static void init_conjunto(double* Z, int len, int inc){
    int i=0;

    for (i=0; i<len; i++){
        Z[i] = i * inc;
    }
}

void* resolver(void* arg){
    pthread_params_t* params = (pthread_params_t*) arg;
    int elemInicio = (*params).elemInicio;
    int cantElem = (*params).cantElem;
    int N = (*params).N;

    int i;      // indice

    for (i=0; i<cantElem; i++){
        intersectar(B[elemInicio+i], N);
    }
}

static void intersectar(double B_elem, int N){

    int i, encontrado = 0;

    for (i=0; i<N && !encontrado; i++){
        if (A[i] == B_elem) encontrado = 1;
    }

    // si se encontró el elemento de B en el conjunto A
    if (encontrado){
        pthread_mutex_lock(&mutex);     // esperar por acceso a la SC
        insertar(B_elem, C);
        pthread_mutex_unlock(&mutex);   // liberar SC
    }
}

// EJECUTAR CON EXCLUSION MUTUA UNICAMENTE
static void insertar(double elem, double* conjunto){
    conjunto[C_size] = elem;
    C_size++;
}