#include<stdio.h>
#include<stdlib.h>

/* Time in seconds from some point in the past */
double dwalltime();

int main(int argc,char*argv[]){
 double *A,*B,*C,*D,*ba,*ca,*cad, *tot;
 int i,j,k,N;
 int check=1;
 double timetick;

 //Controla los argumentos al programa
  if (argc < 2){
   printf("\n Falta un argumento:: N dimension de la matriz \n");
   return 0;
  }
 
   N=atoi(argv[1]);

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);
  D=(double*)malloc(sizeof(double)*N*N);
  ba=(double*)malloc(sizeof(double)*N*N);
  ca=(double*)malloc(sizeof(double)*N*N);
  cad=(double*)malloc(sizeof(double)*N*N);
  tot=(double*)malloc(sizeof(double)*N*N);
  

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	A[i+j*N]=1;
	B[i*N+j]=1;
	C[i*N+j]=1;
	D[i+j*N]=1;
    	ba[i*N+j]=0;
	ca[i*N+j]=0;
	cad[i*N+j]=0;
   }
  }   

 timetick = dwalltime();
 //Realiza la multiplicacion
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    for(k=0;k<N;k++){
	ba[i*N+j]= ba[i*N+j] + B[i*N+k]*A[k+j*N];
	ca[i*N+j]= ca[i*N+j] + C[i*N+k]*A[k+j*N];
    }
   }
  }   
 //Es necesario hacer un bucle mas porque el calculo de (CA)D necesita el resultado de CA que en el bucle anterior no está completo 
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    for(k=0;k<N;k++){
	cad[i*N+j]= cad[i*N+j] + ca[i*N+k]*D[k+j*N];
    }
    tot[i*N+j]= ba[i*N+j]+ cad[i*N+j];
   }
  }   
   
  printf("Tiempo en segundos DOS BUCLES %f \n", dwalltime() - timetick);

 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(tot[i*N+j]==N+N*N);
   }
  }   

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }

 //Inicializa las matrices resultados
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    	ba[i*N+j]=0;
	ca[i*N+j]=0;
	cad[i*N+j]=0;
   }
  }   
  
timetick = dwalltime();
 //Realiza la multiplicacion
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    for(k=0;k<N;k++){
	ba[i*N+j]= ba[i*N+j] + B[i*N+k]*A[k+j*N];
    }
   }
  } 

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    for(k=0;k<N;k++){
        ca[i*N+j]= ca[i*N+j] + C[i*N+k]*A[k+j*N];
    }
   }
  }

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    for(k=0;k<N;k++){
        cad[i*N+j]= cad[i*N+j] + ca[i*N+k]*D[k+j*N];
    }
   }
  }
  
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
        tot[i*N+j]= ba[i*N+j]+ cad[i*N+j];
   }
  }

  printf("Tiempo en segundos CUATRO BUCLES %f \n", dwalltime() - timetick);
 
 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(tot[i*N+j]==N+N*N);
   }
  }   

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }


 free(A);
 free(B);
 free(C);
 free(D);
 free(ba);
 free(ca);
 free(cad);
 free(tot);
 return(0);
}



/*****************************************************************/

#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

