
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Para nanosleep
#include <time.h>

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
  int tiempo, temp;
  int p[2];		// pipe
  char* msg;
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
    fscanf(arch, "%d\t%d\n", &tiempo, &temp);
  
    // esperar que se cumpla el plazo
    nanosleep((const struct timespec[]){{0, tiempo}}, NULL);
  
    // escribir temperatura por pipe (usar siempre p[1])
    write(p[1], &temp, sizeof(temp));
  }
  
  // cerrar archivo
  fclose(arch);

  while(1);
  
  return 0; 
}
