#include<stdio.h>
#include<stdlib.h>
#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1

// Agregué esto
#include <sys/time.h>

//Dimension por defecto de las matrices
int N=100;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 double *A,*B,*C;
 int i,j,k;
 int check=1;
 double timetick;

 // auxiliares
 int inicioFila, inicioCol;

 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N

  for(i=0;i<N;i++){
   inicioFila = i*N;    // Solo vale para las ordenadas por fila

   for(j=0;j<N;j++){
	   A[inicioFila+j] = 1;     // A ordenado por filas
      B[i+N*j] = 1;           // B ordenado por columnas
   }
  }   


 //Realiza la multiplicacion

  timetick = dwalltime();

  // Variable para evitar multiples accesos
  double suma;

  for(i=0;i<N;i++){
   inicioFila = i*N;      // Solo vale para ordenadas por fila

   for(j=0;j<N;j++){
      suma = 0;         // Reiniciamos suma
      inicioCol = j*N;       // Solo vale para ordenadas por columna

      for(k=0;k<N;k++){
         // Sumamos A[i,k] * B[k,j] a la matriz C
         // A está ordenado por filas, B por columnas
         suma = suma + A[inicioFila+k] * B[k+inicioCol]; 
      }

      // Asignamos suma a C (ordenado por filas)
      C[inicioFila+j] = suma;
   }
  }   

 printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",N,N, dwalltime() - timetick);

 //Verifica el resultado (si alguna celda no vale N, check toma el valor false)
  for(i=0;i<N;i++){
   inicioFila = i*N;

   for(j=0;j<N;j++){
      // C ordenado por filas
      check = check && (C[inicioFila+j] == N);
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
 return(0);
}
