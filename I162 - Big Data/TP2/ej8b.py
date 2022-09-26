root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

# directorios de trabajo
parent_path = root_path + "bool/"
carpetaEntrada = parent_path + "input/"        # dataset bool
carpetaSalida1 = parent_path + "out1/"
carpetaSalida2 = parent_path + "out2/"

# primer job: 
def fmap_1(key, value, context):
    v = value.split('\t')               # v es un vector de booleanos
    c = v[-1]

    for a in range(len(v) - 1):
        x = v[a]
        context.write((a, x, c), 1)     # un reducer los contabilizará

# un reducer por cada palabra a contabilizar
def fred_1(key, values, context):
    c = 0

    for v in values:
        c = c+1

    context.write(key, c)           # (a,x,c), cant
 
# ----------------------------------------------------------------------------

# entrada: salida del job 1
def fmap_2(key, value, context):
    # for x in range(len(m)) es para valor distinto de la key
    # for y in range(len(m[0])) es para valor distinto de value.split('\t')[0]
    # for z in range(2) es para valor distinto de value.split('\t')[1], o sea, cada fila
    
    columnas = value.split('\t')
    x = key
    y = columnas[0]
    z = columnas[1]
    valor_celda = int(columnas[2])

    # max[z][0]
    context.write(z, valor_celda, x, y)

# obtencion de max[z], dos reducers (uno para z=0 y otro para z=1)
def fred_2(key, values, context):
    max = (0, 0, 0)

    # v es (valor_celda, x, y), solo importa que valor_celda sea entero
    for v in values:
        valor_celda = v[0]

        if (valor_celda > max[0]):
            max[0] = valor_celda
            max[1] = v[1]
            max[2] = v[2]

    # escribimos las 3 celdas de max[z]
    context.write(key, max)

# ----------------------------------------------------------------------

# relleno de la matriz m
job1 = Job(carpetaEntrada, carpetaSalida1, fmap_1, fred_1)
job1.waitForCompletion()

# obtención de máximos
job2 = Job(carpetaSalida1, carpetaSalida2, fmap_2, fred_2)
job2.waitForCompletion()

# imprimir ubicación de máximos
f = open(carpetaSalida2 + "output.txt", "r")

for z in range(2):
    linea = f.readline()
    columnas = linea.split('\t')    # z, valor_celda, x, y
    x = columnas[2]
    y = columnas[3]
    print(z + ";" + x + ";" + y)

f.close()
