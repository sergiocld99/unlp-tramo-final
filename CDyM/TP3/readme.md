# Consideraciones TP 3 - Arquitectura Background/Foreground, modelo Productor-Consumidor

- No se podrá usar PB0/PB1 como capturador de entrada. Utilizar en su lugar la interfaz E/S entre MCU y DHT11
- Los terminales para USART en parte coinciden con los usados en el LCD => No se pueden usar ambos (excluyentes).
- Cuidado con el ENTER, algunos terminales lo interpretan como \r\n, mientras otros solo uno de esos dos (en Br@y dejar activado para lo primero).
- Aceptar los comandos ON, OFF y RST obligatoriamente en MAYÚSCULA.
- Para volver a mostrar el menu se debe ingresar RST

## Configuración Terminal de Br@y
- Baud rate: 9600
- Data bits: 8
- Parity: none 
- Stop bits: 1 
- Handshaking: none

Si quiero guardar los datos recibidos pulsar "StartLog".

## Actualización cada 1 segundo
Se puede temporizar mediante un Timer.

## Sección crítica
Cuando tanto la tarea de background como la de foreground podrían escribir sobre una variable compartida (flag) a la vez, debo proteger el acceso a la variable mediante cli() y sei().

## Ejercicio del Motor (RPM)
T_MOTOR = 13,33 ms

1) Medición de RPM

(13,33 ms/pulso) x (25 pulsos/vuelta) x (1/1000 s/ms) x (1/60 min/s) = 5,55*10^-3 min/vuelta
invirtiendo: n = 180 vueltas/min = 180 rpm

2) ¿Hasta cuánto debe incrementarse el contador para que dé 13,33 ms?
Suponiendo F_IO = F_CPU = 16 MHz

T_pulso = PERIODO * T_IO => PERIODO = T_pulso / T_IO = T_pulso * F_IO = 213280
REQUIERE 32 BITS!!!

- Probemos con un preescaler de 8 -> F_IO = 2 MHz => PERIODO = 26660

3) ¿Como queda la formula final de rpm?
Suponiendo T_pulso medido en segundos

1/n = (T_pulso) x 25 / 60 
n = 60 / (25 x T_pulso) = 60 / (25 x PERIODO * T_IO) = 60 x F_IO / (25 x PERIODO)

- Para que la cuenta se haga correctamente, poner la cuenta de constantes como macro
