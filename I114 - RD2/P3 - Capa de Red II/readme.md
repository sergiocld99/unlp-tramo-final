# Práctica 3- Capa de Red (II)
- Temas: VLSM, Fragmentación, NAT

## Ej 1 - A
Dada la red IP 65.0.0.0/24 se necesitan definir:
* 1 (una) red de 80 hosts -> Llamemos "A"
* 2 (dos) redes de 10 hosts -> Llamemos "B" y "D"
* 1 (una) red de 40 hosts -> Llamemos "C"

Tenemos 8 bits disponibles, empecemos por la red de mayor cantidad de hosts: 80.

Para tener 80 hosts, necesitamos 7 bits (/25), ya que 2^7 = 128.
* 65.0.0.0/25: red A (0XXX XXXX)
* 65.0.0.128/25: libre (1XXX XXXX)

Para tener 40 hosts, necesitamos 6 bits (/26), ya que 2^6 = 64
* 65.0.0.128/26: red C (10XX XXXX)
* 65.0.0.192/26: libre (11XX XXXX)

Si tomamos otro bit:
* 65.0.0.192/27: libre (110X XXXX)
* 65.0.0.224/27: libre (111X XXXX)

Para tener 10 hosts, necesitamos 4 bits (/28), ya que 2^4 = 16
* 65.0.0.192/28: red B (1100 XXXX)
* 65.0.0.208/28: red D (1101 XXXX)
* 65.0.0.224/28: libre (1110 XXXX)
* 65.0.0.240/28: libre (1111 XXXX)

Entonces resultaron las siguientes redes definidas:
- Red A: 65.0.0.0/25
- Red B: 65.0.0.192/28
- Red D: 65.0.0.208/28
- Red C: 65.0.0.128/26

## Ej 1 - B [VISADO]
Dada la red IP 100.0.0.0/16 se necesitan definir:
* 2 (dos) redes de 2000 hosts -> Llamemos redes "A" y "B"
* 2 (dos) redes de 500 hosts -> Llamemos redes "C" y "D"
* 20 (veinte) redes de 300 hosts -> Llamemos redes "E1" a "E20"
* 50 (cincuenta) redes de 200 hosts -> redes "F1" a "F50"
* Una red de backbone para unir cada uno de los router de las redes anteriores (74 direcciones) -> red "G"

Para 2000 hosts -> 11 bits (/21) para 2^5 = 32 redes
* 100.0.0.0/21: red A (0000 0XXX XXXX XXXX)
* 100.0.8.0/21: red B (0000 1XXX XXXX XXXX)
* 100.0.16.0/21: libre (0001 0XXX XXXX XXXX) [para C y D]
* 100.0.24.0/21: libre (0001 1XXX XXXX XXXX) [para E1...]
* 100.0.32.0/21: libre (0010 0XXX XXXX XXXX) [para E5...]
* 100.0.40.0/21: libre (0010 1XXX XXXX XXXX) [para E9...]
* 100.0.48.0/21: libre (0011 0XXX XXXX XXXX) [para E13...]
* 100.0.56.0/21: libre (0011 1XXX XXXX XXXX) [para E17...]
* 100.0.64.0/21: libre (0100 0XXX XXXX XXXX) [para F1...]
* 100.0.72.0/21: libre (0100 1XXX XXXX XXXX) [para F9...]
* 100.0.80.0/21: libre (0101 0XXX XXXX XXXX) [para F17...]
* 100.0.88.0/21: libre (0101 1XXX XXXX XXXX) [para F25...]
* 100.0.96.0/21: libre (0110 0XXX XXXX XXXX) [para F33...]
* 100.0.104.0/21: libre (0110 1XXX XXXX XXXX) [para F41...]
* 100.0.112.0/21: libre (0111 0XXX XXXX XXXX) [para F49 y F50]
* y asi... hasta 100.0.248.0/21 (16 redes libres)

Para 500 hosts -> 9 bits (/23) para 2^2 = 4 redes
* 100.0.16.0/23: red C (0001 000X XXXX XXXX)
* 100.0.16.2/23: red D (0001 001X XXXX XXXX)
* 100.0.16.4/23: libre (0001 010X XXXX XXXX) [510 dir]
* 100.0.16.6/23: libre (0001 011X XXXX XXXX) [510 dir]

Para 300 hosts -> 9 bits (/23). Haciendo subnetting desde /21, cada subred tiene 4 redes E, necesitando ocupar 5 redes /21 (24, 32, 40, 48 y 56)

* 100.0.24.0/23: red E1 (0001 100X XXXX XXXX)
* 100.0.24.2/23: red E2 (0001 101X XXXX XXXX)
* 100.0.24.4/23: red E3 (0001 110X XXXX XXXX)
* 100.0.24.6/23: red E4 (0001 111X XXXX XXXX)
* 100.0.32.0/23: red E5 (0010 000X XXXX XXXX)
* 100.0.32.2/23: red E6 (0010 001X XXXX XXXX)
* 100.0.32.4/23: red E7 (0010 010X XXXX XXXX)
* 100.0.32.6/23: red E8 (0010 011X XXXX XXXX)
* 100.0.40.0/23: red E9 (0010 100X XXXX XXXX)
* 100.0.40.2/23: red E10 (0010 101X XXXX XXXX)
* 100.0.40.4/23: red E11 (0010 110X XXXX XXXX)
* 100.0.40.6/23: red E12 (0010 111X XXXX XXXX)
* 100.0.48.0/23: red E13 (0011 000X XXXX XXXX)
* 100.0.48.2/23: red E14 (0011 001X XXXX XXXX)
* 100.0.48.4/23: red E15 (0011 010X XXXX XXXX)
* 100.0.48.6/23: red E16 (0011 011X XXXX XXXX)
* 100.0.56.0/23: red E17 (0011 100X XXXX XXXX)
* 100.0.56.2/23: red E18 (0011 101X XXXX XXXX)
* 100.0.56.4/23: red E19 (0011 110X XXXX XXXX)
* 100.0.56.6/23: red E20 (0011 111X XXXX XXXX)

Para 200 hosts -> 8 bits (/24). Haciendo subnetting desde /21, cada subred tiene 8 redes F, necesitando ocupar 7 redes /21...

Por último, para 74 hosts -> 7 bits (/25). Solo necesitamos una red, podemos subnetear con 100.0.248/21 -> red G: 100.0.248/25.