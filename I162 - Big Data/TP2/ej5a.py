root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

# directorios de trabajo
parent_path = root_path + "website/"
inputDir = parent_path + "input/"
carpetaSalida1 = parent_path + "out_A1/"
carpetaSalida2 = parent_path + "out_A2/"

# en los inputs, la key es el user id

# -------------------- JOB 1 ---------------------------------

# agrupa por (user, page)
def fmap_1(key, value, context):
  values = value.split('\t')
  user_id = key
  page_id = values[0]
  time = int(values[1])

  context.write((user_id, page_id), time)

# reducer del job 1: suma los tiempos de una pagina visitada por un usuario
# en values recibo los times de una pagina X y usuario Y
def fred_1(key, values, context):
  tiempo = 0

  # precondicion: t es entero
  for t in values:
    tiempo = tiempo + t

  context.write(key, tiempo)


# ----------------------- JOB 2 ------------------------------

# agrupa por user
def fmap_2(key, value, context):
  values = value.split('\t')
  user_id = key
  page_id = values[0]
  total_time = int(values[1])

  context.write(user_id, (page_id, total_time))

# encuentra el maximo tiempo (un reducer por usuario)
def fred_2(key, values, context):
  max = -1
  answer = -1

  # precondicion: v es un par (page, time) [time entero]
  for v in values:
    if (v[1] > max):
      max = v[1]        # time
      answer = v[0]     # page

  context.write(key, answer)


# ----------------------- INCISO A ---------------------------
# orden: job 1 -> job 2

job_1 = Job(inputDir, carpetaSalida1, fmap_1, fred_1)
job_1.waitForCompletion()

job_2 = Job(carpetaSalida1, carpetaSalida2, fmap_2, fred_2)
job_2.waitForCompletion()

