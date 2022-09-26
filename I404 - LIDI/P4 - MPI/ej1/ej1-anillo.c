/*
** Sending simple, point-to-point messages.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0

int main(int argc, char* argv[])
{
  int myrank;
  int size;
  int dest;              /* destination rank for message */
  int source;            /* source rank of a message */
  int tag = 0;           /* scope for adding extra information to a message */
  MPI_Status status;     /* struct used by MPI_Recv */
  char message[BUFSIZ];

  /* MPI_Init returns once it has started up processes */
  MPI_Init( &argc, &argv );

  /* size and rank will become ubiquitous */ 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  // crear mensaje y guardarlo en la var message
  sprintf(message, "Hola Mundo! Soy el proceso %d!", myrank);

  // send to the next process: dato, tamaño, tipo, destino, tag, comunicador
  dest = (myrank + 1) % size;
  MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

  // recibir mensaje del process anterior
  source = (myrank - 1 + size) % size;
  MPI_Recv(message, BUFSIZ, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
  printf("Mensaje recibido por proceso %d: %s\n", myrank, message);


  /* don't forget to tidy up when we're done */
  MPI_Finalize();

  /* and exit the program */
  return EXIT_SUCCESS;
}