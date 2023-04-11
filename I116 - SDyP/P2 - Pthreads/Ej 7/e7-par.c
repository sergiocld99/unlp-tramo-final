#include "tiempo.h"
#include "hilos.h"
#include <stdio.h>	// para printf
#include <stdlib.h>	// para exit

// prototipos
void extraerParams(int argc, char* argv[]);
void* recorrer(void*);

// shared
int N, T;
int* V;
int pasoGlobal;
int* exitos;

int main(int argc, char* argv[]){
	int i, j, ok = 1;
	
	extraerParams(argc, argv);
	
	// a partir de N...
	V = (int*) malloc(N*sizeof(int));
	for (i=0; i<N; i++) V[i] = i % 4;
	
	// a partir de T...
	exitos = (int*) malloc(T*sizeof(int));

	double t0 = dwalltime();
	
	// encontrar paso
	i = 1;
	while ((i < N) && (V[i] != V[0] * 2)) i++;
	
	// printf("El paso global es %d\n", i);
	
	if (i < N){
		pasoGlobal = i;
		create_and_join(&recorrer, T);
	} else pasoGlobal = -1;
	
	// verificar que cumple duplicación en todas las porciones
	i = 0;
	while ((i < T) && (pasoGlobal != -1)){
		if (exitos[i++] == 0) pasoGlobal = -1;
	}
	
	double t1 = dwalltime();
	printf("N = %d, paso = %d, tiempo en segundos = %f \n", N, pasoGlobal, t1-t0);
	
	free(V);
	
	return 0;
}

void* recorrer(void* arg){
	int id = *(int*) arg;
	
	int ok = 1;
	int paso = pasoGlobal * T;
	int factor = 2*T;
	int j;
	int ant;
	
	// sabemos que pasoGlobal es positivo, sino no estaríamos acá
	ant = V[pasoGlobal];
	
	// verificar que se cumpla el paso	
	j = pasoGlobal * (2+id);
	
	while ((j < N) && ok){
		// printf("Hilo %d chequeando en pos %d\n", id, j);
		if (V[j] == ant * factor) ant = V[j];
		else ok = 0;
		
		j += paso;
	}
	
	exitos[id] = ok;

	pthread_exit(NULL);
}

void extraerParams(int argc, char* argv[]){
	if (argc < 3) exit(1);
	
	N = atoi(argv[1]);
	T = atoi(argv[2]);
	
	if (N < 2 || T < 1) exit(2);
}
