
#include <stdio.h>

// Librería P-threads
#include <pthread.h>

// libreria de tiempo (linux)
#include <unistd.h>

// cantidad de hilos
#define T 2
#define ITERACIONES 1000

// prototipo de tarea
void* tarea(void*);

/*
Para realizar el sensor de temperatura, realice un programa que lea de un archivo
el tiempo (nanosegundos) que debe esperar para entregar un dato y la
temperatura (°C) que debe entregar. Dicho archivo debe tener por cada fila dos
números (el tiempo y la temperatura) separados por un <tab>. Una vez cumplido
el plazo leído del archivo el programa debe escribir en un pipe la temperatura y
esperar el cumplimiento del siguiente plazo para volver a escribir. Debe continuar
con esta mecánica hasta la finalización del archivo. 
*/

int main(int argc, char *argv[])
{
  // variables del programa
  double tiempo, temp;
  FILE *arch;
  
  // abrir archivo
	arch = fopen ( "fichero.in", "r" );        
	
  if (arch == NULL) {
    fputs("Error al abrir el archivo", stderr); 
    exit(1);
  }
  
  // repetir mientras no se acabe el archivo...
  while (!feof(arch)){
    // leer linea: tiempo \t temperatura
    // leer tiempo en ns que debo esperar para entregar un dato
    // leer la temperatura (°C) que debo entregar
    fscanf(arch, "%f\t%f\n", &tiempo, &temp);
  
    // esperar que se cumpla el plazo
    // IBM: the nsleep subroutine suspends the current process in nanoseconds
    nsleep(tiempo);
  
    // escribir temperatura
    printf("%f\n", temp);
  }
  
  // cerrar archivo
  fclose(arch);
  
  return 0; 
}
