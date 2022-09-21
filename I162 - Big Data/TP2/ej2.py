root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "WordCount/input/"
outputDir = root_path + "WordCount/palabras/"

## from MRE import Job

# separa cada frase en palabras, para usarlas como claves
def fmap(key, value, context):
    words = value.split()
    
    for w in words:
        context.write(w, 1)
        
# cuenta cuantos valores tiene cada clave
def fred(key, values, context):
    c=0

    for v in values:
        c=c+1

    context.write(key, c)

# job 1
job = Job(inputDir, outputDir, fmap, fred)
job.setCombiner(fred)                           # asignar combiner
success = job.waitForCompletion()

print(success)