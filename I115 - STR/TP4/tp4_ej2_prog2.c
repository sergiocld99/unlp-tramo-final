
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Para manejo de hilos
#include <pthread.h>
#include <sched.h>		// para cambiar prioridad
#define T 		2
#define PRIORIDAD_ALTA	10
#define PRIORIDAD_BAJA	5

// Semáforo
#include <semaphore.h>

// Para nanosleep
#include <time.h>

// Uso de pipes
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MYFIFO "/tmp/myfifo"


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
  struct sched_param param;
  int i;

  // inicializar atributos de hilos
  pthread_attr_init(&attr);

  // inicializar semaforo en cero
  sem_init(&semaforo, 0, 0);

  // crear hilo de mayor prioridad
  pthread_attr_getschedparam(&attr, &param);
  param.sched_priority = PRIORIDAD_ALTA;
  pthread_attr_setschedparam(&attr, &param);
  pthread_create(&threads[0], &attr, tarea1, NULL);

  // crear hilo de menor prioridad
  pthread_attr_getschedparam(&attr, &param);
  param.sched_priority = PRIORIDAD_BAJA;
  pthread_attr_setschedparam(&attr, &param);
  pthread_create(&threads[1], &attr, tarea2, NULL);

  // esperar a que finalicen
  for (i=0; i<T; i++) pthread_join(threads[i], NULL);
  
  return 0; 
}

void* tarea1(void* args){
  int fd, valor, nBytesLeidos;  

  while(1){
    // abrir pipe para escritura
    // printf("Abriendo pipe para lectura...\n");
    fd = open(MYFIFO, O_RDONLY);  

    // leer pipe
    nBytesLeidos = read(fd, &valor, sizeof(int));
    // printf("%d bytes leidos, valor %d\n", nBytesLeidos, valor);

    // cerrar pipe
    close(fd);

    // si se leyeron datos...
    if (nBytesLeidos > 0){

	// controlar que no exceda 90°C
    	if (valor > 90) 
      	  printf("Atencion: la temperatura excede los 90°C \n");
    	else { 
      	  temp = valor;
      	  sem_post(&semaforo);
	}
    }
  }
}


void* tarea2(void* args){
  int actual, valores[3];
  int i, recibidos = 0, suma = 0;

  while(1){
    // esperar por nuevo dato
    sem_wait(&semaforo);
    actual = temp;

    // leerlo y almacenarlo
    if (recibidos < 3) recibidos++;
    
    valores[2] = valores[1];
    valores[1] = valores[0];
    valores[0] = actual;

    // imprimir promedio
    for (i=0; i<recibidos; i++) suma += valores[i];
    printf("Promedio: %f\n", (double) suma / recibidos);

    // reiniciar suma
    suma = 0;
  }
}
