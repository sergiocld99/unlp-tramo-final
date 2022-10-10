# 3 procesadores
- Versión 1: 2 etapas, t = 60 ns
- Versión 2: 3 etapas, t = 45 ns
- Versión 3: 5 etapas, t = 30 ns

## Ejecución de 10 instrucciones
Versión 1:

    T = (n + k - 1) * t = 11 * 60 ns = 660 ns
    CPI = 11 / 10 = 1.1

Versión 2:

    T = (10 + 3 - 1) * 45 ns = 540 ns
    CPI = 12 / 10 = 1.2

Versión 3:

    T = (10 + 5 - 1) * 30 ns = 420 ns
    CPI = 14 / 10 = 1.4

## Vs monociclo de 10 MHz (T = 1000 ns)
- Versión 1: mejora 1000 / 660 = 1.51 < 2
- Versión 2: mejora 1000 / 540 = 1.85 < 3
- Versión 3: mejora 1000 / 420 = 2.38 < 5

## Ejecución de 1000 instrucciones
- V1: T = 60060 ns, CPI = 1.001, S = 1.66 < 2
- V2: T = 45090 ns, CPI = 1.002, S = 2.21 < 3
- V3: T = 30120 ns, CPI = 1.004, S = 3.32 < 5

El speedup o mejora revela cuál es el mejor.

## Mejora limite (n infinito)

    T = n * 100 ns
    Tseg = (n + k - 1) * t = nt + (k-1)*t / n
    S = T / Tseg = 100 ns / t

- V1: S = 100 / 60 = 1.67
- V2: S = 100 / 45 = 2.22
- V3: S = 100 / 30 = 3.33

Efectivamente, con n = 1000 se acercó al limite.

