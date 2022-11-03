# Ej 3

## Tiempo original
Se tiene como dato que la ejecución tarda 
Tp = 12 horas si se utilizan 
N = 12 procesadores en paralelo.

Un 70% es paralelizable: f = 0.3

Tp = f Ts + (1-f) Ts/N

- Despejamos Ts (Tiempo del secuencial)

Tp = Ts [f + (1-f)/N]

Ts = Tp / [f + (1-f) / N]

Ts = 12h / (0.3 + 0.7 / 12) = 33.49 h

# Cantidad de instrucciones 
Se puede calcular como el producto del
tiempo de ejecución Ts y la velocidad 
del procesador.

I = Ts x Va = 33.49h x 200 Mflops 
I = 33.49h x 200M Inst/s x 3600 s/h
I = 24,112,800,000,000 = 24112800 Millones
