#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/opt_0-n.c"
#include "../src/floyd_versions/common/malloc/aligned.c"

#include<semaphore.h>

#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define UNROLL_FACTOR BS/(SIMD_WIDTH/TYPE_SIZE)

//Public
char* getFloydName(){
	return "with loop unroll + Inc_1";
}

//Public
char* getFloydVersion(){
	return "Opt-7_8";
}

static inline void FW_BLOCK(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base, int* const tmp1, int* const tmp2) __attribute__((always_inline));

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base, int* const tmp1, int* const tmp2){
	INT64 i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
	TYPE dij, dik, dkj, sum;

	for(k=0; k<BS; k++){
		k_disp = k*BS;
		k_disp_d3 = k_disp + d3;
		for(i=0; i<BS; i+=2){
			i_disp = i*BS;
			i_disp_d1 = i_disp + d1;
			dik = graph[i_disp + d2 + k];		
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#endif
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = graph[i_disp_d1 + j];
				dkj = graph[k_disp_d3 + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
					graph[i_disp_d1 + j] = sum;
					#ifndef NO_PATH
						path[i_disp_d1 + j] = base + k;
					#else
						tmp1[j] = tmp2[j];
					#endif
				}
			}
			i_disp = (i+1)*BS;
			i_disp_d1 = i_disp + d1;
			dik = graph[i_disp + d2 + k];			
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#endif
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = graph[i_disp_d1 + j];
				dkj = graph[k_disp_d3 + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
					graph[i_disp_d1 + j] = sum;
					#ifndef NO_PATH
						path[i_disp_d1 + j] = base + k;
					#else
						tmp1[j] = tmp2[j];
					#endif
				}
			}
		}
	}
}

static inline void FW_BLOCK_PARALLEL(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base, int* const tmp1, int* const tmp2) __attribute__((always_inline));

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_PARALLEL(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base, int* const tmp1, int* const tmp2){
	INT64 i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
	TYPE dij, dik, dkj, sum;

	for(k=0; k<BS; k++){
		k_disp = k*BS;
		k_disp_d3 = k_disp + d3;
		#pragma omp for
		for(i=0; i<BS; i+=2){
			i_disp = i*BS;
			i_disp_d1 = i_disp + d1;
			dik = graph[i_disp + d2 + k];
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#endif  
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = graph[i_disp_d1 + j];
				dkj = graph[k_disp_d3 + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
					graph[i_disp_d1 + j] = sum;
					#ifndef NO_PATH
						path[i_disp_d1 + j] = base + k;
					#else
						tmp1[j] = tmp2[j];
					#endif
				}
			}
			i_disp = (i+1)*BS;
			i_disp_d1 = i_disp + d1;
			dik = graph[i_disp + d2 + k];
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#endif  
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = graph[i_disp_d1 + j];
				dkj = graph[k_disp_d3 + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
					graph[i_disp_d1 + j] = sum;
					#ifndef NO_PATH
						path[i_disp_d1 + j] = base + k;
					#else
						tmp1[j] = tmp2[j];
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

	INT64 x, i,j,k, cant_bloques = r*r;
	sem_t** semaforos = (sem_t**) malloc(r * sizeof(sem_t *));
	for(k=0; k<r; k++)
		semaforos[k] = (sem_t*) malloc(r * sizeof(sem_t));
	
	// inicialización de semáforos
	// segundo parametro: compartido entre hilos (0), en lugar de procesos
	// tercer parametro: inicializados con el valor 0
	for (i=0; i<r; i++){
		for (j=0; j<r; j++){
			sem_init(&semaforos[i][j], 0, 0);
		}
	}
	// ------------------------- FIN BLOQUE AGREGADO -----------------------	
	
	#pragma omp parallel default(none) shared(semaforos) firstprivate(r,row_of_blocks_disp,num_of_bock_elems,D,P) num_threads(t)
	{
		INT64 i, j, k, b, kj, ik, kk, ij, k_row_disp, k_col_disp, i_row_disp, j_col_disp, w, aux;
		#ifndef NO_PATH
			int* tmp1 = NULL;
			int* tmp2 = NULL;
		#else
			int tmp1[BS], tmp2[BS];
			//tmp1 and tmp2 are used as a patch to avoid a compiler bug which makes it lose performance instead of winning while omitting the compute of the P patrix
		#endif
		for(k=0; k<r; k++){
			b = k*BS;
			k_row_disp = k*row_of_blocks_disp;
			k_col_disp = k*num_of_bock_elems;

			//Phase 1
			kk = k_row_disp + k_col_disp;
			FW_BLOCK_PARALLEL(D, kk, kk, kk, P, b, tmp1, tmp2);

			//Phase 2 y 3
			#pragma omp for schedule(dynamic) nowait
			for(w=0; w<r*2; w++){
				if(w<r){ //Phase 2
					j = w;
					if(j == k)
						continue;
					kj = k_row_disp + j*num_of_bock_elems;
					FW_BLOCK(D, kj, kk, kj, P, b, tmp1, tmp2);
					
						// -------------- BLOQUE AGREGADO -------------------

					// Finalizo el computo del bloque (k,j) = (k,w)
					// entonces puedo habilitar a todos los que están en mi misma columna
				/*	for (aux=0; aux< r; aux++)
						if(aux != k)
							sem_post(&semaforos[aux][j]);*/

					// -------------- FIN BLOQUE AGREGADO -----------------				
					
				} else { //Phase 3
					i = w - r;
					if(i == k)
						continue;
					ik = i*row_of_blocks_disp + k_col_disp;
					FW_BLOCK(D, ik, ik, kk, P, b, tmp1, tmp2);
					
						// -------------- BLOQUE AGREGADO -------------------

					// Finalizo el computo del bloque (i,k) = (k,w)
					// entonces puedo habilitar a todos los que están en mi misma fila
					for (aux=0; aux< r; aux++)
						if(aux != k)
							sem_post(&semaforos[i][aux]);

					// -------------- FIN BLOQUE AGREGADO -----------------									

					
				}
			}

			//Phase 4
			#pragma omp for collapse(2) schedule(dynamic)
			for(i=0; i<r; i++){
				for(j=0; j<r; j++){
					if( (j == k) || (i == k) )
						continue;
					
					// ----------- BLOQUE AGREGADO -----------------

					// Esperar que se compute el bloque (k,j)
					sem_wait(&semaforos[i][j]);

					// ---------- FIN BLOQUE AGREGADO --------------					
					
					
					i_row_disp = i*row_of_blocks_disp;
					ik = i_row_disp + k_col_disp;
					j_col_disp = j*num_of_bock_elems;
					kj = k_row_disp + j_col_disp;
					ij = i_row_disp + j_col_disp;
					FW_BLOCK(D, ij, ik, kj, P, b, tmp1, tmp2);
				}
			}
		}
	}
	
		for(k=0; k<r; k++)
			free(semaforos[k]);
		free(semaforos);
}
