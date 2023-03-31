# Optimización de algoritmos secuenciales
Para identar rápidamente en VS Code pulsar Ctrl+Shift+P y luego seleccionar "Format Document"

En los archivos dados es de 2 espacios.

## Ejercicios iguales que en SP
* Ej 1d: SP 9 (mmblk)
* Ej 1e: SP 10 (triangular, solo ML aunque habia que corregir cosas...)
* Ej 2: SP 1 (fib)
* Ej 4a: SP 3 (instrucciones)
* Ej 4b: SP 4 (instrucciones2)
* Ej 5a: SP 7 (iterstruc1.c e iterstruc2.c)

## Ejercicio 8 - Porcentaje
Acá sucede lo mismo que en el 5c). El tipo int de 32 bits permite representar hasta 2000 millones y pico.

El programa hecho multiplica el número total (n) por el porcentaje (p) y luego divide por 100. Al calcular el 50% de 50 millones, el producto n*p alcanza el valor 2500 millones, superando el máximo de i32 y quedando representado con el bit más significativo en 1. Por eso el programa devuelve algo negativo.

Solución: sabiendo que p es cero o positivo, cambiar su tipo int a ```unsigned int```.

## Enlaces externos
* Recopilación de resultados: https://docs.google.com/spreadsheets/d/1ImsogOEakPJx6q83Y0WZz4iTZGDssiQGkZDor5RmoaI/edit?usp=sharing
