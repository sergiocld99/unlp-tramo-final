// IMPLEMENTACIÓN SECUENCIAL

#include <stdio.h>
#include <stdlib.h>

/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, int bs);

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs);

/* Time in seconds from some point in the past */
double dwalltime();


// ------------------ MODULARIZACION -------------------

void matsum(double *a, double *b, double *c, double *res, int n){
    int i,j;

    for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
            // A y B accedidos por filas
			res[i*n+j] = a[i*n+j] + b[i*n+j] + c[i*n+j];
		}
	}
}

// ------------------------------------------------

// N se recibe por parámetro
int main(int argc, char* argv[]){
    int i, j, k, n, bs=32, check=1;
	double *a, *b, *c, *d, *e, *f, *r1, *r2, *r3, *rf;
	double startTime, totalTime;

	/* Leer argumentos */
	if ((argc != 2) || ((n = atoi(argv[1])) <= 0) ) {
	    printf("\nUsar: %s size \n  size: Dimension de la matriz y el vector\n", argv[0]);
		exit(1);
	}

    // chequear que N sea multiplo de bs
    if (n % bs != 0) {
		printf("N debe ser multiplo de bs.\n");
		exit(1);
	}

    // Reservar memoria para las matrices operando
    a = (double*) malloc(sizeof(double)*n*n);   // A -> scatter
    b = (double*) malloc(sizeof(double)*n*n);           // B -> bcast
    c = (double*) malloc(sizeof(double)*n*n);   // C -> scatter
    d = (double*) malloc(sizeof(double)*n*n);           // D -> bcast
    e = (double*) malloc(sizeof(double)*n*n);   // E -> scatter
    f = (double*) malloc(sizeof(double)*n*n);           // F -> bcast

    // Reservar memoria para las matrices temporales
    r1 = (double*) malloc(sizeof(double)*n*n);
    r2 = (double*) malloc(sizeof(double)*n*n);
    r3 = (double*) malloc(sizeof(double)*n*n);

    // Reservar memoria para la matriz resultado
    rf = (double*) malloc(sizeof(double)*n*n);  // rf -> gather

    // Inicializar datos de operandos
    for (i=0; i<n ; i++)
		for (j=0; j<n ; j++){
            a[i*n+j] = 1;			// por filas
            b[i*n+j] = 1;
            c[i*n+j] = 1;
            d[i*n+j] = 1;
            e[i*n+j] = 1;
            f[i*n+j] = 1;
        }

    // tiempo 0 = arranque
	startTime = dwalltime();

    // ------------------------- TRABAJO ---------------------------------

    matmulblks(a, b, r1, n, bs);
    matmulblks(c, d, r2, n, bs);
    matmulblks(e, f, r3, n, bs);
    matsum(r1, r2, r3, rf, n);

    totalTime = dwalltime() - startTime;

    // ------------------------ CHECK ---------------------------------

    // Check results
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			check = check && (rf[i*n+j] == 3*n);

	if(check) printf("Multiplicacion de matrices resultado correcto\n");
	else printf("Multiplicacion de matrices resultado erroneo\n");

    printf("Multiplicacion (N=%d)\tTiempo total=%lf\n",n,totalTime);

    // ------------------------ FREE ------------------------------------

    free(a);
	free(b);
	free(c);
    free(d);
    free(e);
    free(f);
    free(r1);
    free(r2);
    free(r3);
    free(rf);

	return 0;

}

/*****************************************************************/

/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, int bs)
{
  int i, j, k;    /* Guess what... */
  
  for (i = 0; i < n; i += bs)
  {
    for (j = 0; j < n; j += bs)
    {
      for  (k = 0; k < n; k += bs)
      {
        blkmul(&a[i*n + k], &b[j*n + k], &c[i*n + j], n, bs);
      }
    }
  }
}

/*****************************************************************/

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs)
{
  int i, j, k;    /* Guess what... again... */

  for (i = 0; i < bs; i++)
  {
    for (j = 0; j < bs; j++)
    {
      for  (k = 0; k < bs; k++)
      {
        cblk[i*n + j] += ablk[i*n + k] * bblk[j*n + k];
      }
    }
  }
}
    
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
