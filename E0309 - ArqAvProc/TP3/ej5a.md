* Arquitectura de carga-almacenamiento
* Dispone de 5 UF, puede emitir en simultáneo:
    - 2 referencias a memoria
    - 2 operaciones de PF
    - 1 operación entera o 1 salto
* Operaciones con enteros: 1 ciclo
* Carga de un double: 2 ciclos
* Operaciones en PF: 3 ciclos (segmentado)

![](img9.png)

    C   MEM     MEM     PF      PF      INT
    1   LDD
    2   LDD
    3                   ADDF
    4                   ADDF
    5                   ADDF            SUB
    6   STD                             BGEZ

Una iteración se resuelve en 6 ciclos