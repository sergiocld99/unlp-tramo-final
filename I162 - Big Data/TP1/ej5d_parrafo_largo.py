root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "WordCount/input/"
outputDir = root_path + "WordCount/parrafo_largo/"

## from MRE import Job

def fmap(key, value, context):

    ## nueva key = contar caracteres
    context.write(len(value), value)

        
def fred(key, values, context):

    # mostrar cada valor encontrado
    for v in values:
        context.write(key, v)


job = Job(inputDir, outputDir, fmap, fred)
success = job.waitForCompletion()
print(success)