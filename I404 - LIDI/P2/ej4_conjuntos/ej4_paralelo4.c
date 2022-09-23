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

void init_conjunto(double*,int,int);

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
    printf("N=%d, T=%d, C_size=%d, Tiempo en segundos %f\n", N, T, C_size, dwalltime() - timetick);

    // liberar memoria
    free(A);
    free(B);
    free(C);

    return 0;
}

void init_conjunto(double* Z, int len, int inc){
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

    int i, j, encontrado;
    double B_elem;

    // vector auxiliar de coincidencias (requiere memoria)
    // como máximo, tendré el tamaño de la porción de B que me fue asignada
    double* coincidencias = (double*) malloc(sizeof(double)*cantElem);
    int putIndex = 0;

    // para mejorar la performance, no realizamos llamados a funcion
    // con O3, prácticamente no hay mejora (ya lo optimiza el compilador)

    for (i=0; i<cantElem; i++){
        encontrado = 0;
        B_elem = B[elemInicio+i];

        for (j=0; j<N && !encontrado; j++){
            if (A[j] == B_elem) encontrado = 1;
        }

        // si se encontró el elemento de B en el conjunto A
        if (encontrado){
            // en vez de pedir un mutex lock por cada vez que quiero insertar un elemento...
            // ... puedo almacenar todos los verdaderos internamente y luego hacerlo todo de una

            coincidencias[putIndex] = B_elem;
            putIndex++;

            /*
            pthread_mutex_lock(&mutex);     // esperar por acceso a la SC

            C[C_size] = B_elem;
            C_size++;

            pthread_mutex_unlock(&mutex);   // liberar SC
            */
        }
    }

    // ahora si, agrego todas las coincidencias a C (con exclusion mutua)
    pthread_mutex_lock(&mutex);     // esperar por acceso a la SC

    for (i=0; i<putIndex; i++){
        C[C_size] = coincidencias[i];
        C_size++;
    }

    pthread_mutex_unlock(&mutex);   // liberar SC
}