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
![figura 1](https://github.com/sergiocarp10/unlp-tramo-final/blob/main/I114%20-%20RD2/P1%20-%20Capa%20de%20Enlace/Screenshot%202022-08-30%20at%2022-04-25%20redes-II-ing-practica_1%20-%20redes-II-ing-practica_1.pdf.png)

- Si no se conoce la MAC destino (Babilonia), se realiza el protocolo ARP. Una vez realizado, cuando Alejandría manda el mensaje los switches que están en el camino (todos en este caso) agregarán la dirección MAC de Alejandría a su tabla, indicando la puerta por la que entró (no indicada en el dibujo).


## Ej 5
En Ethernet, después que se produce una colisión, ¿quién tiene prioridad para transmitir?
- No existen proridades en Ethernet (recordar algoritmo de Back-off)

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

## Ej 9
![ej 9](https://github.com/sergiocarp10/unlp-tramo-final/blob/main/I114%20-%20RD2/P1%20-%20Capa%20de%20Enlace/RD2-P1-Ej9.png)
- Los dominios de colisión nunca atraviesan un switch. Pueden incluir nodos, hubs o solo cables. En la figura tenemos 4.
- Al agregar un swich a la red, AUMENTA la cantidad de dominios de colisión.


## Ej 10
Si se establece una conexión inalámbrica entre dos notebooks directamente, ¿qué tipo de red wireless se ha establecido?
- IBSS, no requiere un AP intermediario.


## Ej 11
¿Qué datos de la trama Ethernet utiliza un switch para “forwardear” los frames?
- MAC destino


## Ej 12
En Ethernet, después que se produce una colisión, para poder volver a transmitir, una estación debe esperar...
- Un tiempo aleatorio, recordemos no hay prioridades (back-off)


## Ej 13
Definir dominio de broadcast. Para la figura 1 indicar la cantidad de los mismos.
- Grupo de nodos entre los cuales el mensaje de difusión (broadcast) de un nodo puede llegar a todos los demás nodos. NO ATRAVIESA ROUTERS.
- En la figura tenemos un solo dominio de broadcast.


## Ej 14
En una red Ethernet de tipo Full-Duplex, ¿es necesario ejecutar el mecanismo CSMA/CD?
- No, porque en Full-Duplex, al ser dos canales (uno para cada dirección) no existirá riesgo de colisión.


## Ej 15
La cantidad máxima de direcciones MAC que puede contener un frame de datos del estándar IEEE 802.11 es...
- Cuatro, tenemos las MAC origen y destino (de los nodos) y las MAC de un AP origen y destino (para Wifi, pueden estar o no)


## Ej 16
Al colocar un hub en una red, se tiene:
- Topología estrella, lógica bus, ya que la trama es repartida en simultáneo por los demás buses.


## Ej 17
¿Cuál es el dispositivo que reemplaza el hub en redes “cableadas”?
- Cable / bus compartido.


## Ej 18
¿Cuántas VLANs diferentes puede soportar el estándar IEEE 802.1q?
- Hasta 4096 (12 bits, en rango extendido), aunque varias están reservadas.


## Ej 19
Si se agrega un switch a una LAN, la cantidad de dominios de colisiones...
- Aumenta, ya que los corta.


## Ej 20
Si se agrega un switch a una LAN, la cantidad de dominios de broadcast...
- No aumenta, ya que solo los cortan los routers.


## Ej 21
Si a una LAN plana (plain) se le agrega un switch que tiene 8 ports y en cada port tiene una VLAN diferente, ¿cuál será la cantidad de dominios de broadcast de la misma?
- Serán 8 broadcast, ya que una vlan distinta define otro dominio de broadcast.


## Ej 22
Un link que permite que pasen más de una VLAN, se lo conoce como...
- Trunk


## Ej 23
¿Cuáles de los siguientes protocolos requieren el uso de mensajes de control de ACK?
* ✅ 802.11b
* ✅ 802.11n
* ❌ Ethernet - No, porque se espera a que se produzca una colisión.


## Ej 24
¿Qué mecanismo usa Ethernet y 802.11 para detectar errores?
- CRC
