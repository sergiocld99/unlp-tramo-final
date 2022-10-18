root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "Banco/"
outputDir = root_path + "output_TP4/"

# ------------------- transformaciones ----------------------

# Cliente: <ID_Cliente, nombre, apellido, DNI, fecha de nacimiento, nacionalidad>
# CajaDeAhorro: <ID_Caja, ID_Cliente, saldo>
# Prestamos: <ID_Caja, cuotas, monto>
# Movimientos: <ID_Caja, monto, timestamp>

def lineToTuple(linea):
  return linea.split('\t')

# ------------------------------------------------------------

# fuentes
clientes = sc.textFile(inputDir + "clientes")
cajas = sc.textFile(inputDir + "cajas")
prestamos = sc.textFile(inputDir + "prestamos")
movimientos = sc.textFile(inputDir + "mov")

# de lineas a tuplas
clientes = clientes.map(lineToTuple)
cajas = cajas.map(lineToTuple)
prestamos = prestamos.map(lineToTuple)
movimientos = movimientos.map(lineToTuple)

# convertimos la fecha de nacimiento en año, mes y día (enteros)
clientes = clientes.map(lambda t: (t[1] + " " + t[2], t[4].split("-"), t[5]))
clientes = clientes.map(lambda t: (t[0], int(t[1][0]), int(t[1][1]), int(t[1][2]), t[2]))

# en cajas pasamos los saldos a real
cajas = cajas.map(lambda c: (c[0], c[1], float(c[2])) )

# en prestamos pasamos las cuotas a numero
prestamos = prestamos.map(lambda t: (t[0], int(t[1]), t[2]) )

# en movimientos pasamos los montos a real // timestamp lo dejamos asi nomas
movimientos = movimientos.map(lambda m: (m[0], float(m[1]), m[2]) )

# ayudita de formato
print(cajas.first())
print(prestamos.first())
print(movimientos.first())
print('\n')

## Inciso A: nombre y apellido de capricornianos, es decir, 22/12 al 19/1
# la cuarta columna (contando desde 0) es la fecha de nacimiento
incisoA = clientes.filter(lambda t: (t[2] == 1 and t[3] < 20) or (t[2] == 12 and t[3] > 21) )
incisoA = incisoA.map(lambda t: t[0] )
print(incisoA.collect())

## Inciso B: nombre y apellido de argentinos
# la nacionalidad es la quinta columna
argentinos = clientes.filter(lambda t: t[4] == 'ARG')
incisoB = argentinos.map(lambda t: t[0])
print(incisoB.collect())

## Inciso C: de (a), cuántos nacieron en verano
print("De capricornio, en verano nacieron " + str(incisoA.count()))

## Inciso D: de (b) el más joven y el más viejo
# para ordenar por más de un parámetro se utiliza una tupla
print(argentinos.sortBy(lambda t: (t[1], t[2], t[3]), ascending=False).first())
print(argentinos.sortBy(lambda t: (t[1], t[2], t[3]), ascending=True).first())

## Inciso E: el préstamo con mayor cantidad de cuotas (columna 1)
maximoCuotas = prestamos.reduce(lambda p1, p2: p1 if p1[1] > p2[1] else p2)

# ... las que tienen misma cantidad de cuotas, el mayor monto
masCuotas = prestamos.filter(lambda p: p[1] == maximoCuotas[1])
mayorMonto = masCuotas.reduce(lambda p1, p2: p1 if p1[2] > p2[2] else p2 )
print(mayorMonto)

## Inciso F: id de clientes con al menos una caja con saldo > 300
saldosMayor = cajas.filter(lambda c: c[2] > 300)
idSaldosMayor = cajas.map(lambda c: c[1] )
print(idSaldosMayor.collect())

## Inciso G: monto de mayor movimiento e ID de caja del ultimo mov
mayorMov = movimientos.reduce(lambda m1, m2: m1 if m1[1] > m2[1] else m2)
print("Monto del mayor movimiento: " + str(mayorMov[1]))

ultimoMov = movimientos.reduce(lambda m1, m2: m1 if m1[2] > m2[2] else m2)
print("ID de caja del ultimo mov: " + ultimoMov[0])