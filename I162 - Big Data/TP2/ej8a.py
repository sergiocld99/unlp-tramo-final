root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

# directorios de trabajo
parent_path = root_path + "WordCount/"
carpetaLibros = parent_path + "input/"        # dataset libros
carpetaSalida1 = parent_path + "tp2_ej8a_out1/"
carpetaSalida2 = parent_path + "tp2_ej8a_out2/"
carpetaSalida3 = parent_path + "tp2_ej8a_out3/"
carpetaSalida4 = parent_path + "tp2_ej8a_out4/"

# primer job: contador de ocurrencias y lineas de cada palabra
def fmap_1(key, value, context):
    words = value.split()

    # para eliminar los duplicados convertimos a diccionario
    uniqueWords: dict = dict.fromkeys(words, 0)     # valor inicializado en 0
    
    # contar ocurrencias para el párrafo actual
    for w in words:
        uniqueWords[w] = uniqueWords[w] + 1

    # enviar a reducer (máximo 1 vez por párrafo para cada palabra distinta)
    for w in uniqueWords:
        context.write(w, uniqueWords[w])

# un reducer por cada palabra a contabilizar
def fred_1(key, values, context):
    ocurrencias = 0
    lineas = 0

    # precondicion: v es un entero
    for v in values:
        ocurrencias = ocurrencias + v
        lineas = lineas + 1

    context.write(key, (ocurrencias, lineas))       # w, (a[w], b[w])

# -----------------------------------------------------------------------

# tercer job: es necesario conocer la cantidad de lineas del dataset

def fmap_3(key, value, context):
    context.write(1, 1)

def fred_3(key, values, context):
    c = 0

    for v in values:
        c = c+1

    context.write("N", c)           # "N", N


# -------------------------------------------------------------------

# job final: realiza el calculo de a[w] * N / b[w] para cada w

# entrada: salida del job 1 (w, a[w], b[w])
# precondicion: en el context está el parámetro N
def fmap_4(key, value, context):
    valores = value.split('\t')
    a = int(valores[0])
    b = int(valores[1])
    N = context["N"]

    context.write(key, a * (N/b))

def fred_4(key, values, context):
    
    for v in values:
        context.write(key, v)

# ----------------------------------------------------------------------

# CALCULO DE a[w] y b[w]
job1 = Job(carpetaLibros, carpetaSalida1, fmap_1, fred_1)
job1.waitForCompletion()

# CALCULO DE N
job3 = Job(carpetaLibros, carpetaSalida3, fmap_3, fred_3)
job3.waitForCompletion()

# OBTENER N
f3 = open(carpetaSalida3 + "output.txt", 'r')
N = int(f3.readline().split('\t')[1])
f3.close()

# realizar calculo para cada palabra...
# para poder hacer esto necesariamente a[w] y b[w] deberian estar en el mismo archivo
# corrección: fusión de jobs 1 y 2

# RESULTADO FINAL PARA CADA PALABRA
job4 = Job(carpetaSalida1, carpetaSalida4, fmap_4, fred_4)
job4.setParams({"N": N})
job4.waitForCompletion()
