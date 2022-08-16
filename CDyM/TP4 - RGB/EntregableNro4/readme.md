# Cronograma

- Jueves 7: seleccionar escala de color de cada LED (0 a 255) por interfaz USART
- Sábado 9: determinar la corriente máxima de cada LED
- Domingo 10: Utilice el potenciómetro (resistencia variable) del kit conectado al terminal ADC3 para controlar el brillo
o intensidad del color seleccionado vía interfaz serie. Investigar un poco antes de verlo en lunes en clase.
- Lunes 11: generar las 3 señales PWM de frecuencia 50 Hz o mayor, con resolución de 8 bits cada una


# Hecho
- Miércoles 6: seleccionar escala de color de cada LED por USART (+29 puntos)
- Sábado 9: corrientes máximas y control de LEDs con PWM + Timer 0 (+33 puntos)


# Validación
Los colores empiezan a verse "bien" a partir de estos valores:

- Amarillo: 30 30 0
- Celeste: 0 10 10


# Analog to Digital Converter
- Abreviado como ADC, tiene 10 bits de resolución -> Contador de 1024 valores
- ADC = Voltaje de entrada * 1024 / Voltaje de Referencia
- Video explicativo: https://youtu.be/te0SIBLJhPY
