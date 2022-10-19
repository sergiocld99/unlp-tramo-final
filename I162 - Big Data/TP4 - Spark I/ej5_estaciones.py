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
# norte: en grados centigrados y mm
# sur: en grados fahrenheit tc = (tf - 32) * 1.8, y cm

def splitter(linea):
  return linea.split('\t')

def celsius(fahrenheit):
  return 1.8 * (int(fahrenheit) - 32)

def cm(mm):
  return 10 * int(mm)

# ------------------- carga de archivos ----------------------

norte = sc.textFile(inputDir + "Norte").map(splitter)
sur = sc.textFile(inputDir + "Sur").map(splitter)

# conversion a enteros y a sistema internacional
norte = norte.map(lambda e: (e[0], e[1], int(e[2]), int(e[3]), int(e[4])) )
sur = sur.map(lambda e: (e[0], e[1], celsius(e[2]), int(e[3]), cm(e[4]) ) )

# juntar datasets
estaciones = norte.union(sur)

## Parte 1: promedio de temperatura, humedad y precipitacion
part1 = estaciones.map(lambda e: (e[2], e[3], e[4], 1) )
sumas = part1.reduce(lambda e1, e2: (e1[0] + e2[0], e1[1] + e2[1], e1[2] + e2[2], e1[3] + e2[3]) )
promedios = (sumas[0] / sumas[3], sumas[1] / sumas[3], sumas[2] / sumas[3])
print(promedios)

## Parte 2:
# minimos y maximos: de esta forma es 4 veces más rápido
part2 = estaciones.map(lambda e: (e[2], e[2], e[3], e[3], e[4], e[4]) )
part2 = part2.reduce(lambda e1, e2: 
                          (e1[0] if e1[0] < e2[0] else e2[0], 
                           e1[1] if e1[1] > e2[1] else e2[1],
                           e1[2] if e1[2] > e2[2] else e2[2],
                           e1[3] if e1[3] < e2[3] else e2[3],
                           e1[4] if e1[4] > e2[4] else e2[4],
                           e1[5] if e1[5] < e2[5] else e2[5]) 
                          )

print(part2)