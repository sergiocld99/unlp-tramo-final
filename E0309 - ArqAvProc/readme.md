# Arquitecturas Avanzadas de Procesadores
Bibliografía para Módulo 1: http://acs.pub.ro/~cpop/SMPA/Computer%20Architecture%20A%20Quantitative%20Approach%20(5th%20edition).pdf

## Superescalares
Por lo general, asumimos que el Forwarding está implementado. No es necesario que una instrucción I2 con dependencia verdadera de I1 espere a que I2 finalice para pasar I1 a ejecución. Directamente, en el mismo ciclo que I2 accede a Commit, I1 pasa a ejecución si la unidad funcional tiene espacio.

## VLIW

- Trace Sheduling: el compilador predice la rama más probable ante un if/else, para buscar instrucciones paralelizables, reordenarlas y empaquetarlas en una instrucción larga más allá del condicional. Sin embargo, si la predicción falla, se dispone de un código de <b>compensación</b> para revertir los cambios que se llegaron a realizar por la rama incorrecta.

  - Diferencia con ejecución especulativa: ésta última es por hardware y utiliza ROB para garantizar finalización ordenada y almacenar resultados temporales antes de conocerse el resultado de la condición. Si falló la predicción, vacía el ROB.
  
  - Diferencia con branch predication: en Itanium las instrucciones se utiliza ejecución con predicado, que permite ejecutar ambas ramas pero no realiza el commit de las instrucciones de la rama correcta hasta conocer la condición, momento en el cual el predicado se vuelve 1.
