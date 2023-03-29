// cabeceras
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Macros
#define SUMATORIA(x) (x*(x+1) / 2)

// Para calcular tiempo
double dwalltime() {
  double sec;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  sec = tv.tv_sec + tv.tv_usec / 1000000.0;
  return sec;
}

// =========================== PROGRAMA PRINCIPAL ======================== //

int main(int argc, char *argv[]) {

  // Variables
  double *M, *U, *C;
  int i, j, k, N;
  int check = 1;
  double timetick;

  // Controla los argumentos al programa
  if ((argc != 2) || ((N = atoi(argv[1])) <= 0)) {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }
  
  // Reserva de memoria
  M = (double*) malloc(sizeof(double)*N*N);        
  U = (double*) malloc(sizeof(double)*N*SUMATORIA(N));          
  C = (double*) malloc(sizeof(double)*N*N);
  
  // Inicialización
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      M[i*N+j] = 1;       // M ordenada por filas
      C[i*N+j] = 0;       // Suma parcial en 0
    }
  }

  // U[i,j] = 0 para j<i
  // U por columnas: U[0,1] = U[1]; U[0,2] = U[3]; U[0,j] = U[Suma(0..j)]
  // En cada columna j tenemos (j+1) valores no nulos

  for (i=0; i<N; i++){
    for (j=i; j<N; j++){
      U[SUMATORIA(j)+i] = 1;
    }
  }
  
  // ==================================== CALCULO ========================== //
  
  timetick = dwalltime();
  
  // U[i,j] = 0 para j<i
  // U[k,j] = 0 para j<k, en otras palabras, nulos en k>j

  int offsetColumna;

  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      offsetColumna = SUMATORIA(j);

      for (k=0; k<=j; k++){
        C[i*N+j] += M[i*N+k] * U[k+offsetColumna];   
      }
    }
  }
  
  printf("N = %d. Tiempo en segundos %f\n", N, dwalltime() - timetick);
  
  // ================================== FIN DE CALCULOS ========================= //
  
  // Verificacion
  
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      if (C[i*N+j] != j+1){
      	check = 0;
      }
    }
  }
  
  if (check) printf("Resultados correctos, bien hecho!");
  else printf("Todo mal...");
  
  printf("\n\n");
  
  // Liberar memoria
  free(M);
  free(U);
  free(C);
 
  return 0;
}
