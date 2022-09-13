root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "WordCount/input/"
outputDir = root_path + "WordCount/palabras/"

## from MRE import Job

def fmap(key, value, context):
    words = value.split()
    for w in words:
        context.write(w, 1)
        
def fred(key, values, context):
    c=0
    for v in values:
        c=c+1
    context.write(key, c)

job = Job(inputDir, outputDir, fmap, fred)
success = job.waitForCompletion()
print(success)