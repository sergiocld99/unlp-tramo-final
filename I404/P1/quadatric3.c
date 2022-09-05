#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TIMES 100
#define N 10000000

#define FA 1.0f
#define FB -4.0000000f
#define FC 3.9999999f

#define DA 1.0
#define DB -4.0000000
#define DC 3.9999999


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
		fa[i] = FA;
	for (i=0; i<N ; i++)
		fb[i] = FB;
	for (i=0; i<N ; i++)
		fc[i] = FC;
	for (i=0; i<N ; i++)
		da[i] = DA;
	for (i=0; i<N ; i++)
		db[i] = DB;
	for (i=0; i<N ; i++)
		dc[i] = DC;

	// computar soluciones usando float
	tick = dwalltime();
	
	for (j=0; j<TIMES ; j++)
		for (i=0; i<N ; i++) {
	//		flt_solve(fa[i], fb[i], fc[i]);
			float d = powf(fb[i],2.0f) - 4.0f*fa[i]*fc[i];
			float sd = sqrtf(d);
			float r1 = (-fb[i] + sd) / (2.0f*fa[i]);
			float r2 = (-fb[i] - sd) / (2.0f*fa[i]);
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

