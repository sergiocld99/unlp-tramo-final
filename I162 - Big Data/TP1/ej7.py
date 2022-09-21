root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "Inversionistas/input/"
outputDir = root_path + "Inversionistas/output/"

## from MRE import Job

def fmap(key, value, context):
    params = value.split('\t')

    # obtener importe
    importe = int(params[4])
    context.write(-1, importe)

    # obtener fecha de nac
    year = int(params[3])
    month = int(params[2])
    day = int(params[1])

    # edad en dias
    edad = (2022*365+9*12+17) - (year*365+month*12+day)
    context.write(-3, (edad, value))

    # edad en años
    anios = edad / 365
    context.write(-2, anios)
        

def fred(key, values, context):

    if (key == -1):
        total = 0
        
        for i in values:
            total = total + i
        
        context.write("importe total", total)
    elif (key == -2):
        total = 0
        cant = 0
        
        for i in values:
            total = total + i
            cant = cant + 1
        
        context.write("edad promedio", total / cant)
    else:
        ## obtener edad minima
        min = 9999
        data_min = 0

        for v in values:
            edad = v[0]

            if (edad < min):
              min = edad
              data_min = v[1]
            
        context.write("mas joven", data_min)


job = Job(inputDir, outputDir, fmap, fred)
success = job.waitForCompletion()
print(success)