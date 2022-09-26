#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define COORDINATOR 0

int main(int argc, char* argv[]){
	int i, j, k, numProcs, rank, n, stripSize, check=1;
	double * a, * b, *c;
	MPI_Status status;
	double commTimes[4], maxCommTimes[4], minCommTimes[4], commTime, totalTime;

	/* Lee parámetros de la línea de comando */
	if ((argc != 2) || ((n = atoi(argv[1])) <= 0) ) {
	    printf("\nUsar: %s size \n  size: Dimension de la matriz y el vector\n", argv[0]);
		exit(1);
	}

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if (n % numProcs != 0) {
		printf("El tamaño de la matriz debe ser multiplo del numero de procesos.\n");
		exit(1);
	}

	// calcular porcion de cada worker
	stripSize = n / numProcs;

	// Reservar memoria
	if (rank == COORDINATOR) {
		a = (double*) malloc(sizeof(double)*n*n);
		c = (double*) malloc(sizeof(double)*n*n);
	}
	else  {
		a = (double*) malloc(sizeof(double)*n*stripSize);
		c = (double*) malloc(sizeof(double)*n*stripSize);
	}
	
	b = (double*) malloc(sizeof(double)*n*n);


	// inicializar datos
	if (rank == COORDINATOR) {
		for (i=0; i<n ; i++)
			for (j=0; j<n ; j++)
				a[i*n+j] = 1;
		for (i=0; i<n ; i++)
			for (j=0; j<n ; j++)
				b[i*n+j] = 1;

	}

	MPI_Barrier(MPI_COMM_WORLD);

	commTimes[0] = MPI_Wtime();

	/* distribuir datos*/
	if (rank==COORDINATOR){
	
		for (i=1; i<numProcs; i++) {
			MPI_Send(a+i*stripSize*n, stripSize*n, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(b, n*n, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(a, stripSize*n, MPI_DOUBLE, COORDINATOR, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(b, n*n, MPI_DOUBLE, COORDINATOR, 1, MPI_COMM_WORLD, &status);
	}

	commTimes[1] = MPI_Wtime();

	/* computar multiplicacion parcial */
	for (i=0; i<stripSize; i++) {
		for (j=0; j<n ;j++ ) {
			c[i*n+j]=0;
			for (k=0; k<n ;k++ ) { 
				c[i*n+j] += (a[i*n+k]*b[j*n+k]); 
			}
		}
	}
		

	commTimes[2] = MPI_Wtime();

	// recolectar resultados parciales
	if (rank==COORDINATOR){
		for (i=1; i<numProcs; i++) {
			MPI_Recv(c+i*stripSize*n, n*stripSize, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);			
		}
	} else
		MPI_Send(c, n*stripSize, MPI_DOUBLE, COORDINATOR, 2, MPI_COMM_WORLD);

	commTimes[3] = MPI_Wtime();

	MPI_Reduce(commTimes, minCommTimes, 4, MPI_DOUBLE, MPI_MIN, COORDINATOR, MPI_COMM_WORLD);
	MPI_Reduce(commTimes, maxCommTimes, 4, MPI_DOUBLE, MPI_MAX, COORDINATOR, MPI_COMM_WORLD);

	MPI_Finalize();

	if (rank==COORDINATOR) {

		// Check results
		for(i=0;i<n;i++)
			for(j=0;j<n;j++)
				check=check&&(c[i*n+j]==n);

		if(check){
			printf("Multiplicacion de matrices resultado correcto\n");
		}else{
			printf("Multiplicacion de matrices resultado erroneo\n");
		}
		
		totalTime = maxCommTimes[3] - minCommTimes[0];
		commTime = (maxCommTimes[1] - minCommTimes[0]) + (maxCommTimes[3] - minCommTimes[2]);		

		printf("Multiplicacion de matrices (N=%d)\tTiempo total=%lf\tTiempo comunicacion=%lf\n",n,totalTime,commTime);
	}
	
	free(a);
	free(b);
	free(c);

	return 0;
}