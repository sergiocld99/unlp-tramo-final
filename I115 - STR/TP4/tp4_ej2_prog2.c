
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Para manejo de hilos
#include <pthread.h>
#define T 2

// Semáforo
#include <semaphore.h>

// Para nanosleep
#include <time.h>

// Prototipos de tareas
void* tarea1(void*);
void* tarea2(void*);

// Variables globales
sem_t semaforo;
int temp;

int main(int argc, char *argv[])
{
  // variables del programa
  pthread_attr_t attr;
  pthread_t threads[T];
  int i;

  // inicializar atributos de hilos
  pthread_attr_init(&attr);

  // inicializar semaforo en cero
  sem_init(&semaforo, 0, 0);

  // crear hilos
  pthread_create(&threads[0], &attr, tarea1, NULL);
  pthread_create(&threads[1], &attr, tarea2, NULL);

  // esperar a que finalicen
  for (i=0; i<T; i++) pthread_join(threads[i], NULL);
  
  return 0; 
}

void* tarea1(void* args){
  int p[2], valor, nbytes;  

  while(1){
    // leer pipe
    nbytes = read(p[0], &valor, sizeof(valor));

    // controlar que no exceda 90°C
    if (valor > 90) 
      printf("Atencion: la temperatura excede los 90°C \n");
    else 
      temp = valor;
      sem_post(&semaforo);
  }
}

void* tarea2(void* args){
  int valores[3];
  int i, recibidos = 0, suma = 0;

  while(1){
    // esperar por nuevo dato
    sem_wait(&semaforo);

    // leerlo y almacenarlo
    if (recibidos < 3) recibidos++;
    
    valores[2] = valores[1];
    valores[1] = valores[0];
    valores[0] = temp;

    // imprimir promedio
    for (i=0; i<recibidos; i++) suma += valores[i];
    printf("Promedio: %f\n", (double) suma / recibidos);

    // reiniciar suma
    suma = 0;
  }
}
