#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<sys/time.h>
#include<time.h>
#include <math.h>

#ifdef DOUBLE
typedef double datatype;
#else
typedef float datatype;	
#endif	


//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

//Calcula el numero de fibonacci para tipos de datos no enteros utilizando el numero aureo
datatype fib_aureo(datatype x){
#ifdef DOUBLE
double sqrt5 = sqrt(5);
#else
float sqrt5 = sqrtf(5);
#endif	

datatype aureo = ((1 + sqrt5)/2);

#ifdef DOUBLE
return ( (pow(aureo,x) - pow(1-aureo,x)) / sqrt5 );
#else
return ( (powf(aureo,x) - powf(1-aureo,x)) / sqrt5 );
#endif	

}

//Calcula el numero de fibonacci para tipos de datos enteros de forma iterativa
int fib_int(int n){
int j = 0;
int i = 1;
int k;
int t;

	for(k=1;k<=n;k++){
		t = i + j;
		i = j;
		j = t;
	}
 	return j;	
}

int main(int argc,char*argv[]){	
double timetick;
double endtime;

 //Controla los argumentos al programa
 if (argc != 2)
  {
    printf("\nUsar: %s N\n  N: Dimension del vector\n", argv[0]);
    exit(1);
  }

time_t t;
srand((unsigned) time(&t)); 

#ifdef DOUBLE
printf("Tipo de datos DOUBLE\n");
#else
printf("Tipo de datos FLOAT\n");	
#endif	

 unsigned long N = atol(argv[1]); 
 datatype *v;
 datatype *resultado_aureo;
 int *resultado_int;
 v=(datatype*)malloc(sizeof(datatype)*N);
 resultado_aureo=(datatype*)malloc(sizeof(datatype)*N);
 resultado_int=(int*)malloc(sizeof(int)*N);
 unsigned long r;

 //Inicializa el vector con el numero 20. Fibonacci de 20 es 6765
 for(r=0;r<N;r++){
		v[r]=rand()%45;	
 }
 
 //Calcula fibonacci con el numero aureo para cada elemento del vector
 timetick = dwalltime();
 for(r=0;r<N;r++){
		resultado_aureo[r] = fib_aureo(v[r]);	
 }
 endtime = dwalltime() - timetick;

#ifdef DOUBLE
printf("Tiempo DOUBLE: %.2f segundos\n",endtime);
#else
printf("Tiempo FLOAT: %.2f segundos\n",endtime);
#endif

 //Calcula fibonacci tradicional para datos enteros
 timetick = dwalltime();
 for(r=0;r<N;r++){
		resultado_int[r] = fib_int(v[r]);	
 }
 endtime = dwalltime() - timetick;
 
 //Calcula el error de precision por tipo de dato
 double error = 0;
 for(r=0;r<N;r++){
		error += abs(resultado_aureo[r] - resultado_int[r]);	
 } 
 
printf("Tiempo INT: %.2f segundos\n",endtime);
printf("Precision (Error Medio):%lf\n",error/N);  

 free(v);
 free(resultado_aureo);
 free(resultado_int);
 return(0);
}

