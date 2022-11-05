;; Actualmente tenemos 1 min es aprox 1000 ticks

;; variables globales inicializadas por defecto en cero
globals [
  pasajeros_restantes_izq
  pasajeros_restantes_der
]


;; -------------------- DEFINICIÓN DE AGENTE ESTACIONARIO ---------------------------------

patches-own [ elevacion anden ]


;; -------------------- DEFINICIÓN DE AGENTES MÓVILES -----------------------------

;; trenes: estado 0 (yendo), 1 (en anden), 2 (lejano)
;;  estado 0: en marcha (se acerca o aleja del centro de la estación)
;;  estado 1: detenido (se encuentra en el andén durante un cierto tiempo)
;;  estado 2: lejano (es invisible durante un cierto tiempo)
trenes-own [ estado espera t_anden ]

;; pasajeros
;;  estado 11: saliendo de la estación (se dirige hacia los molinetes)
;;  estado 12: yendo a puente (se dirige hacia el comienzo del puente)
;;  estado 1: cruzando puente (se dirige hacia el andén destino)
;;  estado 14: esperando tren (se dirige y espera en un lugar random del andén)
;;  estado 15: subiendo a tren (se dirige hacia el tren si está en el andén)
pasajeros-own [ estado anden_actual anden_dest molinete ]

;; plurales
breed [trenes tren]
breed [pasajeros pasajero]


;; -------------------------- SETUP ESTACIONARIO ---------------------------

to setup-patches
  ask patches with [ abs(pxcor) < 6 ] [set elevacion 0]
  ask patches with [ abs(pxcor) >= 6 ] [set elevacion 1]
  ask patches with [ pycor > 9 and pycor < 12 and abs(pxcor) < 8 ] [set elevacion 2]
  ask patches with [ elevacion = 1 and pxcor < 0 ] [set anden 0]
  ask patches with [ elevacion = 1 and pxcor > 0 ] [set anden 1]

  ;; pintar según elevación
  ask patches with [ elevacion = 0 ] [set pcolor black]
  ask patches with [ elevacion = 1 ] [set pcolor 5 + random(4)]
  ask patches with [ elevacion = 2 ] [set pcolor red]

  ;; pintar línea de precaución
  ask patches with [ elevacion = 1 and abs(pxcor) = 6 ] [set pcolor yellow]

  ;; pintar vias y durmientes
  ask patches with [ elevacion = 0 and abs(pxcor) > 0 ] [
    if (pycor mod 2 = 0) [set pcolor brown]
    if (pxcor mod 2 = 0) [set pcolor 7]
  ]
end


;; ------------------------- CREACIÓN DE AGENTES MÓVILES ------------------------

;; solo existen 2 trenes, uno por cada sentido o vía
;; al comienzo de la simulación no se encuentran en la estación
to setup-trenes
  create-trenes 2

  ;; parámetros comunes
  ask trenes [
    set shape "tren"
    set color gray
    set size 12
    set hidden? true
    set estado 2
    set t_anden 0
  ]

  ;; parámetros del tren plaza
  ask trenes with [who = 0] [
    set espera frecuencia_tren / 2       ;; delay inicial medio
    set heading 0                        ;; orientación norte
    set xcor -3                          ;; vía izquierda
  ]

  ;; parámetros del tren la plata
  ask trenes with [who = 1] [
    set espera frecuencia_tren           ;; delay inicial completo
    set heading 180                      ;; orientación sur
    set xcor 3                           ;; via derecha
  ]

end


;; parámetros comunes para ambos andenes
to crear_pasajero
  set shape "pasajero"
  set ycor 4 - (who mod 10)
  set color orange
  set size 2
  lt random 360
  ifelse (random 2 = 0) [set molinete 16] [set molinete -16]
end


to crear_pasajero_izq
  set xcor -6.1
  crear_pasajero

  ;;set estado 0
  set estado 10
  set anden_actual 0
  set anden_dest random 2

  ;; actualizar contador global
  set pasajeros_restantes_izq (pasajeros_restantes_izq - 1)
end


to crear_pasajero_der
  set xcor 6.1
  crear_pasajero

  ;; desde este lado no hacen combinación
  set estado 0
  set anden_actual 1
  set anden_dest random 2

  ;; actualizar contador global
  set pasajeros_restantes_der (pasajeros_restantes_der - 1)
