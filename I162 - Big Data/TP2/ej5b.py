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
carpetaSalida3 = parent_path + "out_B1/"
carpetaSalida4 = parent_path + "out_B2/"

# en los inputs, la key es el user id

# -------------------- JOB 3 ---------------------------------
# vamos a reutilizar salida de Job 1, ya que está agrupada por (user, page)
# ignoramos el tiempo

# agrupa por user
def fmap_3(key, value, context):
  values = value.split('\t')
  user_id = key
  page_id = values[0]

  context.write(user_id, page_id)

# cuenta las paginas visitadas (un reducer por usuario)
def fred_3(key, values, context):
  c = 0

  # por cada pagina encontrada, incremento en 1
  for p in values:
    c = c + 1

  context.write(key, c)   # user, cant


# ----------------------- JOB 4 ------------------------------
# continuación de job 3

# agrupa todos los (user, cant) bajo una misma clave
def fmap_4(key, value, context):
  user = key
  cant = int(value)
  context.write(1, (user, cant))

# encuentra el maximo cant (un reducer por usuario)
def fred_max(key, values, context):
  max = -1
  answer = -1

  # precondicion: v es un par (user, cant) [cant entero]
  for v in values:
    if (v[1] > max):
      max = v[1]        # user
      answer = v[0]     # cant

  context.write(key, answer)


# ----------------------- INCISO B ---------------------------
# orden: job 1 -> job 3 -> job 4

job_3 = Job(carpetaSalida1, carpetaSalida3, fmap_3, fred_3)
job_3.waitForCompletion()

job_4 = Job(carpetaSalida3, carpetaSalida4, fmap_4, fred_max)
job_4.waitForCompletion()