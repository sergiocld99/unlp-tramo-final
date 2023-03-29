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
  double *M, *L, *C;
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
  L = (double*) malloc(sizeof(double)*N*SUMATORIA(N));        
  C = (double*) malloc(sizeof(double)*N*N);
  
  // Inicialización
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      M[i*N+j] = 1;       // M ordenada por filas
      C[i*N+j] = 0;       // Suma parcial en 0
    }
  }
  
  // l ORDENADO POR COLUMNAS
  // En la columna j hay N-j valores no nulos

  // L[i,j] = 0 para j > i == i < j
  // L[0,0] = L[0]; L[1,1] = L[N]; L[2,2] = L[N+N-1] = L[2N-1]; L[3,3] = L[3N-3]
  // En cada columna j tenemos j valores nulos, por eso restamos SUMATORIA(j)

  for (i=0; i<N; i++){
    for (j=0; j<=i; j++){
       L[j*N-SUMATORIA(j)+i] = 1;
    }
  }
  
  // ==================================== CALCULO ========================== //
  
  timetick = dwalltime();
  
  // L[i,j] = 0 para j>i
  // L[k,j] = 0 para j>k, en otras palabras, nulos en k<j

  // L[i,j] = L[Suma(N..N-j)+i]   (por columnas)
  // l[k,j] = L[Suma(N..N-j)+k]

  int offsetColumna;

  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      offsetColumna = j*N - SUMATORIA(j);

      for (k=j; k<N; k++){
        C[i*N+j] += M[i*N+k] * L[offsetColumna+k];
      }
    }
  }
  
  printf("N = %d. Tiempo en segundos %f\n", N, dwalltime() - timetick);
  
  // ================================== FIN DE CALCULOS ========================= //
  
  // Verificacion
  
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      if (C[i*N+j] != N-j){
      	check = 0;
      }
    }
  }
  
  if (check) printf("Resultados correctos, bien hecho!");
  else printf("Todo mal...");
  
  printf("\n\n");
  
  // Liberar memoria
  free(M);
  free(L);
  free(C);
 
  return 0;
}