end


;; ------------------------- UPDATE DE TRENES SEGÚN ESTADO ---------------------------

;; TREN - ESTADO 0
to update_tren_en_marcha
  ;; Realizar accion del estado (avanzar)
  fd (abs(ycor) + 1) / 500

  ;; Chequear entradas
  ;; Camino 1 - ¿Se encuentra en el lugar de espera?
  if (abs(ycor) < 0.01) [
    set estado 1                    ;; cambiar a estado "detenido"
    set t_anden tolerancia_anden    ;; asignar tiempo de espera

    ifelse (who = 0)                ;; indicar cuantos pasajeros deben crearse
      [set pasajeros_restantes_izq cant_bajan]
      [set pasajeros_restantes_der cant_bajan]
  ]

  ;; Camino 2 - ¿Se alejó el tren lo suficiente?
  if (abs(ycor) >= max-pycor) [
    set estado 2                    ;; cambiar a estado "lejano"
    set espera frecuencia_tren      ;; asignar tiempo hasta próximo arribo
    set hidden? true                ;; volver invisible al usuario
  ]
end


;; TREN - ESTADO 1
to update_tren_detenido
  ;; Realizar acción del estado (disminuir espera si nadie baja)
  if (who = 0 and pasajeros_restantes_izq = 0) [set t_anden (t_anden - 1)]
  if (who = 1 and pasajeros_restantes_der = 0) [set t_anden (t_anden - 1)]

  ;; Camino 1 - ¿Se cumplió el tiempo de espera?
  if (t_anden = 0) [
    set estado 0              ;; pasar al estado "en marcha"
    fd 0.02                   ;; avanzar para evitar bloqueo
  ]
end


;; TREN - ESTADO 2
to update_tren_lejano
  ;; Realizar acción del estado (disminuir espera)
  set espera (espera - 1)

  ;; Camino 1 - ¿Se cumplió el tiempo de espera?
  if (espera = 0) [
    ifelse who = 0 [set ycor min-pycor] [set ycor max-pycor]
    set hidden? false
    set estado 0
  ]
end


;; ------------------------- UPDATE DE PASAJEROS SEGÚN ESTADO ---------------------------

to update_pasajero_saliendo
  ;; Realizar acción del estado (acercarse a la salida)
  let x_esperada (8 + who mod 8) * (anden - 1)

  ifelse (distancexy x_esperada ycor > 0.02)
    [facexy x_esperada ycor]
    [facexy xcor molinete]

  ;; avanzar solo si no hay nadie delante
  if (patch-ahead 1 = nobody) or (count pasajeros-on patch-ahead 1 = 0) [fd 0.01]

  ;; Camino 1 - ¿Llegó al molinete?
  if (abs(ycor) >= max-pycor) [die]

end


to mover_pasajero
  let limite 0

  ;; si sale por los molinetes -> matarlos
  if abs(ycor) > max-pycor - 1 [die]

  ;; evitar que ingrese a las vias o se pegue a los laterales
  ifelse abs(xcor) < 6.05 or abs(xcor) > max-pxcor - 1
  [rt 180 set limite 1 fd 0.03]
  [if round(xcor) mod 3 = 0 [
    orientar_anden_dest
    rt random 4
  ]]

  ;; avanzar 1 pixel
  ;; if ticks mod 2 = 0
  fd 0.03

  ;; si estoy en el comienzo de un puente
  if (ycor > 11 and ycor < 11.5) and abs(xcor) < 9 and anden_dest != anden_actual
  [set estado 1 cambiar_anden]

  ;; if limite = 0 and abs(xcor) > 10 [rt random 360]
end

to orientar_anden_dest
  ;; si el anden que quiere ir es el otro
  if anden_dest != anden_actual
  [
    ifelse anden_actual = 0 [facexy -8 11.25] [facexy 8 11.25]
    ;;if ycor < 11.25 [set heading 0]
  ]
end

to cambiar_anden
  ifelse anden_actual = 0 [set heading 90 set anden_actual 1]
  [set heading 270 set anden_actual 0]

  set color yellow
end

to cruzar_puente
  fd 0.03
  if abs(xcor) > 9 [set estado 2 set color green]
end

