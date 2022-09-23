//Ejercicio 2
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>

int main(int argc,char*argv[]){
 double x,scale;
 int i;
 int numThreads = atoi(argv[2]);
 int N=atoi(argv[1]);
 omp_set_num_threads(numThreads);
 scale=2.78;
 x=0.0;

 // PROBLEMA: x siempre da distinto.
 // CAUSA: las iteraciones NO son independientes entre sí
 // OBSERVACIÓN: sqrt(i*scale) es independiente
 // SOLUCIÓN: DIRECTIVA ORDERED para asignar x

 double aux;

 // double timetick = dwalltime();

 #pragma omp parallel for private(aux) ordered
 for(i=1;i<=N;i++){
	aux = sqrt(i*scale);

	#pragma omp ordered
	{
		x= 3*x + aux;
	}

 }

 // el tiempo de ejecucion es muy chico para x grande
 // printf("N=%d, T=%d, t[s] = %f\n", N, numThreads, dwalltime() - timetick);

 printf("\n Resultado: %f \n",x);

 return(0);
}

