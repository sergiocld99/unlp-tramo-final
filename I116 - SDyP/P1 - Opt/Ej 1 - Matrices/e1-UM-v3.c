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
      M[i+j*N] = 1;       // M ordenada por COLUMNAS!!!
      C[i*N+j] = 0;       // Suma parcial en 0
    }
  }
  
  // U ordenada por filas, U[i,j] = 0 para j<i
  // U[1,1] = U[N]; U[2,2] = U[2N-1]; U[3,3] = U[3N-3]...
  // En cada fila i tenemos i valores nulos, por eso restamos SUMATORIA(i)

  for (i=0; i<N; i++){
    for (j=i; j<N; j++){
      U[i*N+j-SUMATORIA(i)] = 1;
    }
  }
  
  // ==================================== CALCULO ========================== //
  
  timetick = dwalltime();
  
  // U[i,j] = 0 para j<i
  // U[i,k] = 0 para k<i

  int indiceFila;

  for (i=0; i<N; i++){
    indiceFila = i*N - SUMATORIA(i);

    for (j=0; j<N; j++){
      for (k=i; k<N; k++){
        C[i*N+j] += U[indiceFila+k] * M[k+j*N];  
      }
    }
  }
  
  printf("N = %d. Tiempo en segundos %f\n", N, dwalltime() - timetick);
  
  // ================================== FIN DE CALCULOS ========================= //
  
  // Verificacion
  
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      if (C[i*N+j] != N-i){
      	printf("ERROR: C[%d, %d] deberia ser %d y encontre %f\n", i, j, N-i, C[i*N+j]);
      	check = 0;
      	break;
      }
    }
  }
  
  if (check) printf("Resultados correctos, bien hecho!");
  
  printf("\n\n");
  
  // Liberar memoria
  free(M);
  free(U);
  free(C);
 
  return 0;
}
