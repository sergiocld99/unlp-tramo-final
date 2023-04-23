#include <stdio.h>
#include <stdlib.h>     // atoi
#include <mpi.h>

// NO EXISTEN VARIABLES COMPARTIDAS EN MPI
void p0();

// programa principal
int main(int argc, char* argv[]){

    // siempre primero esto!!
    MPI_Init(&argc, &argv);

    // variables locales
    int id;
    int cantProcesos;

    // obtener id y cant procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    if (id == 0) p0();

    // siempre por ultimo esto!!
    MPI_Finalize();

    return 0;
}

// funcion que ejecuta el proceso con id=0
void p0(){
    printf("Hola mundo!\n");
}