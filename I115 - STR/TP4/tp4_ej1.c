#include<stdio.h>

// Librería P-threads
#include <pthread.h>

// libreria de tiempo (linux)
#include <unistd.h>

// cantidad de hilos
#define T 2
#define ITERACIONES 1000

// prototipo de tarea
void* tarea(void*);

/*****************************************************************/

#include <stdio.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

/*****************************************************************/

int main(int argc, char *argv[])
{
    int i;

    // pthread
    pthread_attr_t attr;
    pthread_t threads[T];

    // inicializar atributos
    pthread_attr_init(&attr);

    for (i = 0; i < T; i++){
        pthread_create(&threads[i], &attr, tarea, NULL);
    }
  
    // join
    for (i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}


void* tarea(void* params){
    int i;
    double act, ant, acum = 0.0;

    // se ejecutan periócamente en 1000 oportunidades
    for(i=0; i<ITERACIONES; i++){
        ant = dwalltime();
        usleep(10000);        // dormir por 10000 us = 10 ms
        act = dwalltime();
        
        // Medir lo que tarda en retomar menos la centesima
        // de segundo que debía transcurrir (0.01 s = 10 ms)
	acum += (act - ant - 0.01);
    } 

    // imprimir resultados
    double prom = acum / ITERACIONES;
    printf("Latencia prom: %f s = %f us\n", prom, prom * 1000000);   
}
