# Operaciones matemáticas

## Sumas vs multiplicaciones
Si multiplicás potencias de 2 es más rápido que andar sumando (también potencias de 2) porque el compilador detecta que lo primero alcanza con hacer mediante desplazamiento de bits (fuente: ayudante).

## Operación Resto en cada elemento del vector
En este caso se limita a usar un divisor potencia de 2. 

La primera versión usa el operador "%" que realiza una división para calcularlo. 

La segunda versión usa el operador "&" para aplicar una máscara de bits, que es mucho más rápido, como se observa a continuación.

![](./P1-E4-C.png)
