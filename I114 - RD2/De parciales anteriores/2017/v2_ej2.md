# Red 147.83.2.0/24

- 2 áreas con conectividad a Internet.
    - Gestión: 23 hosts
    - Desarrollo: 62 hosts

<b>Aclaración: en ambas redes se requieren una cantidad de direcciones igual a número de máquinas + puerta router + puerta ISP</b>

<b>Se pueden asignar hasta 256 direcciones</b>

Para 62 hosts, requiero log(64) = 6 bits de nodos, máscara /26.

    Subred 147.83.2.X/26
    donde X = XX00 0000     <-- resolución 64

Puedo asignar 147.83.2.64/24 a desarrollo. Aunque ahí tengo justo 62 direcciones, y también necesito identificar al router, por lo que mejor designaría 147.83.2.128/25, con 126 direcciones. ✔️

Para 23 hosts, requiero log(32) = 5 bits de nodos, máscara /27. ✔️

    Subred 147.83.2.X/27
    donde X = XXX0 0000     <-- resolución 32 (ojo)

Puedo asignar 147.83.2.96/27 a gestión (para que quede contiguo), que tiene 30 direcciones disponibles para usar.

- Me quedó entonces 147.83.2.96 a 147.83.2.127 para gestión, y de 147.83.2.128 a 147.83.2.255 para desarrollo.

Si está permitido usar la subred 0 se podría destinar la subred mayor de 147.83.2.0 a 147.83.2.127 (desarrollo) y luego a continuación de 147.83.2.128 a 147.83.2.159 (gestión).