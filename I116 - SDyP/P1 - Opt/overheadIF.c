#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h> 

double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int* inicializarVector(int* v,unsigned long N){
unsigned long i;
	for(i=0;i<N;i++){
		v[i]=rand()%1000;
	}
	
	return v;
}


int main(int argc, char *argv[]){
double timetick;
time_t t;
srand((unsigned) time(&t));
int* v;
unsigned long N=atol(argv[1]);
unsigned long numBytes = sizeof(int)*N;
int posicionSeleccionada = 20;
unsigned long i;
unsigned long menores1=0;
unsigned long menores2=0;
unsigned long menores3=0;

	v=(int*)malloc(numBytes);
	v=inicializarVector(v,N);	

	timetick = dwalltime();
	for(i=0;i<N;i++){
		if(i!=posicionSeleccionada){
			if(v[i]<v[posicionSeleccionada])
				menores1++;	
		}
	}
	printf("-> Tiempo Solucion 1: %f\n", dwalltime() - timetick);
	
	timetick = dwalltime();
	for(i=0;i<posicionSeleccionada;i++){
		if(v[i]<v[posicionSeleccionada])
			menores2++;	
	}
	
	for(i=posicionSeleccionada+1;i<N;i++){
		if(v[i]<v[posicionSeleccionada])
			menores2++;	
	}	
	printf("-> Tiempo Solucion 2: %f\n", dwalltime() - timetick);
	
	timetick = dwalltime();
	for(i=0;i<N;i++){
		if(v[i]<v[posicionSeleccionada])
			menores3++;	
	}
	printf("-> Tiempo Solucion 3: %f\n", dwalltime() - timetick);

	if ((menores1==menores2) && (menores2==menores3)){
		printf("OK\n");
	}else{
		printf("ERROR\n");
	}
	
	free(v);
	return 0;
}