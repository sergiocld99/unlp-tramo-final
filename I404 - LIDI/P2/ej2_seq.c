// Dado un vector de N elementos, contar las ocurrencias de un numero dado

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

// variables globales
static int *v;
static const int X = 10;
static int ocurrencias = 0;

// prototipos de función
static void init_vector(int* v, int N, int max);

int main(int argc,char*argv[]){

    // variables
    int N, i;
    double timetick;

    // lectura de argumentos
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0) ){
        printf("\nError: hay que especificar N (tamaño del vector)\n");
        exit(1);
    }

    // reservar memoria
    v = (int*) malloc(sizeof(int)*N);

    // rellenar vector
    init_vector(v, N, 2*X);

    // marcar inicio de tiempo
    timetick = dwalltime();

    // resolver
    for (i=0; i<N; i++){
        if (v[i] == X) ocurrencias++;
    }

    // marcamos fin de tiempo
    printf("Ocurrrencias de x=%d en vector: %d. Tiempo en segundos %f\n",X,ocurrencias, dwalltime() - timetick);

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
        v[i] = i % max;
    }
}