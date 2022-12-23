#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/opt_0-n.c"
#include "../src/floyd_versions/common/malloc/aligned.c"

#include<pthread.h>

#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define UNROLL_FACTOR BS/(SIMD_WIDTH/TYPE_SIZE)

//Public
char* getFloydName(){
	return "opt_7_8 + Inc_1-cond + fused loops";
}

//Public
char* getFloydVersion(){
	return "Opt-12";
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
//			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
	//		#endif
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
	//		#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
	//		#endif
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
	pthread_cond_t ** conditions = (pthread_cond_t**) malloc(r * sizeof(pthread_cond_t *));
	for(k=0; k<r; k++)
		conditions[k] = (pthread_cond_t*) malloc(r * sizeof(pthread_cond_t));
	
	pthread_mutex_t ** mutexes = (pthread_mutex_t**) malloc(r * sizeof(pthread_mutex_t *));
	for(k=0; k<r; k++)
		mutexes[k] = (pthread_mutex_t*) malloc(r * sizeof(pthread_mutex_t));	

	char ** pending = (char**) malloc(r * sizeof(char *));
	for(k=0; k<r; k++)
		pending[k] = (char*) malloc(r * sizeof(char));	

	
	// inicialización de conditions y mutexes
	for (i=0; i<r; i++){
		for (j=0; j<r; j++){
			pthread_cond_init(&conditions[i][j], NULL);
			pthread_mutex_init(&mutexes[i][j], NULL);
		}
	}
	// ------------------------- FIN BLOQUE AGREGADO -----------------------	
	
	#pragma omp parallel default(none) shared(conditions,mutexes,pending) firstprivate(r,row_of_blocks_disp,num_of_bock_elems,D,P) num_threads(t)
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
			
									// -------------- BLOQUE AGREGADO -------------------
			#pragma omp for collapse(2) schedule(static)
			for (i=0; i<r; i++)
				for (j=0; j<r; j++)
					//pending[i][j] = ((i == k) || (j == k) ? 1: 2);
					pending[i][j] = 2;				
			
								// -------------- FIN BLOQUE AGREGADO -----------------				

			
			//Phase 2, 3 and 4
			#pragma omp for schedule(dynamic) 
			for(w=0; w<r*r-1; w++){
				if(w<r-1){ //Phase 2
					j = w;
					if(j >= k) j++;
					kj = k_row_disp + j*num_of_bock_elems;
					
					//printf ("\nRONDA %d - FASE 2 - (%d,%d)",k,k,j);
					
					FW_BLOCK(D, kj, kk, kj, P, b, tmp1, tmp2);
					
						// -------------- BLOQUE AGREGADO -------------------

					// Finalizo el computo del bloque (k,j) = (k,w)
					// entonces puedo habilitar a todos los que están en mi misma columna
									// Finalizo el computo del bloque (i,k) = (k,w)
					// entonces puedo habilitar a todos los que están en mi misma fila
					for (aux=0; aux< r; aux++)
						if(aux != k) {
							pthread_mutex_lock (&mutexes[aux][j]); 
							pending[aux][j]--;
							pthread_mutex_unlock (&mutexes[aux][j]); 
							pthread_cond_signal(&conditions[aux][j]);
						}

					// -------------- FIN BLOQUE AGREGADO -----------------				
					
				} else { 
					if (w < 2*r -2) {//Phase 3
						i = w - (r -1);
						if(i >= k) i++;

						ik = i*row_of_blocks_disp + k_col_disp;
						FW_BLOCK(D, ik, ik, kk, P, b, tmp1, tmp2);
					
						//				printf ("\nRONDA %d - FASE 3 - (%d,%d)",k,i,k);
						
							// -------------- BLOQUE AGREGADO -------------------

						// Finalizo el computo del bloque (i,k) = (k,w)
						// entonces puedo habilitar a todos los que están en mi misma fila
						for (aux=0; aux< r; aux++)
							if(aux != k) {
								pthread_mutex_lock (&mutexes[i][aux]); 
								pending[i][aux]--;
								pthread_mutex_unlock (&mutexes[i][aux]); 
								pthread_cond_signal(&conditions[i][aux]);
							}
						// -------------- FIN BLOQUE AGREGADO -----------------									
					}
					else { // Phase 4
						
						aux = (w - (2*r -2));
						i = aux / (r-1);
						j = aux % (r-1);
						if(i >= k) i++;						
						if(j >= k) j++;		

					//printf ("\nRONDA %d - FASE 4 - (%d,%d)",k,i,j);

						// ----------- BLOQUE AGREGADO -----------------

						// Esperar que se compute el bloque (k,j)
				//		pthread_cond_wait(&conditions[i][j],&mutexes[i][j]);
						// Esperar que se compute el bloque (i,k)
						pthread_mutex_lock (&mutexes[i][j]); 
						while (pending[i][j] > 0)
							pthread_cond_wait(&conditions[i][j],&mutexes[i][j]);
						pthread_mutex_unlock (&mutexes[i][j]); 

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
	}
	
		for(k=0; k<r; k++)
			free(conditions[k]);
		free(conditions);
}
