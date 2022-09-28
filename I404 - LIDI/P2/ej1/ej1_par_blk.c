/* Blocked matrix multiplication example */
/* Fernando G. Tinetti                   */

#include<stdio.h>
#include<stdlib.h>   /* malloc() */

// Librería P-threads
#include <pthread.h>

/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val, int transpose); 
 
/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, int bs);

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs);

/* Time in seconds from some point in the past */
double dwalltime();

// estructura de params
typedef struct {
    int startIndex;
    int querySize;
} pthread_params_t;

// variables globales
double *a, *b, *c;
int n, bs;

// prototipo funcion ejecutada por cada hilo
void* resolver(void*);


/************** MAIN *************/
int main(int argc, char *argv[])
{
  // double *a, *b, *c;   ahora son globales
  int i, j, T;            // n y bs globales

  double timetick;

  /* Check command line parameters */
  if ( (argc != 4) || ((n = atoi(argv[1])) <= 0) || ((bs = atoi(argv[2])) <= 0) || ((T = atoi(argv[3])) <= 0))
  {
    printf("\nError en los parametros. Usage: ./%s N BS T (N debe ser multiplo de BS)\n", argv[0]);
    exit(1);
  }

  /* Getting memory */  
  a = (double *) malloc(n*n*sizeof(double));
  b = (double *) malloc(n*n*sizeof(double));
  c = (double *) malloc(n*n*sizeof(double));

  /* Init matrix operands */
  initvalmat(a, n, 1.0, 0);
  initvalmat(b, n, 1.0, 1);

  /* Init matrix c, just in case */  
  initvalmat(c, n, 0.0, 0);

 // printf("Multiplying %d x %d matrices\n", n, n);
  
  int h;
  
  // pthread
  pthread_attr_t attr;
  pthread_t threads[T];
  pthread_params_t params[T];

  // inicializar atributos
  pthread_attr_init(&attr);

  // supongamos N = 512 y bs = 32, son 16 iteraciones
  // Crear hilo por cada bloque? no
  // podemos dividir el trabajo por cantidad de filas
  // hilo 1 de i=0..n/2, hilo 2 de n/2 a n

  timetick = dwalltime();

  for (h = 0; h < T; h++){
    params[h].startIndex = h * (n/T);
    params[h].querySize = n/T;

    pthread_create(&threads[h], &attr, resolver, &params[h]);

    // matmulblks(a, b, c, n, bs);
  }
  
  // join
  for (i=0; i<T; i++){
    pthread_join(threads[i], NULL);
  }
  
  
  double workTime = dwalltime() - timetick;

  /* Check results (just in case...) */
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      if (c[i*n + j] != n)
      {
        printf("Error at %d, %d, value: %f\n", i, j, c[i*n + j]);
      }
    }
  }

  printf("MMBLK-SEC;%d;%d;%lf;%lf\n",n,bs,workTime,((double)2*n*n*n)/(workTime*1000000000));

  return 0;
}

/*****************************************************************/

/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val, int transpose)
{
  int i, j;      /* Indexes */

	if (transpose == 0) {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[i*n + j] = val;
		}
	  }
	} else {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[j*n + i] = val;
		}
	  }
	}
}

/*****************************************************************/

void* resolver(void* arg){
  pthread_params_t* params = (pthread_params_t*) arg;
  int startIndex = (*params).startIndex;    // solo para las filas
  int querySize = (*params).querySize;              // solo para las filas

  // reemplazo de la funcion matmulblks
  int i, j, k;

  for (i = startIndex; i < startIndex + querySize; i += bs){
    for (j = 0; j < n; j += bs){
      for (k = 0; k < n; k += bs){
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
