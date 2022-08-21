# Teoria 1 - Nivel de Red

El nivel de enlace refiere a que sólo hay medios físicos para la comunicación. En el nivel de red, los dispositivos (nodos) deben pertenecer a una misma RED.


Una comunicación puede ser:
* Con conexión: yo (A) solicito conectarme con el nodo B, existe un "protocolo", simil a una llamada telefónica.
* Sin conexión: yo (A) me limito a emitir el mensaje a B, sin avisarle y sin saber que hará con el mensaje, simil al envío de un correo.


En una red existe un protocolo de red, que indica de qué manera "A" estructura el mensaje (trama?) para que la red pueda enviárselo a B.


## Interfaces
* UNI (User Network Interface): entre un nodo y la red
* NNI (Node to Node Interface): entre los nodos de una misma red (interno).

Si dos redes tienen las mismas interfaces UNI y NNI podrán interconectarse mediante dispositivos Router. Es por ello que el Router tiene tantas puertas como redes a las que interconecta. Ojo: el router en sí mismo NO pertenece a una red, sólo las interconecta.

## Tipo de Red
* Abierta: se especifica para conocimiento público cómo es UNI y NNI.
* Cerrada: se conoce UNI, se cobran regalías por el uso de NNI.

## Modelo TCP/IP
Está compuesto por el protocolo IP y el protocolo TCP. NO EXISTE EL PROTOCOLO TCP/IP. Antes de IP, para cada aplicación/programa debía desarrollarse la manera de comunicarse según la tecnología usada (FTP, HTTP, etc.) Con la aparición del protocolo IP como intermediario, eso se elimina, existe una mayor versatilidad.

## Ethernet vs 802.3
La diferencia está en cómo se compone la trama. En el primero el campo "Protocol Type" indica el tipo de datagrama. En el segundo, es la longitud del campo de datos, ya que existe un relleno (de ceros) al mismo si es menor a la mínima longitud permitida.

## Ethernet: campos del Header
Ojo con el Frankestein de datos: bits, palabras y bytes. Amor y paz. Las palabras son de 32 bits.

### Palabra 1
* Versión: ocupa 4 bits, IPv4 (bits 0100) o IPv6 (bits 0110).
* Header Length: indica cuántas palabras de 32 bits ocupa en total el Header (esto es porque hay opciones que pueden indicarse o no).
* Total Length: longitud de la trama? en bytes.

### Palabra 2
* Identificación: es un número random de 16 bits (generado por un SO?)
* Flags y Fragment Offset: contienen información para poder rearmar el datagrama a partir de los fragmentos recibidos.

### Palabra 3
* Time to live (tiempo de vida): es la cantidad de saltos restantes que le quedan al mensaje hasta que deje de ser válido. Se decrementa cada vez que pasa por un Router, el cual lo descarta si alcanza el valor 0. Esto evita lazos (bucles) por si se producen cambios de camino.
* Header Checksum: es la suma de cada palabra de 16 bits del Header, vistas como números en CA1. Este campo se toma como 0 al momento de realizar la suma, y se utiliza únicamente para la detección de errores, mas no para la corrección.

## Palabra 6 ??
* IP Options: en bytes, se añade un padding si esta última palabra del header no llega a ocupar 32 bits.