to acercar_pasajero
  let y_random 5 - (who mod 15)
  let x_random 0

  let y_subida 2 - (who mod 4)
  let x_subida 0

  let speed 0
  let llego_tren? [estado] of tren anden_dest = 1

  ifelse anden_dest = 0
  [set x_random -6 - (who mod 10) set x_subida -6]
  [set x_random 6 + (who mod 10) set x_subida 6]

  ifelse llego_tren? [set speed 0.01] [set speed 0.004]

  ifelse llego_tren? [
    ;; el tren a tomar está en el andén
    ifelse abs(xcor) > 6 [facexy x_subida y_subida fd speed] [die]
  ][
    ;; el tren a tomar no está en el andén
    ;; esperar en un sitio random
    if distancexy x_random y_random > 1 [facexy x_random y_random fd speed]
  ]
end


;; ------------------------------------------ RESPUESTAS A BOTONES ----------------------------

to setup
  clear-all
  setup-patches
  setup-trenes
  reset-ticks
end


;; para cada tick del sistema
to go
  ;; actualización de trenes según su estado (todo ok)
  ask trenes with [estado = 0] [update_tren_en_marcha]
  ask trenes with [estado = 1] [update_tren_detenido]
  ask trenes with [estado = 2] [update_tren_lejano]

  ;; actualización de pasajeros según su estado
  ask pasajeros with [estado = 10] [update_pasajero_saliendo]


  ;; actualizar pasajeros que están vagando o están en el andén contrario
  ask pasajeros with [estado = 0] [
    ifelse anden_dest != anden_actual and [espera] of tren anden_dest < 2000
    [if ticks mod 4 = 0 [mover_pasajero] ]
    [if ticks mod 10 = 0 [mover_pasajero] ]
  ]

  ;; actualizar pasajeros que están en el puente
  ask pasajeros with [estado = 1] [
    ifelse [espera] of tren anden_dest < 1000 [if ticks mod 4 = 0 [cruzar_puente] ]
    [if ticks mod 10 = 0 [cruzar_puente] ]
  ]

  ;; actualizar pasajeros que están en el andén correcto
  ask pasajeros with [estado = 2] [acercar_pasajero]

  ;; update 4/11 - creación de pasajeros al llegar un tren
  ;; revisar periodo de esta tarea (cada cuantos ticks crear uno)
  if (ticks mod 50 = 0 and pasajeros_restantes_izq > 0) [create-pasajeros 1 [crear_pasajero_izq] ]
  if (ticks mod 50 = 25 and pasajeros_restantes_der > 0) [create-pasajeros 1 [crear_pasajero_der] ]

  tick
end


;; --------------------------------------------- MONITORES -------------------------------------------

to-report mostrar-hora-actual
  let hora floor(ticks / 60000) + 8
  let minutos floor((ticks mod 60000) / 1000)

  ifelse (minutos < 10)
    [report (word hora ":0" minutos)]
  [report (word hora ":" minutos)]
end
@#$#@#$#@
GRAPHICS-WINDOW
210
10
647
448
-1
-1
13.0
1
10
1
1
1
0
0
0
1
-16
16
-16
16
0
0
1
ticks
30.0

BUTTON
703
179
766
212
NIL
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
850
179
913
212
NIL
go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
18
81
190
114
tolerancia_anden
tolerancia_anden
500
4500
500.0
100
1
NIL
HORIZONTAL

SLIDER
18
123
190
156
frecuencia_tren
frecuencia_tren
10000
40000
10000.0
1000
1
NIL
HORIZONTAL

SLIDER
18
171
190
204
cant_bajan
cant_bajan
0
50
10.0
1
1
NIL
HORIZONTAL

PLOT
4
217
204
423
Pasajeros en la estación
ticks
count pasajeros
0.0
10.0
0.0
10.0
true
false
"" ""
PENS
"Andén Izq" 1.0 0 -10899396 true "" "plot count pasajeros with [elevacion = 1 and anden = 0]"
"Andén Der" 1.0 0 -11221820 true "" "plot count pasajeros with [elevacion = 1 and anden = 1]"
"Puente" 1.0 0 -2674135 true "" "plot count pasajeros with [elevacion = 2]"

MONITOR
676
23
791
68
PLAZA C
word [round(espera / 1000)] of tren 0 \" min\"
17
1
11

