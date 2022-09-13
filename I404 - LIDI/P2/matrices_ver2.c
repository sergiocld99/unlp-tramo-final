#include <stdio.h>
#include <stdlib.h>

// Librería P-threads
#include <pthread.h>

// Cabecera para dwall time
#include <sys/time.h>

// estructura de parametros
typedef struct {
    double *a;
    double *b;
    double *c;
    int n;
    int T;
    int id;
} pthread_params_t;

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

int main(int argc,char*argv[])
{
    int i,j,N,T;                  // indice auxiliar
    pthread_attr_t attr;    // los atributos para los hilos
    double timetick;        // medida inicial de tiempo
    double *A;
    double *B;
    double *C;

    // chequeo de argumentos
    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) ){
        printf("\nError: hay que especificar N y T (tamaño de la matriz y cantidad de hilos)\n");
        exit(1);
    }

    // mas variables
    int ids[T];             // vector de identificadores (una dirección de memoria diferente para cada hilo)
    pthread_t threads[T];   // los hilos en cuestión
    pthread_params_t params[T];     // vector de parametros

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
        ids[i] = i;         // agregamos id al vector para luego pasarlo como argumento
        params[i].a = A;
        params[i].b = B;
        params[i].c = C;
        params[i].n = N;
        params[i].T = T;
        params[i].id = i;

        pthread_create(&threads[i], &attr, resolver, &params[i]);
    }

    // Esperamos a que termine cada hilo
    for (i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }

    // marcamos fin de tiempo
    printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",N,N, dwalltime() - timetick);

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
    int id = (*params).id;
    int N = (*params).n;
    int T = (*params).T;
    double* A = (*params).a;
    double* B = (*params).b;
    double* C = (*params).c;

    int x, y, z;                // indices para los bucles
    double suma, aux;                // suma parcial

    for (x=id*N/T; x<(id+1)*N/T; x++){
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
