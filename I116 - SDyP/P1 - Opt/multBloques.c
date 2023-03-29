// Multiplicacion de matrices cuadradas por bloques

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	sec = tv.tv_sec + tv.tv_usec / 1000000.0;
	return sec;
}

// Multiplicaci�n de matrices por bloques
void matmulblks(double *a, double *b, double *c, int n, int bs)
{
	double *ablk, *bblk, *cblk;
	int I, J, K;
	int i, j, k;

	for (I = 0; I < n; I += bs)
	{
		for (J = 0; J < n; J += bs)
		{
			cblk = &c[I * n + J];
			for (K = 0; K < n; K += bs)
			{
				ablk = &a[I * n + K];
				bblk = &b[J * n + K];

				for (i = 0; i < bs; i++)
				{
					for (j = 0; j < bs; j++)
					{
						for (k = 0; k < bs; k++)
						{
							cblk[i * n + j] += ablk[i * n + k] * bblk[j * n + k];
						}
					}
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	double *A, *B, *C;
	int n, bs, i, j;

	double timetick;

	// Chequeo de par�metros
	if ((argc != 3) || ((n = atoi(argv[1])) <= 0) || ((bs = atoi(argv[2])) <= 0) || ((n % bs) != 0))
	{
		printf("Error en los par�metros. Usar: ./%s N BS (N debe ser multiplo de BS)\n", argv[0]);
		exit(1);
	}

	// Alocar
	A = (double *)malloc(n * n * sizeof(double));
	B = (double *)malloc(n * n * sizeof(double));
	C = (double *)malloc(n * n * sizeof(double));

	// Inicializacion
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			A[i * n + j] = 1.0;
			B[j * n + i] = 1.0;
			C[i * n + j] = 0.0;
		}
	}

	printf("Multiplicando matrices de %d x %d en bloques de %d x %d\n", n, n, bs, bs);

	timetick = dwalltime();

	matmulblks(A, B, C, n, bs);

	double totalTime = dwalltime() - timetick;

	// Validando
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (C[i * n + j] != n)
			{
				printf("Error en %d, %d, valor: %f\n", i, j, C[i * n + j]);
			}
		}
	}

	printf("Tiempo en segundos %f\n", totalTime);

	free(A);
	free(B);
	free(C);

	return 0;
}
