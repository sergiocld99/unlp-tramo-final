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
  L = (double*) malloc(sizeof(double)*N*SUMATORIA(N));            // Sin almacenar ceros
  C = (double*) malloc(sizeof(double)*N*N);
  
  // Inicialización
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      M[i+j*N] = 1;       // M ordenada por COLUMNAS
      C[i*N+j] = 0;       // Suma parcial en 0
    }
  }

  // No almacenar ceros: tenemos i+1 elementos no nulos en cada fila i
  // L[0,0] = L[0]; L[1,0] = L[1]; L[2,0] = L[3]; l[3,0] = l[6] ...
  
  for (i=0; i<N; i++){
    for (j=0; j<=i; j++){
      L[SUMATORIA(i)+j] = 1;    // ORDENADO POR FILAS
    }
  }
  
  // ==================================== CALCULO ========================== //
  
  timetick = dwalltime();

  // L[i,j] = 0 si j > i
  // L[i,k] = 0 si k > i

  // l[i,j] = L[Suma(0..i)+j]
  // l[i,k] = L[Suma(0..i)+k]

  int inicioFila = 0;

  for (i=0; i<N; i++){
    inicioFila += i;

    for (j=0; j<N; j++){
      for (k=0; k<=i; k++){
        C[i*N+j] += L[inicioFila+k] * M[k+j*N]; 
      }
    }
  }
  
  printf("N = %d. Tiempo en segundos %f\n", N, dwalltime() - timetick);
  
  // ================================== FIN DE CALCULOS ========================= //
  
  // Verificacion
  // El resultado es una matriz donde cada fila repite el valor i+1 en todas sus columnas

  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      if (C[i*N+j] != i+1){
      	printf("ERROR: C[%d, %d] deberia ser %d y encontre %f\n", i, j, i+1, C[i*N+j]);
      	check = 0;
      	break;
      }
    }
  }
  
  if (check) printf("Resultados correctos, bien hecho!");
  
  printf("\n\n");
  
  // Liberar memoria
  free(M);
  free(L);
  free(C);
 
  return 0;
}
