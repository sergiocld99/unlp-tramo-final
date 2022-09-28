root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

# directorios de trabajo
parent_path = root_path + "jacobi/"
inputDir = parent_path + "input/"
outputDir = parent_path + "output/"

# --------------------------------------

# key es el nombre de variable despejada, value son los coeficientes del resto
# para Jacobi.txt tenemos 15 coeficientes
def fmap_1(key, value, context):
    vars = context["incognitas"]

    coefs = value.split('\t')
    res = 0

    # para TI (vars[0]) el coef siempre debe ser 1
    for i in range(len(coefs)):
        res = res + vars[i] * float(coefs[i])

    context.write(key, res)         # nomVariable, res


def fred_1(key, values, context):
    res = 0

    # obtener valor calculado
    for v in values:
        res = v

    context.write(key, res)         # nomVariable, res

# ---------------------------------------------------------------

params = {"incognitas": [1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]}

job = Job(inputDir, outputDir, fmap_1, fred_1)
job.setParams(params)
success = job.waitForCompletion()