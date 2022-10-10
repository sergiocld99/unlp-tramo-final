# Ejercicio práctico

## Latencias
- Lectura en memoria:   0,3 ns.
- Escritura en memoria: 0,45 ns.
- Lectura/escritura de registros: 0,05 ns.
- Operaciones ALU:      0,25 ns.
- Actualización PC:     0,1 ns.

## Tipos de instrucciones
- Load: F-D-X-M-W
- Store: F-D-X-M
- ALU: F-D-X-W
- BEQ: F-D-X        (salto condicional)

## Frecuencia máxima de monociclo
- LW: 0,3 + 0,1 + 0,25 + 0,3 + 0,05 ns = 1 ns
- SW: 0,3 + 0,1 + 0,05 + 0,25 + 0,45 ns = 1,15 ns
- ALU: 0,3 + 0,1 + 0,05 + 0,25 + 0,05 ns = 0,75 ns
- BEQ: 0,3 + 0,05 + 0,25 + 0,1 ns = 0,7 ns

Determinado por OP más lenta: 869,56 MHz ✔️

## Frecuencia máxima de multiciclo
Determinado por etapa más lenta: 2,22 GHz ✔️

## CPI de multiciclo
Sabiendo que 20% LW, 10% SW, 35% ALU, 35% BEQ.

CPI = (20 x 5 + 10 x 4 + 35 x 4 + 35 x 3) / 100

CPI = 3.85 ✔️