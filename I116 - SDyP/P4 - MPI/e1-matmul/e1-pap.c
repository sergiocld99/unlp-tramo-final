#include <stdio.h>
#include <stdlib.h>     // atoi
#include <mpi.h>

// NO EXISTEN VARIABLES COMPARTIDAS EN MPI
int getMatrixSize(int argc, char* argv[]);
void p0(int N);

// programa principal
int main(int argc, char* argv[]){

    // siempre primero esto!!
    MPI_Init(&argc, &argv);

    // variables locales
    int id;
    int cantProcesos;
    int N = getMatrixSize(argc, argv);

    // obtener id y cant procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    if (id == 0) p0(N);

    // siempre por ultimo esto!!
    MPI_Finalize();

    return 0;
}

// control de argumentos
int getMatrixSize(int argc, char* argv[]){
    if (argc < 2) return -1;
    return atoi(argv[1]);
}

// funcion que ejecuta el proceso con id=0
void p0(int N){
    if (N < 1) printf("ERROR: Hay que pasar N positivo\n");
    else printf("Hola mundo!\n");
}
