# ARM

## Características importadas de RISC
- Arquitectura Load/Store
- Instrucciones de ancho fijo: 32 bits.
- Formato de instrucción de 3 direcciones.

## Características rechazadas (p. 48)
- Ventana de registros: en RISC se tienen muchos registros, de los cuales 32 son visibles en todo momento. Las llamadas y salidas de procedimientos mueven la ventana visible, para reducir el movimiento de datos entre procesador y memoria debido a context switch. Sin embargo, esto ocupa un gran tamaño en el chip y aumenta costos.

- Salto retardado: en RISC se aprovecha el delay slot para ejecutar una instrucción útil si es posible. Aunque funcione bien en procesadores segmentados, no tiene buena escalabilidad en superescalares, y puede interferir de manera indeseable en los mecanismos de predicción. Además dificulta el manejo de excepciones preciso.

- Ejecución de un ciclo: en ARM, algunas instrucciones pueden requerir más de 1 ciclo. Para garantizarlo, en RISC se separa la memoria de datos e instrucciones. Sin embargo, esto no se realiza en ARM debido a los costos que conlleva.