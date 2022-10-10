El siguiente código tiene múltiples tipos de dependencias. Localizarlas, clasificarlas e indicar cuáles pueden ser eliminadas por medio de la técnica de renombrado de registros.

![](img5.png)

- Dependencias verdaderas (RAW): hacia adelante
    - Entre I1 e I3: operando Y
    - Entre I1 e I4: operando Y

* Antidependencia (WAR): hacia atrás
    - Entre I1 e I2: I2 escribe antes que I1 lea X
    - Entre I3 e I4: I4 escribe antes que I3 lea Y

- Dependencias de salida (WAW): escrituras.
    - Entre I1 e I4: I4 escribe antes que I1.

## Renombrado de registros

    Y2 = X1 / c
    X2 = X1 + c
    Z2 = Y2 + c
    Y3 = c - Y2

Podemos observar que se eliminan las antidependencias y las dependencias de salida, pero siguen existiendo dependencias verdaderas.