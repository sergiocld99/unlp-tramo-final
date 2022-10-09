# Arquitecturas Avanzadas de Procesadores
Bibliografía para Módulo 1: http://acs.pub.ro/~cpop/SMPA/Computer%20Architecture%20A%20Quantitative%20Approach%20(5th%20edition).pdf

## Superescalares
Por lo general, asumimos que el Forwarding está implementado. No es necesario que una instrucción I2 con dependencia verdadera de I1 espere a que I2 finalice para pasar I1 a ejecución. Directamente, en el mismo ciclo que I2 accede a Commit, I1 pasa a ejecución si la unidad funcional tiene espacio.

### Finalización ordenada
Cuando una instrucción finaliza su ejecución y debe terminar, respetando el orden del programa, espera en la parte de Commit (si hay espacio), sino se queda ocupando el recurso hasta poder entrar a Commit.
