// Cabeceras
#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/time.h>   // dwalltime

// Constantes
#define SIN_ORDEN 0
#define ORDEN_CRECIENTE 1
#define ORDEN_DECRECIENTE 2

// Prototipos de funcion
void extraerParams(int argc, char* argv[]);
void inicializarVector(int orden);
double dwalltime();

int verificarSinOrden();
int verificarCreciente();
int verificarDecreciente();

// Variables compartidas
int N;
double* V;

// Programa principal
int main(int argc, char* argv[]){
    int orden = SIN_ORDEN;
    int i = 1;
    
    extraerParams(argc, argv);

    // alocar memoria
    V = (double*) malloc(N * sizeof(double));
    inicializarVector(ORDEN);

    // operacion a medir
    double t0 = dwalltime();

    // determinar orden a partir de los primeros elementos
    while (orden == SIN_ORDEN && i < N){
        if (V[i] > V[i-1]) orden = ORDEN_CRECIENTE;
        else if (V[i] < V[i-1]) orden = ORDEN_DECRECIENTE;

        i++;
    }

    // chequear que los siguientes elementos respeten dicho orden
    if (orden == ORDEN_CRECIENTE){
        while (orden == ORDEN_CRECIENTE && i < N){
            orden = (V[i] >= V[i-1]) ? orden : SIN_ORDEN;
            i++;
        }
    } else if (orden == ORDEN_DECRECIENTE){
        while (orden == ORDEN_DECRECIENTE && i < N){
            orden = (V[i] <= V[i-1]) ? orden : SIN_ORDEN;
            i++;
        }
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

    free(V);

    return 0;
}

// Implemento funciones
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
    if (argc < 2) {
        printf("Especificar N\n");
        exit(1);
    }

    N = atoi(argv[1]);

    if (N < 2) {
        printf("N debe ser mayor o igual a 2\n");
        exit(2);
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