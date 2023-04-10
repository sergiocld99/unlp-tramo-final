#include "hilos.h"

void create_and_join(void *(*start_routine)(void *), int T){
    pthread_t hilos[T];
    int ids[T];
    int i;

    for (i=0; i<T; i++){
        ids[i] = i;
        pthread_create(&hilos[i], NULL, start_routine, &ids[i]);
    }

    for (i=0; i<T; i++){
        pthread_join(hilos[i], NULL);
    }
}