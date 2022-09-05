#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TIMES 1000
#define N 10000000

#define A 1.0
#define B -4.0000000
#define C 3.9999999


/* Time in seconds from some point in the past */
double dwalltime();

int main(void)
{
	float * fa = (float*) malloc(N*sizeof(float));
	float * fb = (float*) malloc(N*sizeof(float));
	float * fc = (float*) malloc(N*sizeof(float));
	double * da = (double*) malloc(N*sizeof(double));
	double * db = (double*) malloc(N*sizeof(double));
	double * dc = (double*) malloc(N*sizeof(double));
	double timed, timef, tick;
	int i, j;

	// inicializar vectores
	for (i=0; i<N ; i++)
		fa[i] = A;
	for (i=0; i<N ; i++)
		fb[i] = B;
	for (i=0; i<N ; i++)
		fc[i] = C;
	for (i=0; i<N ; i++)
		da[i] = A;
	for (i=0; i<N ; i++)
		db[i] = B;
	for (i=0; i<N ; i++)
		dc[i] = C;

	// computar soluciones usando float
	tick = dwalltime();
	
	for (j=0; j<TIMES ; j++)
		for (i=0; i<N ; i++) {
	//		flt_solve(fa[i], fb[i], fc[i]);
			float d = pow(fb[i],2) - 4.0*fa[i]*fc[i];
			float sd = sqrt(d);
			float r1 = (-fb[i] + sd) / (2.0*fa[i]);
			float r2 = (-fb[i] - sd) / (2.0*fa[i]);
		}

	
	timef = dwalltime() - tick;

	// computar soluciones usando double
	tick = dwalltime();

	for (j=0; j<TIMES ; j++)
		for (i=0; i<N ; i++){
			//dbl_solve(da[i], db[i], dc[i]);
			double d = pow(db[i],2) - 4.0*da[i]*dc[i];
			double sd = sqrt(d);
			double r1 = (-db[i] + sd) / (2.0*da[i]);
			double r2 = (-db[i] - sd) / (2.0*da[i]);
		}


	timed = dwalltime() - tick;    

	
	printf("Tiempo requerido solucion Double: %f\n",timed);
	printf("Tiempo requerido solucion Float: %f\n",timef);

	free(fa);
	free(fb);
	free(fc);
	free(da);
	free(db);
	free(dc);

	return 0;

}  

#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

