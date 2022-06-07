# Consideraciones TP 3

- No se podrá usar PB0/PB1 como capturador de entrada. Utilizar en su lugar la interfaz E/S entre MCU y DHT11

## Ejercicio del Motor (RPM)
T_MOTOR = 13,33 ms
Contador = 16726

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
