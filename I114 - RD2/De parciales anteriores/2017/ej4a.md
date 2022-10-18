## Sumarización y direccionamiento

El conjunto de direcciones IP de 29.18.0.0 a 19.18.127.255 se ha agregado/sumarizado a 29.18.0.0/17. 

Sin embargo, hay un hueco de 1024 direcciones
sin asignar de 29.18.60.0 a 29.18.63.255 que de repente se asignan a un host
que utiliza una línea de salida diferente. 

Ahora es necesario dividir la dirección
agregada en sus bloques constituyentes, agregar el nuevo bloque a la tabla y,
después, ver si es posible alguna reagregación. 

Si no lo es, ¿qué se puede hacer
en lugar de eso?

### Solución
El hueco estaba compuesto por 4 x 256 direcciones, que se pueden sumarizar con /22. Justamente, 29.18.60.0/22 es ese bloque (60 es múltiplo de 4), sin más ni menos direcciones.

Con simplemente agregar 29.18.60.0/22 a la tabla de ruteo, todos los paquetes que tengan dirección destino a alguna perteneciente a ese bloque serán encaminados para la nueva red, ya que la máscara /22 es "más específica (coincidente)" que /17.