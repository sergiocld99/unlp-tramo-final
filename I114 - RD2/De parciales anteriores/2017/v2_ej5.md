# Tipo de dirección, host, broadcast, red

- 192.168.37.192/25:
    - La red es 192.168.37.128/25
    - Broadcast: 192.168.37.255/25
    - Host: 192 - 128 = 64 de 126.
    - Dirección PRIVADA (192.168.x.x) de clase C.

* 72.17.16.255/23:
    - Red: 72.17.16.0/23
    - Broadcast: 72.17.17.255/23
    - Host: 255 - 0 = 255 de 510.
    - Dirección: PÚBLICA de Clase B (010...) ❌
        - Corrijo, es Clase A (B es desde 128)

- 10.0.8.0/22:
    - Red: 10.0.8.0/22
    - Broadcast: 10.0.15.0/22
    - Host: NO, es la dirección de red.
    - Dirección: PRIVADA de Clase A (10.x.x.x)
