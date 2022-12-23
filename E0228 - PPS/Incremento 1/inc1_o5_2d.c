#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/opt_0-n.c"
#include "../src/floyd_versions/common/malloc/aligned.c"

#include <semaphore.h>
#include <pthread.h>

//Public
char* getFloydName(){
	return "semaphores with data alignment";
}

//Public
char* getFloydVersion(){
	return "Inc1_Opt5";
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
				if(sum < dij){
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
				if(sum < dij){
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

	INT64 x, y;
	char** pendientes;
	pthread_cond_t** cv;
	pthread_mutex_t** mutex;

	// asignación de memoria (convenientemente matrices separadas)
	pendientes = (char**) malloc(r * sizeof(char*));
	for (x=0; x<r; x++) pendientes[x] = (char*) malloc(r * sizeof(char));

	cv = (pthread_cond_t**) malloc(r * sizeof(pthread_cond_t*));
	for (x=0; x<r; x++) cv[x] = (pthread_cond_t*) malloc(r * sizeof(pthread_cond_t));

	mutex = (pthread_mutex_t**) malloc(r * sizeof(pthread_mutex_t*));
	for (x=0; x<r; x++) mutex[x] = (pthread_mutex_t*) malloc(r * sizeof(pthread_mutex_t));

	// inicialización de pendientes
	for (x=0; x<r; x++){
		for (y=0; y<r; y++){
			// pendientes[x][y] = 2;
			pthread_cond_init(&cv[x][y], NULL);
			pthread_mutex_init(&mutex[x][y], NULL);
		}
	}

	// ------------------------- FIN BLOQUE AGREGADO -----------------------

	// Modificación: shared(pendientes,mutex,cv)

	#pragma omp parallel shared(pendientes,mutex,cv) default(none) firstprivate(r,row_of_blocks_disp,num_of_bock_elems,D,P) num_threads(t)
	{
		INT64 i, j, k, b, kj, ik, kk, ij, k_row_disp, k_col_disp, i_row_disp, j_col_disp, w;

		// Variable agregada
		INT64 aux;

		for(k=0; k<r; k++){
			b = k*BS;
			k_row_disp = k*row_of_blocks_disp;
			k_col_disp = k*num_of_bock_elems;

			//Phase 1
			kk = k_row_disp + k_col_disp;
			FW_BLOCK_PARALLEL(D, kk, kk, kk, P, b);

			// Asignación de pendientes
			#pragma omp for collapse(2) schedule(dynamic)
			for (i=0; i<r; i++){
				for (j=0; j<r; j++){
					pendientes[i][j] = 2;
				}
			}

			//Phase 2 y 3
			#pragma omp for schedule(dynamic) nowait
			for(w=0; w<r*2; w++){
				if(w<r){ //Phase 2
					j = w;
					if(j == k) continue;		// No se computa la columna k

					kj = k_row_disp + j*num_of_bock_elems;
					FW_BLOCK(D, kj, kk, kj, P, b);

					// -------------- BLOQUE AGREGADO -------------------

					// Finalizó el computo del bloque (k,j) = (k,w)
					// Modif: se debe decrementar pendientes de la columna actual "j"
					for (aux=0; aux<r; aux++){
						if (aux == k) continue;			// no se levanta actual (k,j)
						pthread_mutex_lock(&mutex[aux][j]);
						pendientes[aux][j]--;
						pthread_mutex_unlock(&mutex[aux][j]);
						pthread_cond_signal(&cv[aux][j]);
					}

					// -------------- FIN BLOQUE AGREGADO -----------------

				} else { //Phase 3
					i = w - r;
					if(i == k) continue;		// No se computa la fila k

					ik = i*row_of_blocks_disp + k_col_disp;
					FW_BLOCK(D, ik, ik, kk, P, b);

					// -------------- BLOQUE AGREGADO -------------------

					// Finalizo el computo del bloque (i,k) = (w-r, k)
					// Modif: se debe decrementar pendientes de la fila actual "i"
					for (aux=0; aux<r; aux++) {
						if (aux == k) continue;			// no se levanta actual (i,k)
						pthread_mutex_lock(&mutex[i][aux]);
						pendientes[i][aux]--;
						pthread_mutex_unlock(&mutex[i][aux]);
						pthread_cond_signal(&cv[i][aux]);
					}

					// -------------- FIN BLOQUE AGREGADO -----------------

				}
			}

			//Phase 4
			#pragma omp for collapse(2) schedule(dynamic)
			for(i=0; i<r; i++){
				for(j=0; j<r; j++){
					// "i" corresponde a filas, y "j" a columnas
					// No se computan la fila k y la columna k
					if( (j == k) || (i == k) ) continue;

					// ----------- BLOQUE AGREGADO -----------------

					// Esperar que se computen los bloques (k,j) e (i,k)
					pthread_mutex_lock(&mutex[i][j]);
					while (pendientes[i][j] > 0) pthread_cond_wait(&cv[i][j], &mutex[i][j]);
					pthread_mutex_unlock(&mutex[i][j]);

					// ---------- FIN BLOQUE AGREGADO --------------

					i_row_disp = i*row_of_blocks_disp;
					ik = i_row_disp + k_col_disp;
					j_col_disp = j*num_of_bock_elems;
					kj = k_row_disp + j_col_disp;
					ij = i_row_disp + j_col_disp;
					FW_BLOCK(D, ij, ik, kj, P, b);
				}
			}
		}
	}

	// --------------------------- BLOQUE AGREGADO -----------------------

	// liberación de memoria reservada
	for (x=0; x<r; x++) {
		free(mutex[x]);
		free(pendientes[x]);
		free(cv[x]);
	}

	free(mutex);
	free(pendientes);
	free(cv);

	// ------------------------- FIN BLOQUE AGREGADO -----------------------
}
