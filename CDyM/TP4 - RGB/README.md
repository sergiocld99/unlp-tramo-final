# cdym-tp4-pwm
Generar colores de un LED RGB por USART

Led rojo a PB5, verde a PB2, azul a PB1
Resistencias de 220

Escribir un 1 en un pin -> No hay caida de tension -> APAGADOS

Para variar la intensidad de cada uno, usar señales PWM (PB1 y PB2 son OC1 y OC2 de Timer)

	- En el caso de PB5 hay que hacerlo manualmente temporizado por SOFTWARE.
	- PWM de 8 bits -> Hay 256 valores posibles de ciclo de trabajo -> 256 intensidades distintas para cada led
		- Total de colores: 256^3
	- Usamos una frecuencia de al menos 50 Hz para que el ojo vea el valor medio de la intensidad.

- Cuidado: convendría usar modo invertido para evitar confusiones.

Voltajes de activación (mínimos) -> son distintos, la corriente tmb!

	- Red: 		2.0490 V		Ir = (5-2.0490) V / 220 Ohm
	- Green: 	2.8100 V
	- Blue: 	2.8710 V

En la terminal debo poder ingresar el valor de intensidad para rojo, verde y azul. Hacer algo simple.

Encender los 3 a la misma intensidad -> Color blanco