MONITOR
676
92
793
137
LA PLATA / BOSQUES
word [round(espera / 1000)] of tren 1 \" min\"
17
1
11

MONITOR
807
21
964
66
Actividad en Tren Norte
word pasajeros_restantes_izq \" por salir\"
17
1
11

MONITOR
808
93
960
138
Actividad en Tren Sur
word pasajeros_restantes_der \" por salir\"
17
1
11

MONITOR
53
17
154
62
HORA ACTUAL
mostrar-hora-actual
17
1
11

@#$#@#$#@
## WHAT IS IT?

(a general understanding of what the model is trying to show or explain)

## HOW IT WORKS

(what rules the agents use to create the overall behavior of the model)

## HOW TO USE IT

(how to use the model, including a description of each of the items in the Interface tab)

## THINGS TO NOTICE

(suggested things for the user to notice while running the model)

## THINGS TO TRY

(suggested things for the user to try to do (move sliders, switches, etc.) with the model)

## EXTENDING THE MODEL

(suggested things to add or change in the Code tab to make the model more complicated, detailed, accurate, etc.)

## NETLOGO FEATURES

(interesting or unusual features of NetLogo that the model uses, particularly in the Code tab; or where workarounds were needed for missing features)

## RELATED MODELS

(models in the NetLogo Models Library and elsewhere which are of related interest)

## CREDITS AND REFERENCES

(a reference to the model's URL on the web if it has one, as well as any other necessary credits, citations, and links)
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123
Circle -16777216 true false 405 -30 30
Circle -16777216 true false 39 114 42
Line -16777216 false 120 150 90 150
Circle -16777216 true false 84 129 42
Circle -16777216 true false 225 75 30
Circle -16777216 true false 123 78 85

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pasajero
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105
Circle -1 true false 120 15 30
Circle -1 true false 150 15 30
Line -16777216 false 120 90 150 150
Line -16777216 false 180 90 150 150
Circle -1184463 true false 150 150 0
Circle -1184463 true false 135 150 30
Circle -16777216 true false 135 30 0
Rectangle -16777216 true false 135 30 135 30

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

sheep
false
15
Circle -1 true true 203 65 88
Circle -1 true true 70 65 162
Circle -1 true true 150 105 120
Polygon -7500403 true false 218 120 240 165 255 165 278 120
Circle -7500403 true false 214 72 67
Rectangle -1 true true 164 223 179 298
Polygon -1 true true 45 285 30 285 30 240 15 195 45 210
Circle -1 true true 3 83 150
Rectangle -1 true true 65 221 80 296
Polygon -1 true true 195 285 210 285 210 240 240 210 195 210
Polygon -7500403 true false 276 85 285 105 302 99 294 83
Polygon -7500403 true false 219 85 210 105 193 99 201 83

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

tren
true
0
Rectangle -13791810 true false 105 45 195 105
Rectangle -1 true false 105 15 195 45
Rectangle -13345367 true false 120 60 180 90
Rectangle -7500403 true true 105 0 195 15
Rectangle -11221820 true false 105 105 195 300
Rectangle -1184463 true false 165 45 180 45
Rectangle -1184463 true false 165 15 180 30
Rectangle -1184463 true false 120 15 135 30
Rectangle -13791810 true false 105 150 195 165
Rectangle -13791810 true false 105 225 195 240

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

wolf
false
0
Polygon -16777216 true false 253 133 245 131 245 133
Polygon -7500403 true true 2 194 13 197 30 191 38 193 38 205 20 226 20 257 27 265 38 266 40 260 31 253 31 230 60 206 68 198 75 209 66 228 65 243 82 261 84 268 100 267 103 261 77 239 79 231 100 207 98 196 119 201 143 202 160 195 166 210 172 213 173 238 167 251 160 248 154 265 169 264 178 247 186 240 198 260 200 271 217 271 219 262 207 258 195 230 192 198 210 184 227 164 242 144 259 145 284 151 277 141 293 140 299 134 297 127 273 119 270 105
Polygon -7500403 true true -1 195 14 180 36 166 40 153 53 140 82 131 134 133 159 126 188 115 227 108 236 102 238 98 268 86 269 92 281 87 269 103 269 113

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270
@#$#@#$#@
NetLogo 6.3.0
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180
@#$#@#$#@
0
@#$#@#$#@
