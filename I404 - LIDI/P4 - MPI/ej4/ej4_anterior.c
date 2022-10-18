#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define COORDINATOR 0

// ------------------------------------------------------------------------------

int main(int argc, char* argv[]){
	int i, j, k, numProcs, rank, n, stripSize, check=1;
	double * a, * b, *c;
	MPI_Status status;
	double commTimes[4], commTime, totalTime;

	/* Lee par�metros de la l�nea de comando */
	if ((argc != 2) || ((n = atoi(argv[1])) <= 0) ) {
	    printf("\nUsar: %s size \n  size: Dimension de la matriz y el vector\n", argv[0]);
		exit(1);
	}

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if (n % numProcs != 0) {
		printf("El tama�o de la matriz debe ser multiplo del numero de procesos.\n");
		exit(1);
	}

	// calcular porcion de cada worker
	stripSize = n / numProcs;

	// Reservar memoria
	// el master maneja matrices de NxN, mientras los workers de NxM (M = porción de N)
	if (rank == COORDINATOR) {
		a = (double*) malloc(sizeof(double)*n*n);
		c = (double*) malloc(sizeof(double)*n*n);
	}
	else  {
		a = (double*) malloc(sizeof(double)*n*stripSize);
		c = (double*) malloc(sizeof(double)*n*stripSize);
	}
	
	// B es de NxN para todos los procesos
	b = (double*) malloc(sizeof(double)*n*n);


	// inicializar datos (solo lo realiza el proceso master)
	if (rank == COORDINATOR) {
		for (i=0; i<n ; i++)
			for (j=0; j<n ; j++)
				a[i*n+j] = 1;			// por filas: a[i][j] = a[i*n+j]
		for (i=0; i<n ; i++)
			for (j=0; j<n ; j++)
				b[i*n+j] = 1;			// también por filas

	}

	// No permito que los workers avancen hasta que el master inicialice A y B
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == COORDINATOR) commTimes[0] = MPI_Wtime();
	MPI_Scatter(a, stripSize*n, MPI_DOUBLE, a, stripSize*n, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
	MPI_Bcast(b, n*n, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
	if (rank == COORDINATOR) commTimes[1] = MPI_Wtime();

	/* computar multiplicacion parcial */
	
	for (i=0; i<stripSize; i++) {
		for (j=0; j<n; j++) {
			c[i*n+j]=0;					
			for (k=0; k<n; k++) { 
				c[i*n+j] += (a[i*n+k]*b[j*n+k]); 
			}
		}
	}
	
	if (rank == COORDINATOR) commTimes[2] = MPI_Wtime();
	MPI_Gather(c, n*stripSize, MPI_DOUBLE, c, n*stripSize, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == COORDINATOR) commTimes[3] = MPI_Wtime();

	MPI_Finalize();

	if (rank==COORDINATOR) {

		// Check results
		for(i=0;i<n;i++)
			for(j=0;j<n;j++)
				check=check&&(c[i*n+j]==n);

		if(check) printf("Multiplicacion de matrices resultado correcto\n");
		else printf("Multiplicacion de matrices resultado erroneo\n");
		
		totalTime = commTimes[3] - commTimes[0];
		commTime = (commTimes[1] - commTimes[0]) + (commTimes[3] - commTimes[2]);		

		printf("Multiplicacion de matrices (N=%d)\tTiempo total=%lf\tTiempo comunicacion=%lf\n",n,totalTime,commTime);
	}
	
	free(a);
	free(b);
	free(c);

	return 0;
}