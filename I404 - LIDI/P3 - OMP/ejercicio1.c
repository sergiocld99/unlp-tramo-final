//Ejercicio 1
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(int argc,char*argv[]){
 double *A;
 int i,j;
 int check = 1; 
 
 int numThreads = atoi(argv[2]);
 int N=atoi(argv[1]);
 omp_set_num_threads(numThreads);

 //Aloca memoria para la matriz
  A=(double*)malloc(sizeof(double)*N*N);
 
 //Inicializa la matriz. Cada posicion debe quedar con el valor I*J
 // I => fila J=> columna. 

  for(i=0;i<N;i++){
   
   // A es compartida -> bien
   
   // 1 - ¿son las iteraciones independientes entre sí? (...)
   // 2 - ¿el numero de iteraciones ya es conocido? (Sí, es N)
   // 3 - ¿la variable indice la modifican los hilos? (No la modifican)
   // 4 - ¿hay algún break? (No)

   // PROBLEMA: private(i) hace que siempre valga 0 dentro del for
      // CAUSA: no se le asigna un valor a la copia local de cada hilo (por defecto 0)
      // NO CONFUNDIR: el bucle for(i=0..N-1) exterior NO INTERFIERE (la copia no toma su valor)
   // SOLUCIÓN: shared(i)

   #pragma omp parallel for shared(A,i) private(j)
   for(j=0;j<N;j++){
      // matriz inicializada por filas
		A[i*N+j]=i*j;
      // printf("A[%d,%d] = %f\n", i, j, A[i*N+j]);
   }
  }   

 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(A[i*N+j]==i*j);
   }
  }   

  if(check){
   printf("Resultado correcto\n");
  }else{
   printf("Resultado erroneo \n");
  }

 free(A);

 return(0);
}

