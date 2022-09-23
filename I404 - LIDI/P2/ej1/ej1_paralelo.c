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

// Prototipo de funcion llamada por hilos
void* resolver(void* arg);

// variables compartidas por los threads
static double* A;
static double* B;
static double* C;
static int N;                  // cantidad de filas = columnas
static int T;                  // cantidad de hilos a ejecutar

// estructura de parametros
typedef struct {
    int filaInicio;
    int cantFilas;
} pthread_params_t;

int main(int argc,char*argv[])
{
    int i,j;                  // indices
    double timetick;        // medida inicial de tiempo

    // chequeo de argumentos
    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) ){
        printf("\nError: hay que especificar N y T (tamaño de la matriz y cantidad de hilos)\n");
        exit(1);
    }

    // variables de pthreads
    pthread_t threads[T];   // los hilos en cuestión
    pthread_params_t params[T];     // los parametros para cada hilo
    pthread_attr_t attr;    // los atributos para los hilos

    // matrices
    A = (double*) malloc(sizeof(double)*N*N);
    B = (double*) malloc(sizeof(double)*N*N);
    C = (double*) malloc(sizeof(double)*N*N);

    // inicializar atributos
    pthread_attr_init(&attr);

    // inicializar matrices (operandos con 1, resultado con 0)
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            A[i*N+j] = 1;       // matriz ordenada por filas
            B[i+N*j] = 1;       // matriz ordenada por columnas
            C[i*N+j] = 0;       // matriz ordenada por filas
        }
    }

    // marcamos inicio de tiempo
    timetick = dwalltime();

    // Creamos los hilos
    for (i=0; i<T; i++){
        params[i].filaInicio = i*(N/T);
        params[i].cantFilas = N/T;

        pthread_create(&threads[i], &attr, resolver, &params[i]);
    }

    // Esperamos a que termine cada hilo
    for (i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }

    // marcamos fin de tiempo
    printf("N=%d, T=%d, Tiempo en segundos %f\n", N, T, dwalltime() - timetick);

    // verificamos resultados (todas las celdas deben valer N)
    int errores = 0;

    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            if (C[i*N+j] != N){
                printf("Error en la celda [%d,%d]\n", i,j);
                errores++;
            } 
        }
    }

    if (errores) printf("Ejecución terminada con %d errores\n", errores);
    else printf("Ejecución finalizada con éxito!\n");

    // liberar memoria
    free(A);
    free(B);
    free(C);

    return 0;
}

// FUNCION LLAMADA POR CADA UNO DE LOS HILOS (Con diferente argumento)
void* resolver(void* arg){
    pthread_params_t* params = (pthread_params_t*) arg;
    int filaInicio = (*params).filaInicio;
    int filaFin = filaInicio + (*params).cantFilas;
    int x, y, z;                // indices para los bucles
    double suma;                // suma parcial

    for (x=filaInicio; x<filaFin; x++){
        for (y=0; y<N; y++){
            suma = 0;

            // recorremos una fila de A y una columna de B
            for (z=0; z<N; z++){
                suma += A[x*N+z] * B[z+y*N];
            }

            // guardamos resultado
            C[x*N+y] = suma;
        }
    }

    return NULL;
}