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
carpetaSalida5 = parent_path + "out_C1/"
carpetaSalida6 = parent_path + "out_C2/"
carpetaSalida7 = parent_path + "out_C3/"

# en los inputs, la key es el user id

# -------------------- JOB 5 ---------------------------------

# agrupa por (page, user)
# ignoramos el tiempo, valor generico 1
def fmap_5(key, value, context):
  values = value.split('\t')
  user_id = key
  page_id = values[0]

  context.write((page_id, user_id), 1)

# cuenta las visitas (un reducer por cada par pagina-usuario)
# es igual al reducer del job 3 -> CONTADOR
def fred_count(key, values, context):
  c = 0

  # por cada visita encontrada, incremento en 1
  for v in values:
    c = c + 1

  context.write(key, c)   # (page, user), visitCount


# ----------------------- JOB 6 ------------------------------
# continuación de job 5

# agrupa por página, ignoramos usuario
# al reducer se le pasa las visitas contadas para cada usuario
def fmap_6(key, value, context):
  page = key
  count = int(value.split('\t')[1])
  
  context.write(page, count)

# suma las visitas recibidas (un reducer por pagina)
# es igual al reducer del job 1 -> sumador
def fred_sum(key, values, context):
  suma = 0

  # precondicion: v es entero
  for v in values:
    suma = suma + v

  context.write(key, suma)

# -------------------------- JOB 7 -----------------------------
# continuacion de job 6

# agrupa todo bajo una misma clave
def fmap_7(key, value, context):
  page = key
  count = int(value)

  context.write(1, (page, count))

# encuentra el maximo count (un reducer por pagina)
def fred_max(key, values, context):
  max = -1
  answer = -1

  # precondicion: v es un par (page, count) [count entero]
  for v in values:
    if (v[1] > max):
      max = v[1]        # count
      answer = v[0]     # page

  context.write(key, answer)


# ----------------------- INCISO C ---------------------------
# orden: job 5 -> 6 -> 7

job_5 = Job(inputDir, carpetaSalida5, fmap_5, fred_count)
job_5.waitForCompletion()

job_6 = Job(carpetaSalida5, carpetaSalida6, fmap_6, fred_sum)
job_6.waitForCompletion()

job_7 = Job(carpetaSalida6, carpetaSalida7, fmap_7, fred_max)
job_7.waitForCompletion()