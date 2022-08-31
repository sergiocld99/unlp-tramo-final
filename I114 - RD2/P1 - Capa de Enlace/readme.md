# Práctica 1 - Repaso / Capa de Enlace

## Ej 1 - Modelo OSI y TCP/IP
Según el modelo OSI, cuál es el orden de las capas desde abajo hacia arriba. Comparar el diagrama de modelo OSI con el modelo TCP/IP:

![osi vs tcp](https://sites.google.com/site/trabajoredeslocalesesther/_/rsrc/1452844553162/transmision-de-datos-en-las-redes/comparativa-osi-frente-tcp-ip/ositcp01.png)


## Ej 2 - Direcciones MAC
De los siguientes dispositivos indique cuáles requieren tener direcciones MAC, cuántas y cuáles serían las condiciones.

* ✅ Bridge/Switch (si es administrable, requiere 1 MAC para controlarlo)
* ✅ Laptop con una NIC Wifi y otra Ethernet (2 MAC, una para cada tipo)
* ✅ PC
* ✅ Smartphone
* ✅ Bridge/Hub con administración remota

¿Cuántas direcciones MAC tiene un frame/trama Ethernet?
- Dos: origen y destino.

¿Cuál de las siguientes direcciones MACs son válidas? ¿Cuales podemos encontrar en el campo origen de un/una frame/trama MAC?

- Pista: El origen siempre tendrá una dirección unicast.
- Pista: La dirección MAC multicast es un valor especial que comienza con 01-00-5E en hexadecimal

* ✅ 00:1e:2d:00:98:1a
* ✅ 01:00:5E:00:54:12 - Solo destino (multicast)
* ❌ fe:ga:87:12:64:3a (tiene que ser en hexa, la g no pertenece)
* ❌ ad:e6:b7:42:ef (está incompleta, le faltan 2 números hex)
* ✅ 01:80:C2:00:33:10
* ✅ de:9f:ae:65:08:47
* ✅ ff:ff:ff:ff:ff:ff - Solo destino (broadcast)


## Ej 3
Para construir su tabla MAC, qué datos de la trama Ethernet mira el switch:
- MAC origen

## Ej 4
- Si no se conoce la MAC destino (Babilonia), se realiza el protocolo ARP. Una vez realizado, cuando Alejandría manda el mensaje los switches que están en el camino (todos en este caso) agregarán la dirección MAC de Alejandría a su tabla, indicando la puerta por la que entró (no indicada en el dibujo).


## Ej 5
En Ethernet, después que se produce una colisión, ¿quién tiene prioridad para transmitir?
- No existen proridades en Ethernet (recordar algoritmo de Backoff)

## Ej 6
¿Cuáles de las siguientes tecnologías trabajan con CSMA/CD?

![csma cd](https://upload.wikimedia.org/wikipedia/commons/7/7f/Csma_cd.jpg)

- Pista: CSMA/CA adapta los procedimientos de gestión de colisiones CSMA/CD utilizados en las redes semidúplex de Ethernet a las necesidades de las redes inalámbricas.

* ✅ 10Base2 - Fuente: Wikipedia en inglés
* ❌ 10BaseT, 100BaseT - No, porque son Half Duplex.
* ❌ 10GBaseLR, 100GBASE-LR4 - No, porque son Full Duplex.
* ❌ 802.15.4, 802.11n - No, usa otro método (¿CSMA/CA?)


## Ej 7
Un trama multicast va dirigida a...
- A un conjunto de destinos en la red.


## Ej 8
Al recibir una trama de tipo broadcast o multicast, ¿qué hace el switch?
- La reenvía por todos los puertos menos por el que la recibió.
