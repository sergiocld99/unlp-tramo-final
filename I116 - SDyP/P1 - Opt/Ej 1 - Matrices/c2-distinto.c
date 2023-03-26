// cabeceras
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
  double *A, *B, *C;
  int i, j, k, N;
  int check = 1;
  double timetick;

  // Controla los argumentos al programa
  if ((argc != 2) || ((N = atoi(argv[1])) <= 0)) {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }

  // Aloca memoria para las matrices
  A = (double *)malloc(sizeof(double) * N * N);
  B = (double *)malloc(sizeof(double) * N * N);
  C = (double *)malloc(sizeof(double) * N * N);

  // Ambas matrices inicializadas con mismo orden (elegí filas)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      A[i*N+j] = 1;
      // B[i*N+j] = 1;
      C[i*N+j] = 0;
    }
  }
  
  // =================================== CALCULO ======================== //

  timetick = dwalltime();

  // setValor(double *matriz, int fila, int columna, int orden, double valor)
    // ORDEN POR FILAS: matriz[fila * N + columna] = valor
    // ORDEN POR COLUMNAS: matriz[fila + columna * N] = valor

  // Construccion de la nueva matriz (A ordenada por columnas)
  for (i = 0; i < N; i++){
    for (j = 0; j < N; j++) {
      B[i+j*N] = A[i*N+j];
    }
  }

  // getValor(double *matriz, int fila, int columna, int orden)
    // ORDEN POR FILAS: matriz[fila * N + columna]
    // ORDEN POR COLUMNAS: matriz[fila + columna * N]

  // Multiplicacion
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < N; k++) {
        // En este caso, B[k,j] SÍ aprovecha localidad espacial (salto k unitario)
        C[i*N+j] += A[i*N+k] * B[k+j*N];
      }
    }
  }

  printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  // ============================== FIN DE CALCULOS ========================= //

  // Verifica el resultado
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      check = check && (C[i*N+j] == N);
    }
  }

  if (check) printf("Multiplicacion de matrices resultado correcto\n");
  else printf("Multiplicacion de matrices resultado erroneo\n");

  // Libera memoria alocada
  free(A);
  free(B);
  free(C);

  return 0;
}


// ======================================================================= //