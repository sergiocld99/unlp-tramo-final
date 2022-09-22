root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "WordCount/palabras/"
outputDir = root_path + "WordCount/tp2_ej3/out_1/"
desvioDir = root_path + "WordCount/tp2_ej3/out_2/"

# map job 1
def fmap(key, value, context):
  valores = value.split('\t')

  ocurrencias = int(valores[0])
  context.write(1, ocurrencias)
  context.write(2, ocurrencias)
  context.write(3, ocurrencias)

# reduce job 1
def fred(key, values, context):

  if (key == 1):
    min = 9999

    for v in values:
      if (v < min):
        min = v

    context.write("min", min)

  elif (key == 2):
    max = -1

    for v in values:
      if (v > max):
        max = v
    
    context.write("max", max)

  else:
    suma = 0
    cant = 0

    for v in values:
      suma = suma + v
      cant = cant + 1

    context.write("promedio", suma / cant)

# map del job 2
def fmap_2(key, value, context):
  valores = value.split('\t')

  ocurrencias = int(valores[0])
  resta = ocurrencias - context["avg"]
  context.write(1, resta**2)

# reduce del job 2
def fred_2(key, values, context):
  suma = 0
  cant = 0

  for v in values:
    suma = suma + v
    cant = cant + 1

  context.write("desvio", suma / (cant-1))

# job 1
job = Job(inputDir, outputDir, fmap, fred)
# job.setCombiner(fred)
job.waitForCompletion()

# leer promedio obtenido (3ra linea)
f = open(outputDir + "output.txt", 'r')
f.readline()
f.readline()
linea = f.readline()
f.close()

promedio = float(linea.split('\t')[1])
params = {"avg": promedio}

# job 2
job2 = Job(inputDir, desvioDir, fmap_2, fred_2)
# job2.setCombiner(fred_2)
job2.setParams(params)
job2.waitForCompletion()

