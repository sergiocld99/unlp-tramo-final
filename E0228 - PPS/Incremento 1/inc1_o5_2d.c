#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/opt_0-n.c"
#include "../src/floyd_versions/common/malloc/aligned.c"

#include <semaphore.h>

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
	sem_t** semaforos;

	// asignación de memoria para semaforos
	sem_t** semaforos = (sem_t**) malloc(r * sizeof(sem_t*));

	for (x=0; x<r; x++){
		semaforos[x] = (sem_t*) malloc(r * sizeof(sem_t));
	}

	// inicialización de semáforos
	// segundo parametro: compartido entre hilos (0), en lugar de procesos
	// tercer parametro: inicializados con el valor 0
	for (x=0; x<r; x++){
		for (y=0; y<r; y++){
			sem_init(&semaforos[x][y], 0, 0);
		}
	}

	// ------------------------- FIN BLOQUE AGREGADO -----------------------

	// Modificación: shared(semaforos)

	#pragma omp parallel shared(semaforos) default(none) firstprivate(r,row_of_blocks_disp,num_of_bock_elems,D,P) num_threads(t)
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

			//Phase 2 y 3
			#pragma omp for schedule(dynamic) nowait
			for(w=0; w<r*2; w++){
				if(w<r){ //Phase 2
					j = w;
					if(j == k) continue;		// No se computa la fila k

					kj = k_row_disp + j*num_of_bock_elems;
					FW_BLOCK(D, kj, kk, kj, P, b);

					// -------------- BLOQUE AGREGADO -------------------

					// Finalizo el computo del bloque (k,j) = (k,w)
					// Fila de semaforo es k, entonces el comienzo de fila es k*r
					// Columna de semaforo es j, entonces el indice es k*r+j
					for (aux=0; aux<r-1; aux++) sem_post(&semaforos[k][j]);

					// -------------- FIN BLOQUE AGREGADO -----------------

				} else { //Phase 3
					i = w - r;
					if(i == k) continue;		// No se computa la columna k

					ik = i*row_of_blocks_disp + k_col_disp;
					FW_BLOCK(D, ik, ik, kk, P, b);

					// -------------- BLOQUE AGREGADO -------------------

					// Finalizo el computo del bloque (i,k) = (w-r, k)
					// Fila de semaforo es i, entonces el comienzo de fila es i*r
					// Columna de semaforo es k, entonces el indice es i*r+k
					for (aux=0; aux<r-1; aux++) sem_post(&semaforos[i][k]);

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

					// Esperar que se compute el bloque (k,j)
					sem_wait(&semaforos[k][j]);
					
					// Esperar que se compute el bloque (i,k)
					sem_wait(&semaforos[i][k]);

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
	for (x=0; x<r; x++) free(semaforos[x]);
	free(semaforos);

	// ------------------------- FIN BLOQUE AGREGADO -----------------------
}
