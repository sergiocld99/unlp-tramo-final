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
  M = (double*) malloc(sizeof(double)*N*N);            // N x N
  U = (double*) malloc(sizeof(double)*N*N);            // Almacenando ceros
  C = (double*) malloc(sizeof(double)*N*N);
  
  // Inicialización
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      M[i*N+j] = 1;       // M ordenada por filas
      C[i*N+j] = 0;       // Suma parcial en 0
    }
  }
  
  for (i=0; i<N; i++){
    for (j=0; j<i; j++){
      U[i+j*N] = 0;       // Parte inferior 
    }
    
    for (j=i; j<N; j++){
      U[i+j*N] = 1;       // Parte superior 
    }
  }
  
  // ==================================== CALCULO ========================== //
  
  timetick = dwalltime();
  
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      for (k=0; k<N; k++){
        C[i*N+j] += M[i*N+k] * U[k+j*N];    // Esta forma no es la más eficiente (multiplica ceros) 
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
