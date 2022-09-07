# Práctica 2 - Capa de Red (I) - IPv4/IPv6

## Ej 10 - Sumarizar 
¿Cómo se podrían sumarizar las siguientes direcciones aplicando CIDR?
- 200.10.0.0/24
- 200.10.1.0/24
- 200.10.2.0/24
- 200.10.3.0/24

Vemos que las 4 redes son consecutivas, así que vemos cuales son los bits en común (desde el más significativo)

* Con 200.10.X.X tenemos 16 bits comunes 
* Luego de 0 a 3 tenemos formato 0000 00XX
* Resultan 22 bits comunes

Luego, verificamos que no haya ninguna red de más en el grupo. 
Todo bien. Se pasan los bits XX a cero y se especifica la nueva máscara. 

- Respuesta final: Si, es 200.10.0.0/22

## Ej 11
¿Cómo quedará la sumarización si necesita agrupar las 8 primeras redes (hasta las
200.10.7.0/24)

* De 0 a 7 tenemos formato 0000 0XXX
* Resultan 21 bits comunes

Nuevamente no hay redes de más.

* Respuesta: 200.10.0.0/21

## Ej 12
Y las siguientes:
- 195.80.0.0/24
- 195.80.1.0/24
- 195.80.2.0/24

* De 0 a 2 tenemos formato 0000 00XX
* Son 22 bits comunes, PERO estamos incluyendo a 195.80.3.0

Entonces no se puede sumarizar en 1 solo grupo. 

* De 0 a 1 tenemos formato 0000 000X
* Son 23 bits comunes para las primeras 2.

Sumarizo las primeras 2 y la tercera no puedo (verdad?)

* Respuesta: 195.80.0.0/23 para las 2 primeras

## Ej 20
Los mensajes ARP, ¿son re-enviados por los routers? Justifique.

* Respuesta: NO, porque sino se repartirán fuera de una red, inundando Internet??

## Ej 24
¿Cuál sería una abreviatura correcta de 3f80:0000:0000:0a00:0000:0000:0000:0845?
- 3f80::a00::845
- 3f80::a:845
- 3f80::a00:0:0:0:845:4567
- 3f80:0:0:a00::845
- 3f8:0:0:a00::845

* Respuesta: la cuarta
