#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
double dwalltime(){
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

int main(int argc, char* argv[]){
	
	unsigned long N = atol(argv[1]);
	unsigned long m = atol(argv[2]);
	
	unsigned long* a;
	unsigned long* resultModulo;
	unsigned long* resultOpt;
	
	a = (unsigned long*)malloc(sizeof(unsigned long)*N);
	resultModulo = (unsigned long*)malloc(sizeof(unsigned long)*N);
	resultOpt = (unsigned long*)malloc(sizeof(unsigned long)*N);
	unsigned long i;
		
	//Inicializa el vector a	
	for(i=0;i<N;i++){
		a[i]=i;
	}

	//Calculo usando %
	double timetick = dwalltime();
	for(i=0;i<N;i++){
		resultModulo[i] = i%m;
	}
	printf("Tiempo usando %%: %f \n", dwalltime() - timetick);
	 
	//Calculo usando la equivalencia
	timetick = dwalltime();
	for(i=0;i<N;i++){
		resultOpt[i] = i&(m-1);
	}
	printf("Tiempo usando equivalencia: %f \n", dwalltime() - timetick);
	
	//Chequeo de resultados
	for(i=0;i<N;i++){
		if(resultModulo[i]!=resultOpt[i]) printf("Error\n");
	}
	
	return 0;
}