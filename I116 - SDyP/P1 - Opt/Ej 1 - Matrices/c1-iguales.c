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
  double *A, *C;
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
  // B = (double *)malloc(sizeof(double) * N * N);
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

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < N; k++) {
        // En este caso, A[k,j] no aprovecha localidad espacial (salto k*N)
        C[i*N+j] += A[i*N+k] * A[k*N+j];
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
  // free(B);
  free(C);

  return 0;
}


// ======================================================================= //