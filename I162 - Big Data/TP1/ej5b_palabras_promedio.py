root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "WordCount/input/"
outputDir = root_path + "WordCount/promedio/"

## from MRE import Job

def fmap(key, value, context):
    words = value.split()

    # pasar cantidad de palabras bajo clave genérica
    context.write(1, len(words))

        
def fred(key, values, context):
    palabras=0
    parrafos=0

    ## contar palabras y parrafos
    for v in values:
        palabras = palabras + v
        parrafos = parrafos + 1

    ## mandar promedio general
    context.write(1, palabras / parrafos)



job = Job(inputDir, outputDir, fmap, fred)
success = job.waitForCompletion()
print(success)