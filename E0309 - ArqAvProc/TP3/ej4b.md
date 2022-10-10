![](img8.png)

## Emisión y finalización ordenada
Los resultados temporales se almacenan en un ROB.

    Issue       ADDF    ADD     MUL     Commit
    I1, I2
    I6, I4      I1      I2
    I5, I3      I1      I6      I4
                        I5      I3      I1, I2
                                        I6, I4
                                        I5, I3

Con el reordenamiento, ahora toma 6 ciclos. Las UF se utilizan un 78% del tiempo (ciclos 2-4).