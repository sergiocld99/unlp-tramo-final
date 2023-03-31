//Dados dos arrays (U y V) de vectores en R^3. Para cada posición se realiza el producto vectorial uno a uno, es decir para una posicion i, W[i] = U[i] X V[i]  
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

/**********Para calcular tiempo*************************************/
double dwalltime()
{
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}
/****************************************************************/

typedef struct rvector {
  double i;
  double j;
  double k;
} Tvector;

void inicializarArrays(Tvector* U, Tvector* V, unsigned long N){
 unsigned long p;
 
	for(p=0; p<N; p++){
		U[p].i = 2.0;
		U[p].j = 0.0;
		U[p].k = 1.0;

		V[p].i = 1.0;
		V[p].j = -1.0;
		V[p].k = 3.0;		
	}
} 

Tvector* productoVectorialArray(Tvector* U, Tvector* V, Tvector* W, unsigned long N){
 unsigned long p;
 
	for(p=0; p<N; p++){
		W[p].i = U[p].j * V[p].k - U[p].k * V[p].j;
		W[p].j = U[p].k * V[p].i - U[p].i * V[p].k;
		W[p].k = U[p].i * V[p].j - U[p].j * V[p].i;
	}

 return W;
} 

void verificarResultado(Tvector* A, unsigned long N){
 unsigned long p;
 unsigned long errores = 0;
 
	for(p=0; p<N; p++){
		if( (A[p].i != 1.0) || (A[p].j != -5.0) || (A[p].k != -2.0) ){
			printf("Error en la posición %lu.\n",p);
			errores++;
		} 
	}	
	
	if(errores == 0)
		printf("Resultado Correcto\n");
	else	
		printf("Errores %lu\n",errores);
}

int main(int argc, char* argv[]){
double time_init, time_end;

	if(argc<2){
		printf("Falta un parámetro de la dimensión de los arreglos.\n");
		printf("Ejecutar ./programa N\n");
		return -1;
	}	

 unsigned long N = atol(argv[1]);
 Tvector* U;
 Tvector* V;
 Tvector* W;
 
	U=(Tvector*)malloc(sizeof(Tvector)*N); 
	V=(Tvector*)malloc(sizeof(Tvector)*N);
	W=(Tvector*)malloc(sizeof(Tvector)*N);

	inicializarArrays(U,V,N);

	time_init = dwalltime();
	W = productoVectorialArray(U,V,W,N);
	time_end = dwalltime();
	printf("Tiempo en segundos usando registros %.10lf\n",time_end - time_init);

	verificarResultado(W,N);

	free(U);
	free(V);
	free(W);

	return 0;
}
