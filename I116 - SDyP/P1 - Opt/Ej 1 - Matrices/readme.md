## Índices Triangulares como Vector
Partimos de las cuentas mostradas en las filminas (clase 2) y las sintetizamos:

* L por filas: ```L[i,j] = L[i*N+j - (i*N-i*(i+1)/2) ] = L[j+i*(i+1)/2] = L[SUMATORIA(i)+j]``` (correcto)
* L por columnas: ```L[i,j] = L[i+j*N - (j*(j+1)/2) ] = L[j*N - SUMATORIA(j) + i]```
* U por columnas: ```U[i,j] = U[i+j*N- (j*N-SUMATORIA(j)) ] = U[SUMATORIA(j)+i]```
* U por filas: ```U[i,j] = U[i*N+j - SUMATORIA(i) ]```

## Matrices N x N
Demostración que el ordenamiento por columnas del segundo factor (matriz) importa:

![link](./P1-E1-C.png)
