## Compilar
Acá extraje la función dwalltime() a un archivo común "tiempo.c", por lo que se debe compilar todo junto así:

```gcc e2.c tiempo.c -o e2```

En el caso de la versión paralela, también se extrajeron los loops de pthread_create y join. Las implementaciones que siguen la lógica de Merge calculan logaritmos de base 2, entonces requiere el flag -lm al final.

```gcc -pthread e2.c tiempo.c hilos.c -o e2 -lm```

## Ejecutar
De la siguiente forma: ```./e2 N X [T]```

## Resultados
Ambas versiones (usando mutex vs vector de contadores) arrojan un rendimiento similar.
