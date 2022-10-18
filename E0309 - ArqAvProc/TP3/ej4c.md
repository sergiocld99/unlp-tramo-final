![](img8.png)

## Emisión y finalización desordenada
Plus renombrado de registros.

    R12b = R13a + R14a
    R1b = R8a + R9a
    R11b = R2a + R3a
    R5b = R6a * R7a
    R10b = R5b + R7a
    R4b = R2a + R3a

Ventana de ejecución:

    C   Decode      ADDF    ADD     MUL     Commit
    1   I1, I2
    2   I6, I4      I1      I2
    3   I5, I3      I1      I6      I4      I2
    4                       I5      I3      I1, I6
    5                                       I4, I5
    6                                       I3

Siguen siendo 6 ciclos de reloj. CPI = 1.