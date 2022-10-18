// IMPLEMENTACIÓN PARALELA

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define COORDINATOR 0

// ------------------------------------------------

// N se recibe por parámetro
int main(int argc, char* argv[]){
    int i, numProcs, rank, n, stripSize, check=1;
	double *v, *res, min = 9999.9, max = -9999.9, acum = 0.0, prom;
	MPI_Status status;
	double commTimes[5], commTime, totalTime;

	/* Leer argumentos */
	if ((argc != 2) || ((n = atoi(argv[1])) <= 0) ) {
	    printf("\nUsar: %s size \n  size: Dimension de la matriz y el vector\n", argv[0]);
		exit(1);
	}

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // chequear que N sea multiplo de numProcs
    if (n % numProcs != 0) {
		printf("N debe ser multiplo del numero de procesos.\n");
		exit(1);
	}

    // calcular porcion de cada worker
	stripSize = n / numProcs;

    if (rank == COORDINATOR){
        v = (double*) malloc(sizeof(double)*n);
        res = (double*) malloc(sizeof(double)*3*numProcs);
        for (i=0; i<n; i++) v[i] = i;
    } else {
        v = (double*) malloc(sizeof(double)*stripSize);
        res = (double*) malloc(sizeof(double)*3);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // REPARTIR (de master a todos)
    if (rank == COORDINATOR) commTimes[0] = MPI_Wtime();
    MPI_Scatter(v, stripSize, MPI_DOUBLE, v, stripSize, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
    if (rank == COORDINATOR) commTimes[1] = MPI_Wtime();

    // buscar valores
    res[0] = 9999.9;    // min
    res[1] = -9999.9;   // max
    res[2] = 0.0;       // acumulado

    for (i=0; i<stripSize; i++){
        if (v[i] < res[0]) res[0] = v[i];
        if (v[i] > res[1]) res[1] = v[i];
        res[2] += v[i];
    }

    // RECOLECTAR
    if (rank == COORDINATOR) commTimes[2] = MPI_Wtime();
    MPI_Gather(res, 3, MPI_DOUBLE, res, 3, MPI_DOUBLE, COORDINATOR, MPI_COMM_WORLD);
    if (rank == COORDINATOR) commTimes[3] = MPI_Wtime();

    // buscar minimos, maximos y promedio con datos reducidos -> paralelizable?
    if (rank == COORDINATOR){
        
        for(i=0; i<3*numProcs; i+=3){
            if (res[i] < min) min = res[i];
        }

        for (i=1; i<3*numProcs; i+=3){
            if (res[i] > max) max = res[i];
        }

        for (i=2; i<3*numProcs; i+=3){
            acum += res[i];
        }

        prom = acum / (3*numProcs);
        commTimes[4] = MPI_Wtime();
    }

    MPI_Finalize();

    // muestra de resultados
    if (rank == COORDINATOR){
        if (min == 0) printf("min correcto \n");
        if (max == n-1) printf("max correcto \n");

        // total time
        totalTime = commTimes[4] - commTimes[0];
        commTime = (commTimes[1] - commTimes[0]) + (commTimes[3] - commTimes[2]);

        printf("Multiplicacion (N=%d)\tTiempo total=%lf\tTiempo comm=%lf\n",n,totalTime,commTime);
    }

    free(v);
    free(res);
    return 0;
}