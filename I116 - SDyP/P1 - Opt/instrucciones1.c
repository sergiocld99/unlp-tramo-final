#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

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


double randfrom(double min, double max) {
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}


int main(int argc, char *argv[]){
 double* x;
 double* y;
 double* resultado;
 double timetick, timeend;
 unsigned long i;
 unsigned long r;
 unsigned long N = atol(argv[1]);
 unsigned long R = atol(argv[2]);
 
 x=(double*)malloc(sizeof(double)*N);
 y=(double*)malloc(sizeof(double)*N);
 resultado=(double*)malloc(sizeof(double)*N);

 srand (time (NULL));
 
 //Inicializa los vectores
 for(i=0;i<N;i++){
	x[i] = 2*(1 + randfrom(0,39916801));
	y[i] = randfrom(1,719);
 }

 //Este for es para eliminar el cool state y limpiar cache
 printf("Corriendo código de prueba para calentar el hardware...");
 for(i=0;i<N;i++){
	resultado[i] = x[i]+y[i];
 }
 printf("OK\n");

//EMPIEZAN LOS CALCULOS ACA!!! 
 printf("Ejecutando operaciones\n");
  //Suma
  printf("Suma...\n");
  timetick = dwalltime();
   for(r=0;r<R;r++){
	for(i=0;i<N;i++){
		resultado[i] = x[i]+y[i];
	}
   }		
  timeend = dwalltime();
  printf(" Tiempo total en segundos %.10lf \n", (timeend - timetick));
  printf(" Tiempo promedio en segundos %.10lf \n", (timeend - timetick)/N*R);
  
  //Resta
  printf("Resta...\n");
  timetick = dwalltime();
   for(r=0;r<R;r++){
	for(i=0;i<N;i++){
		resultado[i] = x[i]-y[i];
	}
   } 	
  timeend = dwalltime();
  printf(" Tiempo total en segundos %.10lf \n", (timeend - timetick));
  printf(" Tiempo promedio en segundos %.10lf \n", (timeend - timetick)/N*R);
  
  //Producto
  printf("Producto...\n");
  timetick = dwalltime();
  for(r=0;r<R;r++){ 
	for(i=0;i<N;i++){
		resultado[i] = x[i]*y[i];
	}
  }	
  timeend = dwalltime();
  printf(" Tiempo total en segundos %.10lf \n", (timeend - timetick));
  printf(" Tiempo promedio en segundos %.10lf \n", (timeend - timetick)/N*R);
  
  //Division
  printf("Division...\n");
  timetick = dwalltime();
  for(r=0;r<R;r++){
	for(i=0;i<N;i++){
		resultado[i] = x[i]/y[i];
	}
  }	
  timeend = dwalltime();
  printf(" Tiempo total en segundos %.10lf \n", (timeend - timetick));
  printf(" Tiempo promedio en segundos %.10lf \n", (timeend - timetick)/N*R);

  free(x);
  free(y);
  free(resultado);
  return(0);

}
