## EJERCICIO 5 - ESTACIONES

root_path = 'drive/My Drive/Colab Notebooks/'

from google.colab import drive
drive.mount('/content/drive', force_remount=True)

import datetime, math
import sys
sys.path.append('/content/' + root_path)

inputDir = root_path + "EstacionesMeteorologicas/"

# ------------------- transformaciones ----------------------

# <ID_Estación, fecha_registro, temperatura, humedad, precipitación>

def splitter(linea):
  return linea.split('\t')

# ------------------- carga de archivos ----------------------

estaciones = sc.textFile(inputDir)
estaciones = estaciones.map(splitter)
estaciones = estaciones.map(lambda e: (e[0], e[1], int(e[2]), int(e[3]), int(e[4])) )
print(estaciones.first())

## Parte 1: promedio de temperatura, humedad y precipitacion
part1 = estaciones.map(lambda e: (e[2], e[3], e[4], 1) )
sumas = part1.reduce(lambda e1, e2: (e1[0] + e2[0], e1[1] + e2[1], e1[2] + e2[2], e1[3] + e2[3]) )
promedios = (sumas[0] / sumas[3], sumas[1] / sumas[3], sumas[2] / sumas[3])
print(promedios)

## Parte 2:
# minimos y maximos: de esta forma es 4 veces más rápido
part2 = estaciones.map(lambda e: (e, e, e, e, e, e) )
part2 = part2.reduce(lambda e1, e2: 
                          (e1[0] if e1[0][2] < e2[0][2] else e2[0], 
                           e1[1] if e1[1][2] > e2[1][2] else e2[1],
                           e1[2] if e1[2][3] > e2[2][3] else e2[2],
                           e1[3] if e1[3][3] < e2[3][3] else e2[3],
                           e1[4] if e1[4][4] > e2[4][4] else e2[4],
                           e1[5] if e1[5][4] < e2[5][4] else e2[5]) 
                          )

print(part2)