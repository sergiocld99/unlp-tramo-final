// Buscar promedio, minimo y máximo de un vector con semaforos

#include <stdio.h>
#include <stdlib.h>

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

// Prototipos de funcion
static void init_vector(double* v, int N);

int main(int argc,char*argv[]){

    // variables
    int N, i;
    double aux, timetick;

    // lectura de argumentos
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ){
        printf("\nError: hay que especificar N (tamaño del vector)\n");
        exit(1);
    }

    // reservar memoria
    v = (double*) malloc(sizeof(double)*N);

    // rellenar vector
    init_vector(v, N);

    // marcar inicio de tiempo
    timetick = dwalltime();

    // resolver
    for (i=0; i<N; i++){
        aux = v[i];
        if (aux > maximo) maximo = aux;
        if (aux < minimo) minimo = aux;
        sumaParcial += aux;
    }

    // marcamos fin de tiempo
    printf("N=%d, min: %f, max: %f, prom: %f. Tiempo en segundos %f\n", N, minimo, maximo, sumaParcial / N, dwalltime() - timetick);

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