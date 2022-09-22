root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

librosDir = root_path + "WordCount/input/"
outputDir = root_path + "WordCount/tp2_ej4/out_1/"
desvioDir = root_path + "WordCount/tp2_ej4/out_2/"

# map job 1
def fmap(key, value, context):
  cant = 0

  for car in value:
    cant = cant + 1

  context.write(1, cant)

# reduce job 1: calcula el promedio
def fred(key, values, context):
  suma = 0
  cant = 0

  for v in values:
    suma = suma + v
    cant = cant + 1

  context.write("promedio", suma / cant)

# map del job 2: cuenta longitud de cada parrafo
def fmap_2(key, value, context):
  cant = 0

  for car in value:
    cant = cant + 1

  context.write(value, cant)

# reduce del job 2
def fred_2(key, values, context):

  for v in values:
    if (v > context["avg"]):
      context.write(v, key)

# job 1
job = Job(librosDir, outputDir, fmap, fred)
# job.setCombiner(fred)
job.waitForCompletion()

# leer promedio obtenido
# out1/output.txt solo tiene 1 linea
f = open(outputDir + "output.txt", 'r')
linea = f.readline()
f.close()

promedio = float(linea.split('\t')[1])
params = {"avg": promedio}

# job 2
job2 = Job(librosDir, desvioDir, fmap_2, fred_2)
# job2.setCombiner(fred_2)
job2.setParams(params)
job2.waitForCompletion()