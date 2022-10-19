# EJERCICIO 5: FACTOR DE RIESGO

root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "Banco/"

# ------------------- transformaciones ----------------------

# Cliente: <ID_Cliente, nombre, apellido, DNI, fecha de nacimiento, nacionalidad>
# CajaDeAhorro: <ID_Caja, ID_Cliente, saldo>
# Prestamos: <ID_Caja, cuotas, monto>
# Movimientos: <ID_Caja, monto, timestamp>

def lineToTuple(linea):
  return linea.split('\t')

def calculadorEdad(cliente):
  return 2022 - int(cliente[4].split('-')[0])

def calculadorRiesgo(idCliente, clientes, cajas, prestamos):
  cajasCliente = cajas.filter(lambda c: c[1] == idCliente )
  idCajas = cajasCliente.map(lambda c: c[0]).collect()
  prestamosCliente = prestamos.filter(lambda p: p[0] in idCajas )

  F = prestamosCliente.count()

  # primer control: si no tiene prestamos, no se puede calcular
  if (F == 0):
    return -1

  saldos = cajasCliente.map(lambda c: float(c[2]) )
  montosPrestamo = prestamosCliente.map(lambda p: float(p[2]) )
  cuotasPrestamo = prestamosCliente.map(lambda p: int(p[1]) )
  
  A = saldos.reduce(lambda c1, c2: c1 + c2 )

  # segundo control: si el saldo total es negativo, hay raiz negativa
  if (A < 0):
    return -2

  B = cajasCliente.count()
  C = saldos.filter(lambda s: s < 0 ).count()
  D = montosPrestamo.reduce(lambda m1, m2: m1 + m2 )
  E = cuotasPrestamo.reduce(lambda c1, c2: c1 + c2 ) / F

  return pow(D/E + 0.001, F) / pow(A/B, 1/(B-C+1))

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

# preparar RDD para join
prestamosPorClave = prestamos.map(lambda p: (p[0], (p[1], p[2]) ) )
cajasConPrestamos = prestamos.map(lambda p: p[0] ).collect()
cajas = cajas.filter(lambda c: c[0] in cajasConPrestamos)
clientesConPrestamos = cajas.map(lambda c: c[1]).collect()
clientes = clientes.filter(lambda c: c[0] in clientesConPrestamos)

cajas.persist()
clientes.persist()

# separar clientes por país
clientesPorPais = clientes.groupBy(lambda c : c[-1])
print("Analizando " + str(clientes.count()) + " clientes de " + str(clientesPorPais.count()) + " países")

# por cada cliente
for pais in clientesPorPais.collect():
  riesgosos = 0
  
  for c in pais[1]:
    factorRiesgo = calculadorRiesgo(c[0], clientes, cajas, prestamos)

    if (factorRiesgo > 0 and factorRiesgo < 2):
      riesgosos = riesgosos + 1

  print("En " + pais[0] + " hay " + str(riesgosos) + " clientes riesgosos")

