root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

import random

# directorios de trabajo
parent_path = root_path + "jacobi2/"
inputDir = parent_path + "input/"
outputDir = parent_path + "output/"

# --------------------------------------

# key es el nombre de variable despejada
# aparece en varias lineas: una para cada coeficiente y TI
def fmap_1(key, value, context):
    columnas = value.split('\t')

    incognita_i = key
    coef_i = columnas[0]
    valor = float(columnas[1])

    # agrupamos por incognita despejada (key)
    context.write(incognita_i, (coef_i, valor))


def fred_1(key, values, context):
    res = 0

    coefs = context["incognitas"]

    # obtener valor calculado
    for v in values:
        if (v[0] == "TI"):
            res = res + v[1]
        else:
            res = res + coefs[v[0]] * v[1]

    context.write(key, res)         # nomVariable, res

# ---------------------------------------------------------------

# valores arbitrarios (var1 a var15)
diccio = {}
for i in range(1, 16):
    diccio["var"+str(i)] = random.randint(1,10)

# primera iteración del job
job = Job(inputDir, outputDir, fmap_1, fred_1)
job.setParams({"incognitas": diccio})
job.waitForCompletion()

# para las 14 iteraciones restantes, usar valores calculados
for i in range(14):
    with open(outputDir + "output.txt", "r") as archivo:
        for linea in archivo:
            columnas = linea.split('\t')
            diccio[columnas[0]] = float(columnas[1])       # actualizo valores

    # volver a ejecutar el job
    job = Job(inputDir, outputDir, fmap_1, fred_1)
    job.setParams({"incognitas": diccio})
    job.waitForCompletion()

# fin del programa
