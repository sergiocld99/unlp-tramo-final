root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

# directorios de trabajo
parent_path = root_path + "Banco/"
clientesDir = parent_path + "clientes/"
cajasDir = parent_path + "cajas/"
prestamosDir = parent_path + "prestamos/"

outputDir0 = parent_path + "output0/"
outputDir1 = parent_path + "output1/"
outputDir2 = parent_path + "output2/"
outputDirFinale = parent_path + "outputFinale/"

# --------------------------------------------------------------------

# INNER JOIN ON CA.ID_CAJA = P.ID_CAJA
# Prestamo = id_caja, cuotas, monto
# puede existir o no un préstamo asociado a una caja (0-1)

def fmap_P(key, value, context):
    datos = value.split('\t')

    context.write(("P", key), datos)        # ("P", id_caja), (cuotas, monto)

def fmap_CA_0(key, value, context):
    datos = value.split('\t')

    context.write(("CA", key), datos)       # ("CA", id_caja), (id_cliente, saldo)

# agrupa las tuplas de Prestamo y Caja que coincidan en id_caja
def fshuffle_0(aKey, anotherKey):
    id_1 = aKey[1]
    id_2 = anotherKey[1]

    if (id_1 == id_2):
        return 0
    elif (id_1 < id_2):
        return -1
    else:
        return 1

# PROBLEMA: NUNCA LLEGAN LAS KEYS DE PRESTAMO Y CAJA AL MISMO REDUCER
def fred_0(key, values, context):
    datos_caja = None
    datos_prestamo = None

    for v in values:
        if (key[0] == "CA"):
            datos_caja = v
        else:
            datos_prestamo = v

    # si existe el préstamo para el id_caja
    if (datos_prestamo != None):
        id_caja = key[1]
        # se escribe id_caja, id_cliente, saldo, cuotas, monto
        context.write(id_caja, datos_caja + datos_prestamo)


# --------------------------------------

# INNER JOIN ON C.ID_CLIENTE = CA.ID_CLIENTE (CA = T0)

def fmap_C(key, value, context):
    context.write(("C", key), value)

def fmap_T0(key, value, context):
    columnas = value.split('\t')

    id_caja = key
    id_cliente = columnas[0]
    saldo = columnas[1]
    cuotas = columnas[2]
    monto = columnas[3]

    context.write(("CA", id_cliente), (id_caja, saldo, cuotas, monto))

# agrupa las tuplas de Cliente y Caja que coincidan en id_cliente
def fShuffleComb(aKey, anotherKey):
    idCliente_1 = aKey[1]
    idCliente_2 = anotherKey[1]

    if (idCliente_1 == idCliente_2):
        return 0
    elif (idCliente_1 < idCliente_2):
        return -1
    else:
        return 1

# ordena las tuplas de un id_cliente de modo que primero aparezca Cliente
def fSortComb(aKey, anotherKey):
    tipo1 = aKey
    tipo2 = anotherKey

    if (tipo1 == tipo2):
        return 0
    elif (tipo1 == "C"):
        return -1
    else:
        return 1

def fred_1(key, values, context):
    datos_cliente = None

    # precondición: primero se recibe el cliente (ordenado por sort)
    for v in values:
        if (key[0] == "C"):
            datos_cliente = v
        else:
            id_cliente = key[1]
            # juntar tuplas de Cliente y Caja
            # escribe id_cliente, nom, ap, dni, fn, nacionalidad, id_caja, saldo, cuotas, monto
            context.write(id_cliente, datos_cliente + v)

# ---------------------------------------------------------------------

# SELECT COUNT(*) GROUP BY NACIONALIDAD

def fmap_group(key, value, context):
    columnas = value.split('\t')
    nacionalidad = columnas[4]
    context.write(nacionalidad, 1)

def fred_group(key, values, context):
    c = 0

    for v in values:
        c = c+1

    context.write(key, c)

# -------------------------------------------------------------------

# ORDER BY COUNT(*) DESC

def fmap_finale(key, value, context):
    context.write(1, (key, value))

def fsort_finale(aKey, anotherKey):
    cuantos1 = int(aKey[1])
    cuantos2 = int(anotherKey[1])

    if (cuantos1 == cuantos2):
        return 0
    elif (cuantos1 > cuantos2):
        return -1
    else:
        return 1

# recibo tuplas 1, (nacionalidad, cuantos)
def fred_finale(key, values, context):
    # precondicion: cuantos ordenados de manera descendente
    for v in values:
        context.write(v[0], v[1])       # nacionalidad, cuantos
        break


# -------------------------------------------------------------------

# inner join caja y prestamo
job0 = Job(cajasDir, outputDir0, fmap_CA_0, fred_0)
job0.addInputPath(prestamosDir, fmap_P)
job0.setShuffleCmp(fshuffle_0)
job0.waitForCompletion()

# inner join cliente y join anterior
job1 = Job(clientesDir, outputDir1, fmap_C, fred_1)
job1.addInputPath(outputDir0, fmap_T0)
job1.setShuffleCmp(fShuffleComb)
job1.setSortCmp(fSortComb)
job1.waitForCompletion()

# select count(*) group by nacionalidad
job2 = Job(outputDir1, outputDir2, fmap_group, fred_group)
job2.waitForCompletion()

# order by
job3 = Job(outputDir2, outputDirFinale, fmap_finale, fred_finale)
job3.setSortCmp(fsort_finale)
job3.waitForCompletion()