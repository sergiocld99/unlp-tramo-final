// IMPLEMENTACIÓN SECUENCIAL

#include <stdio.h>
#include <stdlib.h>

/* Time in seconds from some point in the past */
double dwalltime();

// ------------------------------------------------

// N se recibe por parámetro
int main(int argc, char* argv[]){
  int i, n;
	double *v, min = 9999.9, max = -9999.9, acum = 0.0, prom, startTime, totalTime;

	/* Leer argumentos */
	if ((argc != 2) || ((n = atoi(argv[1])) <= 0) ) {
	    printf("\nUsar: %s size \n  size: Dimension de la matriz y el vector\n", argv[0]);
		exit(1);
	}

  // Reservar memoria para las matrices operando
  v = (double*) malloc(sizeof(double)*n);
  for (i=0; i<n; i++) v[i] = i;

	startTime = dwalltime();

  // ------------------------- TRABAJO ---------------------------------

  for (i=0; i<n; i++){
    if (v[i] < min) min = v[i];
    if (v[i] > max) max = v[i];
    acum += v[i];
  }

  prom = acum / n;

  totalTime = dwalltime() - startTime;

  // ------------------------ CHECK ---------------------------------

  // Check results
	if (min == 0) printf("minimo correcto\n");
  if (max == n-1) printf("máximo correcto\n");

  printf("Vector (N=%d)\tTiempo total=%lf\n",n,totalTime);

  // ------------------------ FREE ------------------------------------

  free(v);
	return 0;
}

/*****************************************************************/
    
/*****************************************************************/

#include <stdio.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
