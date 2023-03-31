//Dados dos arrays (U y V) de vectores en R^3. Para cada posición se realiza el producto vectorial uno a uno, es decir para una posicion i, W[i] = U[i] X V[i]
//Sin embargo, cada array estan separado en 3 array, uno por cada componente del vector.   
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

void inicializarArrays(double* Ui, double* Uj, double* Uk, double* Vi, double* Vj, double* Vk, unsigned long N){
 unsigned long p;

	for(p=0; p<N; p++){
		Ui[p] = 2.0;
		Uj[p] = 0.0;
		Uk[p] = 1.0;
		
		Vi[p] = 1.0;
		Vj[p] = -1.0;
		Vk[p] = 3.0;		
	}

} 

void productoVectorialArrays(double* Ui, double* Uj, double* Uk, double* Vi, double* Vj, double* Vk, double* Wi, double* Wj, double* Wk, unsigned long N){
 unsigned long p;
 
	for(p=0; p<N; p++){
		Wi[p] = Uj[p] * Vk[p] - Uk[p] * Vj[p];
		Wj[p] = Uk[p] * Vi[p] - Ui[p] * Vk[p];
		Wk[p] = Ui[p] * Vj[p] - Uj[p] * Vi[p];
	}

} 

void verificarResultado(double* Ai, double* Aj, double* Ak, unsigned long N){
 unsigned long p;
 unsigned long errores = 0;
 
	for(p=0; p<N; p++){
		if( (Ai[p] != 1.0) || (Aj[p] != -5.0) || (Ak[p] != -2.0) ){
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

 double* Ui;
 double* Vi;
 double* Wi;
 double* Uj;
 double* Vj;
 double* Wj;
 double* Uk;
 double* Vk;
 double* Wk;

	Ui=(double*)malloc(sizeof(double)*N); 
	Vi=(double*)malloc(sizeof(double)*N);
	Wi=(double*)malloc(sizeof(double)*N);

	Uj=(double*)malloc(sizeof(double)*N); 
	Vj=(double*)malloc(sizeof(double)*N);
	Wj=(double*)malloc(sizeof(double)*N);

	Uk=(double*)malloc(sizeof(double)*N); 
	Vk=(double*)malloc(sizeof(double)*N);
	Wk=(double*)malloc(sizeof(double)*N);
		
	inicializarArrays(Ui,Uj,Uk,Vi,Vj,Vk,N);

	time_init = dwalltime();	
	productoVectorialArrays(Ui,Uj,Uk,Vi,Vj,Vk,Wi,Wj,Wk,N);
	time_end = dwalltime();
	printf("Tiempo en segundos sin usar registros %.10lf\n",time_end - time_init);
	

	verificarResultado(Wi,Wj,Wk,N);

	free(Ui);
	free(Vi);
	free(Wi);

	free(Uj);
	free(Vj);
	free(Wj);

	free(Uk);
	free(Vk);
	free(Wk);
 
	return 0;
}
