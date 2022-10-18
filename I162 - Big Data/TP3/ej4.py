## EJERCICIO DE LA MEDIANA

root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

import random

# directorios de trabajo
parent_path = root_path + "website/"
inputDir = parent_path + "input/"
outputDir = parent_path + "tp3-ej4/"
carpetaCount = parent_path + "tp3-ej4_n/"

# --------------------------------------------

def fmap_count(key, value, context):
  context.write(1,1)

def fred_count(key, values, context):
  c = 0

  for v in values:
    c = c+1

  context.write("N", c)

# --------------------------------------

def fmap(key, value, context):
  columnas = value.split('\t')
  # time es la columna 3 del archivo -> segundo value -> columnas[1]
  context.write(columnas[1], columnas[1])

def fshuffle(aKey, anotherKey):
  return 0

def fsort(aKey, anotherKey):
  num1 = float(aKey)
  num2 = float(anotherKey)

  if (num1 == num2):
    return 0
  elif (num1 < num2):
    return -1
  else:
    return 1

def fred(key, values, context):
  cant = context["N"]
  mediana = None
  i = 0

  # precondición: sort ya ordenó de menor a mayor
  if (cant % 2 == 0):
    valor_izq = None
    valor_der = None

    # si cant es par, es el promedio entre los elementos N/2 y su siguiente
    # como no puedo usar indice, vuelvo a iterar
    for v in values:
      if (i==cant/2):
        valor_izq = v
      elif (i == cant/2 + 1):
        valor_der = v
        break
      i = i+1
    mediana = (float(valor_izq) + float(valor_der)) / 2
  else:
    # si cant es impar, es el elemento (N+1)/2
    for v in values:
      if (i == (cant+1)/2):
        mediana = v
        break
      i = i+1

  # imprimir la mediana
  context.write("mediana", mediana)

# -------------------------------------------------

jobCounter = Job(inputDir, carpetaCount, fmap_count, fred_count)
jobCounter.waitForCompletion()

# leer N
valor_n = None

with open(carpetaCount + "output.txt", "r") as arch:
  linea = arch.readline()
  valor_n = int(linea.split('\t')[1])

job = Job(inputDir, outputDir, fmap, fred)
job.setShuffleCmp(fshuffle)
job.setSortCmp(fsort)
job.setParams({"N": valor_n})
job.waitForCompletion()

# fin del programa