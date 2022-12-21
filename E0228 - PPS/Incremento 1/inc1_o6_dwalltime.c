#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/opt_0-n.c"
#include "../src/floyd_versions/common/malloc/aligned.c"

#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)


// ----------------- BLOQUE AGREGADO -------------------

#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

double dwalltime1(){
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

// ---------------- FIN BLOQUE AGREGADO --------------


//Public
char* getFloydName(){
	return "cond variables with branch predication";
}

//Public
char* getFloydVersion(){
	return "Inc1_Opt6_v3";
}

static inline void FW_BLOCK(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base) __attribute__((always_inline));

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base){
	INT64 i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
	TYPE dij, dik, dkj, sum;

	for(k=0; k<BS; k++){
		k_disp = k*BS;
		k_disp_d3 = k_disp + d3;
		for(i=0; i<BS; i++){
			i_disp = i*BS;
			i_disp_d1 = i_disp + d1;
			dik = graph[i_disp + d2 + k];
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = graph[i_disp_d1 + j];
				dkj = graph[k_disp_d3 + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
					graph[i_disp_d1 + j] = sum;
					#ifndef NO_PATH
						path[i_disp_d1 + j] = base + k;
					#endif
				}
			}
		}
	}
}

static inline void FW_BLOCK_PARALLEL(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base) __attribute__((always_inline));

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_PARALLEL(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base){
	INT64 i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
	TYPE dij, dik, dkj, sum;

	for(k=0; k<BS; k++){
		k_disp = k*BS;
		k_disp_d3 = k_disp + d3;
		#pragma omp for
		for(i=0; i<BS; i++){
			i_disp = i*BS;
			i_disp_d1 = i_disp + d1;
			dik = graph[i_disp + d2 + k];
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = graph[i_disp_d1 + j];
				dkj = graph[k_disp_d3 + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
					graph[i_disp_d1 + j] = sum;
					#ifndef NO_PATH
						path[i_disp_d1 + j] = base + k;
					#endif
				}
			}
		}
	}
}

//Public
void floydWarshall(TYPE* D, int* P, int n, int t){
	INT64 r, row_of_blocks_disp, num_of_bock_elems;	
	r = n/BS;
	row_of_blocks_disp = n*BS;
	num_of_bock_elems = BS*BS;

	// --------------------------- BLOQUE AGREGADO -----------------------

	int terminados = 0;
	pthread_mutex_t mutex;
	double t1 = 0.0, t2 = 0.0, t4 = 0.0;

	// asignación de memoria
	

	// inicialización de pendientes
	pthread_mutex_init(&mutex, NULL);

	// ------------------------- FIN BLOQUE AGREGADO -----------------------

	// Modificación: shared(pendientes,mutex,cv)

	#pragma omp parallel shared(terminados,mutex,t1,t2,t4) default(none) firstprivate(r,row_of_blocks_disp,num_of_bock_elems,D,P) num_threads(t)
	{
		INT64 i, j, k, b, kj, ik, kk, ij, k_row_disp, k_col_disp, i_row_disp, j_col_disp, w;

		// Variable agregada
		INT64 aux;
		double t0;

		for(k=0; k<r; k++){
			b = k*BS;
			k_row_disp = k*row_of_blocks_disp;
			k_col_disp = k*num_of_bock_elems;

			//Phase 1
			t0 = dwalltime1();
			kk = k_row_disp + k_col_disp;
			FW_BLOCK_PARALLEL(D, kk, kk, kk, P, b);
			
			if (omp_get_thread_num() == 0){
				pthread_mutex_lock(&mutex);
				t1 += (dwalltime1() - t0);
				pthread_mutex_unlock(&mutex);
			}
			
			t0 = dwalltime1();

			//Phase 2 y 3
			#pragma omp for schedule(dynamic) nowait
			for(w=0; w<r*2; w++){
				if(w<r){ //Phase 2
					j = w;
					if(j == k) continue;		// No se computa la columna k

					t0 = dwalltime1();
					kj = k_row_disp + j*num_of_bock_elems;
					FW_BLOCK(D, kj, kk, kj, P, b);

					// -------------- BLOQUE AGREGADO -------------------

					// Finalizó el computo del bloque (k,j) = (k,w)
					// Modif: se debe decrementar pendientes de la columna actual "j"

					// -------------- FIN BLOQUE AGREGADO -----------------

				} else { //Phase 3
					i = w - r;
					if(i == k) continue;		// No se computa la fila k

					ik = i*row_of_blocks_disp + k_col_disp;
					FW_BLOCK(D, ik, ik, kk, P, b);

					// -------------- BLOQUE AGREGADO -------------------

					// Finalizo el computo del bloque (i,k) = (w-r, k)
					// Modif: se debe decrementar pendientes de la fila actual "i"

					// -------------- FIN BLOQUE AGREGADO -----------------

				}
			}

			if (omp_get_thread_num() == 0){
				pthread_mutex_lock(&mutex);
				t2 += (dwalltime1() - t0);
				pthread_mutex_unlock(&mutex);
			}

			t0 = dwalltime1();

			//Phase 4
			#pragma omp for collapse(2) schedule(dynamic)
			for(i=0; i<r; i++){
				for(j=0; j<r; j++){
					// "i" corresponde a filas, y "j" a columnas
					// No se computan la fila k y la columna k
					if( (j == k) || (i == k) ) continue;

					// ----------- BLOQUE AGREGADO -----------------

					// Esperar que se computen los bloques (k,j) e (i,k)

					// ---------- FIN BLOQUE AGREGADO --------------

					i_row_disp = i*row_of_blocks_disp;
					ik = i_row_disp + k_col_disp;
					j_col_disp = j*num_of_bock_elems;
					kj = k_row_disp + j_col_disp;
					ij = i_row_disp + j_col_disp;
					FW_BLOCK(D, ij, ik, kj, P, b);
					
				}
			}

			if (omp_get_thread_num() == 0){
				pthread_mutex_lock(&mutex);
				t4 += (dwalltime1() - t0);
				pthread_mutex_unlock(&mutex);
			}
		}
	}

	// --------------------------- BLOQUE AGREGADO -----------------------

	// liberación de memoria reservada
	printf("Tiempo de Fase 1: %f secs. \n", t1);
	printf("Tiempo de Fase 2-3: %f secs. \n", t2);
	printf("Tiempo de Fase 4: %f secs. \n", t4);

	// ------------------------- FIN BLOQUE AGREGADO -----------------------
	
}
