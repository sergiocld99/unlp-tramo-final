## PRÁCTICA 5

## Ejercicio 2

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
  tupla = linea.split('\t')
  return (tupla[0], tupla[1], int(tupla[2]), int(tupla[3]), int(tupla[4]))

def fMaximos(e1, e2):
  temp = e1[0] if e1[0][2] > e2[0][2] else e2[0]
  hum = e1[1] if e1[1][3] > e2[1][3] else e2[1]
  prec = e1[2] if e1[2][4] > e2[2][4] else e2[2]
  return (temp, hum, prec)

def celsius(fahrenheit):
  return 1.8 * (int(fahrenheit) - 32)

def cm(mm):
  return 10 * int(mm)

def splitterNorte(linea):
  e = splitter(linea)
  return (e[0], e[1], int(e[2]), int(e[3]), int(e[4]))

def splitterSur(linea):
  e = splitter(linea)
  return (e[0], e[1], celsius(e[2]), int(e[3]), cm(e[4]) )

# ------------------- carga de archivos ----------------------

# TR 1-4: conversion a enteros y a sistema internacional
norte = sc.textFile(inputDir + "Norte").map(splitterNorte)
sur = sc.textFile(inputDir + "Sur").map(splitterSur)

# TR 5: juntar datasets
estaciones = norte.union(sur)

# TR 6
estaciones = estaciones.map(lambda e: (e, e, e) )

# acciones
maximos = estaciones.reduce(fMaximos)
ids = (maximos[0][0], maximos[1][0], maximos[2][0])

print(ids)

