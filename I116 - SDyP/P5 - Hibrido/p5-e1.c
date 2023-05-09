#include <stdio.h>
#include <stdlib.h>     // atoi
#include "tiempo.h"
#include <mpi.h>
#include <omp.h>

// NO EXISTEN VARIABLES COMPARTIDAS EN MPI
void getParams(int argc, char* argv[], int *N, int *T);
void p0(int N, int cp);
void p1(int N, int cp);

// Funciones comunes
void resolverOMP(double *A, double *B, double *C, int filas, int cols, int N);

// Funciones de verificacion
void saludar(int rank);

// programa principal
int main(int argc, char* argv[]){

    // siempre primero esto!!
    MPI_Init(&argc, &argv);

    // variables locales
    int id;
    int cantProcesos;
    int N, T;
    
    getParams(argc, argv, &N, &T);

    // obtener id y cant procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);

    if (N < 1 || T < 1 || N % T != 0) {
        printf("Formato: mpirun --bind-to none -np CP archivo N T\n");
        MPI_Finalize();
        return 1;
    }
    
    // establecer cantidad de hilos por rank
    omp_set_num_threads(T);
		saludar(id);

		// comenzar trabajo
    if (id == 0) p0(N, cantProcesos);
    else p1(N, cantProcesos);

    // siempre por ultimo esto!!
    MPI_Finalize();

    return 0;
}

// funcion que ejecuta el proceso con id=0
void p0(int N, int cp){
    double *A, *B, *C;
    int i,j;
    int filasRep = N / cp;
    int errores = 0;
    
    // reservamos memoria
    A = (double*) malloc(N*N*sizeof(double));
    B = (double*) malloc(N*N*sizeof(double));
    C = (double*) malloc(N*N*sizeof(double));
    
    // creamos las matrices
    for (i=0; i<N; i++){
    	for (j=0; j<N; j++){
    		A[i*N+j] = 1;
    		B[i+j*N] = 1;
    		C[i*N+j] = 0;
    	}
    }
    
    // printf("Tenemos %d procesos\n", cp);
    double t0 = dwalltime();
    
    // repartimos por conjuntos de filas
    MPI_Scatter(A, filasRep*N, MPI_DOUBLE, A, filasRep*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // resolver mi parte...
    resolverOMP(A,B,C,filasRep,N,N);
    
    // juntar los resultados de cada uno
    MPI_Gather(C, filasRep*N, MPI_DOUBLE, C, filasRep*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    double t1 = dwalltime();
   	
    // verificacion
    for (i=0; i<N; i++){
    	for (j=0; j<N; j++){
    		if (C[i*N+j] != N) errores++;
    	}
    }
    
    printf("Tiempo: %f segundos, %d errores\n", t1-t0, errores);
   	
    // liberamos memoria
    free(A);
		free(B);
		free(C);
}

void p1(int N, int cp){
		double *A, *B, *C;
		double *vacio;
    int i,j;
    int filasRep = N / cp;
    
    // reservamos memoria
    A = (double*) malloc(filasRep*N*sizeof(double));
    B = (double*) malloc(N*N*sizeof(double));
    C = (double*) malloc(filasRep*N*sizeof(double));
    
    // recibimos matrices operando (no quiero enviar nada)
    MPI_Scatter(vacio, 0, MPI_DOUBLE, A, filasRep*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // resolver mi parte...
    resolverOMP(A,B,C,filasRep,N,N);
    
    // enviar resultados al rank 0 (no voy a recibir nada)
    MPI_Gather(C, filasRep*N, MPI_DOUBLE, vacio, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

// funciones comunes
void resolverOMP(double *A, double *B, double *C, int filas, int cols, int N){
		int i,j,k;
	
		// paralelizacion por hilos
		// Se recorren pocas filas de A y todas las columnas de B
		
		#pragma omp parallel for private(i,j,k) shared(A,B,C,filas,cols,N) default(none)
		for (i=0; i<filas; i++){
			for (j=0; j<cols; j++){
				for (k=0; k<N; k++){
					// C[i,j] += A[i,k] * B[k,j]
					C[i*N+j] += A[i*N+k] * B[k+j*N];
				}
			}
		}
}

void saludar(int rank){
		#pragma omp parallel 
		{
			printf("Hola desde rank %d, hilo %d\n", rank, omp_get_thread_num());
		}
}

// control de argumentos
void getParams(int argc, char* argv[], int *N, int *T){
    
    if (argc < 3){
    	*N = 0;
    	*T = 0;
    } else {
    	*N = atoi(argv[1]);
    	*T = atoi(argv[2]);
    }
}
