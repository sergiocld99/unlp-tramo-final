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

 double aux;

 #pragma omp parallel for private(aux) ordered
 for(i=1;i<=N;i++){
	// esto depende de la iteración anterior
	// x= x + sqrt(i*scale) + 2*x;

	// parte independiente
	aux = sqrt(i * scale);

	#pragma omp ordered 
	{
		x = aux + 3*x;
	}
 }

 printf("\n Resultado: %f \n",x);

 return(0);
}

