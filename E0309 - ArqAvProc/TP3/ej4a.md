![](img7.png)

* Puede captar, decodificar y emitir dos instrucciones simultáneamente en un ciclo de reloj.

* Puede finalizar (commit) dos instrucciones simultáneamente.

* Dispone de las siguientes UF:
    * Un sumador PF (2 ciclos)
    * Un sumador entero (1 ciclo)
    * Un multiplicador entero (1 ciclo)

## Emisión y finalización ordenada
No se pueden mandar a ejecutar nuevas instrucciones hasta que las anteriores hayan terminado su etapa Execute, para garantizar una finalización ordenada.

    C   Decode      ADDF    ADD     MUL     Commit
    1   I1, I2
    2   I3, I4      I1      I2
    3   I3, I4      I1              
    4       14                      I3      I1, I2
    5   I5, I6                      I4      I3
    6       I6              I5              I4
    7                       I6              I5
    8                                       I6

La ejecución toma 8 ciclos. Las UF se utilizan durante solo un 38% del tiempo (ciclos 2-7).
