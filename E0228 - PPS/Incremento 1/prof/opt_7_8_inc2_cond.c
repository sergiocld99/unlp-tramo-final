#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/opt_0-n.c"
#include "../src/floyd_versions/common/malloc/aligned.c"

#include<pthread.h>

#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define UNROLL_FACTOR BS/(SIMD_WIDTH/TYPE_SIZE)

//Public
char* getFloydName(){
	return "with loop unroll + Inc_2-cond";
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
	pthread_cond_t *** conditions = (pthread_cond_t***) malloc(r * sizeof(pthread_cond_t **));
	for(i=0; i<r; i++) {
		conditions[i] = (pthread_cond_t**) malloc(r * sizeof(pthread_cond_t*));
		for(j=0; j<r; j++)
			conditions[i][j] = (pthread_cond_t*) malloc(r * sizeof(pthread_cond_t));
	}

	pthread_mutex_t *** mutexes = (pthread_mutex_t***) malloc(r * sizeof(pthread_mutex_t **));
	for(i=0; i<r; i++) {
		mutexes[i] = (pthread_mutex_t**) malloc(r * sizeof(pthread_mutex_t*));
		for(j=0; j<r; j++)
			mutexes[i][j] = (pthread_mutex_t*) malloc(r * sizeof(pthread_mutex_t));
	}

	char *** pending = (char***) malloc(r * sizeof(char **));
	for(i=0; i<r; i++) {
		pending[i] = (char**) malloc(r * sizeof(char*));
		for(j=0; j<r; j++)
			pending[i][j] = (char*) malloc(r * sizeof(char));
	}

	
	// inicialización de conditions y mutexes
	for (i=0; i<r; i++){
		for (j=0; j<r; j++){
			for (k=0; k<r; k++){
				pthread_cond_init(&conditions[i][j][k], NULL);
				pthread_mutex_init(&mutexes[i][j][k], NULL);
			}
		}
	}
	
	// inicialización para ronda 1 (solo dependencias del algoritmo)
	for (i=0; i<r; i++)
		for (j=0; j<r; j++)
			pending[0][i][j] = ((i == 0 || j == 0) ? 0 : 2);	
	// inicializacion para rondas restantes (dependencias del algoritmo + propio bloque en ronda previa)
	for (k=1; k<r; k++)	
		for (i=0; i<r; i++)
			for (j=0; j<r; j++)
				pending[k][i][j] = ((i == k || j == k) ? 1 : 3);	

	// ------------------------- FIN BLOQUE AGREGADO -----------------------	
	
	#pragma omp parallel default(none) shared(conditions,mutexes,pending) firstprivate(r,row_of_blocks_disp,num_of_bock_elems,D,P) num_threads(t)
	{
		INT64 i, j, k, b, kj, ik, kk, ij, k_row_disp, k_col_disp, i_row_disp, j_col_disp, w, aux;
		TYPE * p2_buffer, *p3_buffer;
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
			
			// ----------- BLOQUE AGREGADO -----------------

			// Esperar que se compute el bloque (k,k)
			pthread_mutex_lock (&mutexes[k][k][k]); 
			while (pending[k][k][k] > 0)
				pthread_cond_wait(&conditions[k][k][k],&mutexes[k][k][k]);
			pthread_mutex_unlock (&mutexes[k][k][k]); 
			// ---------- FIN BLOQUE AGREGADO --------------				

			//Phase 1
			kk = k_row_disp + k_col_disp;
			FW_BLOCK_PARALLEL(D, kk, kk, kk, P, b, tmp1, tmp2);
			
			// ----------- BLOQUE AGREGADO -----------------
			#pragma omp single nowait
			{
				if (k < r-1) {
					pthread_mutex_lock (&mutexes[k+1][k][k]); 
					pending[k+1][k][k]--;
					pthread_mutex_unlock (&mutexes[k+1][k][k]); 
					pthread_cond_signal(&conditions[k+1][k][k]);
				}
			}
			// ---------- FIN BLOQUE AGREGADO --------------				

			//Phase 2 y 3
			#pragma omp for schedule(dynamic) nowait
			for(w=0; w<r*2; w++){
				if(w<r){ //Phase 2
					j = w;
					if(j == k)
						continue;
					kj = k_row_disp + j*num_of_bock_elems;
					
					// ----------- BLOQUE AGREGADO -----------------

					// Esperar que se compute el bloque (k,j) de la ronda previa
					pthread_mutex_lock (&mutexes[k][k][j]); 
					while (pending[k][k][j] > 0)
						pthread_cond_wait(&conditions[k][k][j],&mutexes[k][k][j]);
					pthread_mutex_unlock (&mutexes[k][k][j]); 
					// ---------- FIN BLOQUE AGREGADO --------------	
					
					
					FW_BLOCK(D, kj, kk, kj, P, b, tmp1, tmp2);
					
						// -------------- BLOQUE AGREGADO -------------------
					for (aux=0; aux< r; aux++)
						if(aux != k) {
							pthread_mutex_lock (&mutexes[k][aux][j]); 
							pending[k][aux][j]--;
							pthread_mutex_unlock (&mutexes[k][aux][j]); 
							pthread_cond_signal(&conditions[k][aux][j]);
						}
						
					if (k < r-1) {
						pthread_mutex_lock (&mutexes[k+1][k][j]); 
						pending[k+1][k][j]--;
						pthread_mutex_unlock (&mutexes[k+1][k][j]); 
						pthread_cond_signal(&conditions[k+1][k][j]);
					}						

					// -------------- FIN BLOQUE AGREGADO -----------------				
					
				} else { //Phase 3
					i = w - r;
					if(i == k)
						continue;
					ik = i*row_of_blocks_disp + k_col_disp;
					
					// ----------- BLOQUE AGREGADO -----------------

					// Esperar que se compute el bloque (k,j) de la ronda previa
					pthread_mutex_lock (&mutexes[k][i][k]); 
					while (pending[k][i][k] > 0)
						pthread_cond_wait(&conditions[k][i][k],&mutexes[k][i][k]);
					pthread_mutex_unlock (&mutexes[k][i][k]); 
					// ---------- FIN BLOQUE AGREGADO --------------						
					
					FW_BLOCK(D, ik, ik, kk, P, b, tmp1, tmp2); // , D_p2_mirror[k], D_p3_mirror[k], P_p2_mirror[k], P_p3_mirror[k]);
					

					// Finalizo el computo del bloque (i,k) = (k,w)
					// entonces puedo habilitar a todos los que están en mi misma fila
					for (aux=0; aux< r; aux++)
						if(aux != k) {
							pthread_mutex_lock (&mutexes[k][i][aux]); 
							pending[k][i][aux]--;
							pthread_mutex_unlock (&mutexes[k][i][aux]); 
							pthread_cond_signal(&conditions[k][i][aux]);
						}
						
					if (k < r-1) {
						pthread_mutex_lock (&mutexes[k+1][i][k]); 
						pending[k+1][i][k]--;
						pthread_mutex_unlock (&mutexes[k+1][i][k]); 
						pthread_cond_signal(&conditions[k+1][i][k]);
					}						
						
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
			//		pthread_cond_wait(&conditions[i][j],&mutexes[i][j]);
					// Esperar que se compute el bloque (i,k)
					pthread_mutex_lock (&mutexes[k][i][j]); 
					while (pending[k][i][j] > 0)
						pthread_cond_wait(&conditions[k][i][j],&mutexes[k][i][j]);
					pthread_mutex_unlock (&mutexes[k][i][j]); 

					// ---------- FIN BLOQUE AGREGADO --------------					
					
					
					i_row_disp = i*row_of_blocks_disp;
					ik = i_row_disp + k_col_disp;
					j_col_disp = j*num_of_bock_elems;
					kj = k_row_disp + j_col_disp;
					ij = i_row_disp + j_col_disp;
					FW_BLOCK(D, ij, ik, kj, P, b, tmp1, tmp2);
					
					// ----------- BLOQUE AGREGADO -----------------
					
					if (k < r-1) {
						pthread_mutex_lock (&mutexes[k+1][i][j]); 
						pending[k+1][i][j]--;
						pthread_mutex_unlock (&mutexes[k+1][i][j]); 
						pthread_cond_broadcast(&conditions[k+1][i][j]);
					}		

					// ---------- FIN BLOQUE AGREGADO --------------										
				}
			}
		}
	}
	
		for(k=0; k<r; k++)
			free(conditions[k]);
		free(conditions);
}
