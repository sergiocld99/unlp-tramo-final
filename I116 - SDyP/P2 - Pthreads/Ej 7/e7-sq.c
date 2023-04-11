#include "tiempo.h"
#include <stdio.h>	// para printf
#include <stdlib.h>	// para exit

// prototipos
void extraerParams(int argc, char* argv[]);

// shared
int N;
int* V;

int main(int argc, char* argv[]){
	int i, j;
	
	extraerParams(argc, argv);
	
	V = (int*) malloc(N*sizeof(int));
	for (i=0; i<N; i++) V[i] = i % 4;
	
	double t0 = dwalltime();
	
	// operacion a medir
	int ant = V[0], paso;
	int ok = 1;
	i = 1;
	
	// encontrar paso
	// precondicion: sabemos que es creciente
	while ((i < N) && (V[i] != V[0] * 2)) i++;
	paso = i;
	
	if (i<N) ant = V[i];
	else ok = 0;
	
	// verificar que se cumpla el paso	
	j = i + paso;
	
	while ((j < N) && ok){
		if (V[j] == ant * 2) ant = V[j];
		else ok = 0;
		
		j += paso;
	}
	
	if (!ok) paso = -1;
	
	double t1 = dwalltime();
	printf("N = %d, paso = %d, tiempo en segundos = %f \n", N, paso, t1-t0);
	
	free(V);
	
	return 0;
}

void extraerParams(int argc, char* argv[]){
	if (argc < 2) exit(1);
	
	N = atoi(argv[1]);
	if (N < 2) exit(2);
}
