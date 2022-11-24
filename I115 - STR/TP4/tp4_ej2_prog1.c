
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Para nanosleep
#include <time.h>

// Uso de pipes
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MYFIFO "/tmp/myfifo"


int main(int argc, char *argv[])
{
  // variables del programa
  int tiempo, temp, status, fd;
  char* msg;
  FILE *arch;
  
  // abrir archivo
  arch = fopen ( "fichero.in", "r" );        
	
  if (arch == NULL) {
    fputs("Error al abrir el archivo", stderr); 
    exit(1);
  }

  // generar pipe
  status = mkfifo(MYFIFO, 0666);
  // if (status != 0) printf("ERROR: mkfifo returned %d\n", status);


  // repetir mientras no se acabe el archivo...
  while (!feof(arch)){
    // leer linea: tiempo \t temperatura
    // leer tiempo en ns que debo esperar para entregar un dato
    // leer la temperatura (°C) que debo entregar
    fscanf(arch, "%d\t%d\n", &tiempo, &temp);
  
    // esperar que se cumpla el plazo
    nanosleep((const struct timespec[]){{0, tiempo}}, NULL);
  
    // abrir pipe para escritura
    // printf("Abriendo pipe, esperando al otro programa...\n");
    fd = open(MYFIFO, O_WRONLY);  

    // escribir temperatura por pipe
    // printf("Temp: %d, cerrando pipe...\n", temp);
    write(fd, &temp, sizeof(int));
    close(fd);
  }
  
  // cerrar archivo
  fclose(arch);

  while(1);
  
  return 0; 
}
