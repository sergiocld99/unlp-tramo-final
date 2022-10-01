root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

# directorios de trabajo
parent_path = root_path + "Banco/"
cajasDir = parent_path + "cajas/"
clientesDir = parent_path + "clientes/"
prestamosDir = parent_path + "prestamos/"

outputDir0 = parent_path + "output0/"
outputDir1 = parent_path + "output1/"
outputDir2 = parent_path + "output2/"
outputDirFinale = parent_path + "outputFinale/"

# --------------------------------------------------------------------

# INNER JOIN ON CA.ID_CAJA = P.ID_CAJA
# Prestamo = id_caja, cuotas, monto
# puede existir o no un préstamo asociado a una caja (0-1)

def fmap_CA_0(key, value, context):
    datos = value.split('\t')
    context.write(key, ("CA", datos))       # id_caja, (CA, (id_cliente, saldo))

def fmap_P(key, value, context):
    datos = value.split('\t')
    context.write(key, ("P", datos))        # id_caja, (P, (cuotas, monto))

def fred_0(key, values, context):
    datos_caja = None
    datos_prestamo = None

    for v in values:
        if (v[0] == "CA"):
            datos_caja = v[1]
        else:
            datos_prestamo = v[1]

    # si existe el préstamo para el id_caja
    if (datos_prestamo != None):
        id_caja = key
        # se escribe id_caja, id_cliente, saldo, cuotas, monto
        context.write(id_caja, datos_caja + datos_prestamo)


# --------------------------------------

# INNER JOIN ON C.ID_CLIENTE = CA.ID_CLIENTE (CA = T0)

def fmap_C(key, value, context):
    datos = value.split('\t')
    context.write((key, "C"), datos)

def fmap_T0(key, value, context):
    columnas = value.split('\t')

    id_cliente = columnas[0]
    datos_caja = [key, columnas[1], columnas[2], columnas[3]]

    context.write((id_cliente, "CA"), datos_caja)

# agrupa las tuplas de Cliente y Caja que coincidan en id_cliente
def fshuffle_1(aKey, anotherKey):
    idCliente_1 = aKey[0]
    idCliente_2 = anotherKey[0]

    if (idCliente_1 == idCliente_2):
        return 0
    elif (idCliente_1 < idCliente_2):
        return -1
    else:
        return 1

# ordena las tuplas de un id_cliente de modo que primero aparezca Cliente
def fsort_1(aKey, anotherKey):
    tipo1 = aKey[1]
    tipo2 = anotherKey[1]

    if (tipo1 == tipo2):
        return 0
    elif (tipo1 == "C"):
        return -1
    else:
        return 1

def fred_1(key, values, context):
    datos_cliente = None
    primero = True

    # precondición: primero se recibe el cliente (ordenado por sort)
    for v in values:
        if (primero):
          datos_cliente = v
          primero = False
        else:
            id_cliente = key[0]
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
    context.write(value, (key, value))

def fshuffle_finale(aKey, anotherKey):
    return 0

def fsort_finale(aKey, anotherKey):
    cuantos1 = int(aKey)
    cuantos2 = int(anotherKey)

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
#job0.setShuffleCmp(fshuffle_0)
job0.waitForCompletion()

# inner join cliente y join anterior
job1 = Job(clientesDir, outputDir1, fmap_C, fred_1)
job1.addInputPath(outputDir0, fmap_T0)
job1.setShuffleCmp(fshuffle_1)
job1.setSortCmp(fsort_1)
job1.waitForCompletion()

# select count(*) group by nacionalidad
job2 = Job(outputDir1, outputDir2, fmap_group, fred_group)
job2.waitForCompletion()

# order by
job3 = Job(outputDir2, outputDirFinale, fmap_finale, fred_finale)
job3.setShuffleCmp(fshuffle_finale)
job3.setSortCmp(fsort_finale)
job3.waitForCompletion()