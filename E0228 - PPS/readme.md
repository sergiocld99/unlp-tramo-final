# Dependencias

## Fase 1
El bloque (k,k) solo depende de sí mismo.

## Fase 2
Cada bloque de la fila (k, -) depende del bloque (k,k)

## Fase 3
Cada bloque de la columna (-, k) depende del bloque (k,k)

## Fase 4
El resto de bloques (a,b) depende de los bloques (a,k) y (k,b)


# Paralelización actual
La fase 1 se ejecuta primero. Luego las fases 2 y 3 se ejecutan en paralelo, y por último la fase 4 se ejecuta luego de que terminen las fases 2 y 3.


# ¿Qué se puede mejorar? (Parte 1)
La fase 4 podría arrancar sin esperar a que las fases 2 y 3 se ejecuten por completo. Para respetar las dependencias se puede hacer uso de semáforos, habiendo
uno para cada bloque de tamaño BSxBS. Entonces, para computar cada bloque (a,b) de la fase 4 se espera a que los semáforos de (a,k) y (k,b) estén en 1.
