- Total Length = 24
- More fragment = 0
- Offset = 173

Indique cual es el tamaño original del payload(campo de datos) del datagrama enviado.

Fragment Offset está expresado en palabras de 64 bits, entonces el datagrama contiene los datos desde el bit 173 x 8B = 1384 B.

Total Length indica el tamaño total del datagrama IP actual en bytes, en este caso, 24. Sin embargo, de esos 24 bytes, hay que restar 20 bytes (160 bits) del tamaño mínimo de header. Entonces la carga útil de este último datagrama es 4 bytes.

Luego, como dice que el fragmento recibido es el último (flag MF = 0), ya podemos calcular el payload original, que resulta 1388 B. ✔️