#include<stdio.h>

// Librería P-threads
#include <pthread.h>

// libreria de tiempo (linux)
#include <unistd.h>

// cantidad de hilos
#define T 2

// prototipo de tarea
void tarea(void*);

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
    pthread_params_t params[T];

    // inicializar atributos
    pthread_attr_init(&attr);

    for (i = 0; i < T; h++){
        pthread_create(&threads[i], &attr, tarea, &params[i]);
    }
  
    // join
    for (i=0; i<T; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}


void tarea(void* params){
    int i;
    double act, ant;

    // se ejecutan periócamente en 1000 oportunidades
    for(i=0; i<1000; i++){
        ant = dwalltime();
        usleep(10000);
        act = dwalltime();
        printf("Latencia: %f\n", act - ant);
    } 
}