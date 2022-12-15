;; Actualmente tenemos 1 min = 1000 ticks

;; variables globales inicializadas por defecto en cero
globals [
  pasajeros_restantes_izq
  pasajeros_restantes_der
  hora_actual
]


;; -------------------- DEFINICIÓN DE AGENTE ESTACIONARIO ---------------------------------

patches-own [ elevacion anden lugar_espera libre? ]


;; -------------------- DEFINICIÓN DE AGENTES MÓVILES -----------------------------

;; trenes: estado 0 (yendo), 1 (en anden), 2 (lejano)
;;  estado 0: en marcha (se acerca o aleja del centro de la estación)
;;  estado 1: detenido (se encuentra en el andén durante un cierto tiempo)
;;  estado 2: lejano (es invisible durante un cierto tiempo)
trenes-own [ estado espera t_anden ]

;; pasajeros
;;  estado 10: saliendo de la estación (se dirige hacia los molinetes)
;;  estado 11: yendo a puente (se dirige hacia el comienzo del puente)
;;  estado 12: cruzando puente (se dirige hacia el andén destino)
;;  estado 13: esperando tren (se dirige y espera en un lugar random del andén)
;;  estado 14: subiendo a tren (se dirige hacia el tren si está en el andén)
pasajeros-own [ estado anden_dest edad molinete x_elegida lugar_elegido ]

;; plurales
breed [trenes tren]
breed [pasajeros pasajero]


;; -------------------------- SETUP ESTACIONARIO ---------------------------

to setup-patches
  ask patches with [ abs(pxcor) < 6 ] [set elevacion 0]
  ask patches with [ abs(pxcor) >= 6 ] [set elevacion 1]
  ask patches with [ pycor >= 10 and pycor <= 12 and abs(pxcor) < 8 ] [set elevacion 2]
  ask patches with [ elevacion = 1 and pxcor < 0 ] [set anden 0]
  ask patches with [ elevacion = 1 and pxcor > 0 ] [set anden 1]

  ;; definir zonas de espera
  let x_posibles (list 9 10 14 15 16)

  ask patches with [ elevacion = 1 ] [
    if (member? abs(pxcor) x_posibles ) [
      if (pycor >= -6 and pycor <= 6 ) [
        set lugar_espera 1
        set libre? true
      ]
    ]
  ]

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


;; configuración común para todos los pasajeros
to crear_pasajero_common
  set shape "pasajero"
  set size 2

  ;; establecer edad (aleatoria entre 15 y 85)
  set edad (15 + random 70)
  set label edad
end


;; parámetros comunes en ambos andenes
;; para evitar choques con los que ingresan al tren, se crean en "y" par
to crear_pasajero_from_tren
  crear_pasajero_common
  set ycor 4 - 2 * (who mod 5)

  ;; solo crear pasajeros si hay espacio
  if (any? pasajeros-on neighbors) [die]
end


;; pasajero que baja del tren lado izquierdo
to crear_pasajero_izq
  set xcor -6
  crear_pasajero_from_tren

  set heading 270

  ;; elección de tipo según celdas libres
  let libres (patches with [anden = 1 and libre? = true] )

  ifelse (random 100 >= porcentaje_cruzan or count libres = 0 or hora_actual >= 21) [
    set estado 10
    ifelse (ycor > 0) [set molinete 16] [set molinete -16]
  ]
  [
    set estado 11
    set anden_dest 1
    set x_elegida -8 - who mod 3
    set lugar_elegido one-of libres
    ask lugar_elegido [set libre? false]
  ]

  ;; actualizar contador global
  set pasajeros_restantes_izq (pasajeros_restantes_izq - 1)
end


;; pasajero que baja del tren lado derecho
to crear_pasajero_der
  set xcor 6
  crear_pasajero_from_tren

  set heading 90

  ;; desde este lado no hacen combinación
  set estado 10
  ifelse (ycor > 0) [set molinete 16] [set molinete -16]

  ;; actualizar contador global
  set pasajeros_restantes_der (pasajeros_restantes_der - 1)
end


;; pasajero que ingresa por alguno de los 4 molinetes
to crear_pasajero_ingreso
  crear_pasajero_common

  ;; verificar espacio disponible
  let libres_izq (patches with [anden = 0 and libre? = true])
  let libres_der (patches with [anden = 1 and libre? = true])

  ;; si no hay lugar, directamente descartar creación
  if (count libres_izq = 0 and count libres_der = 0) [die]

  ;; elegir el andén con más espacio disponible
  ifelse (count libres_izq > count libres_der) [
    ;; hay más espacio en andén 0
    set anden_dest 0
    set lugar_elegido one-of libres_izq

    ;; opción 1: generar en andén 1 y cruzar puente
    ;; opción 2: generar en anden 0 y esperar ahi
    ifelse (random 100 < porcentaje_cruzan) [
      set estado 11
      set xcor 11
    ] [
      set estado 13
      set xcor ([pxcor] of lugar_elegido)
    ]
  ] [
    ;; hay más espacio en andén 1
    set anden_dest 1
    set lugar_elegido one-of libres_der

    ;; opción 1: generar en anden 0 y cruzar puente
    ;; opción 2: generar en andén 1 y esperar ahi
    ifelse (random 2 = 0) [
      set estado 11
      set xcor -11
    ] [
      set estado 13
      set xcor ([pxcor] of lugar_elegido)
    ]
  ]

  ;; acciones comunes
  ask lugar_elegido [set libre? false]
  set x_elegida xcor                       ;; para estado "hacia puente"

  ;; desde arriba o abajo
  ifelse (random 2 = 0) [set ycor 16] [set ycor -16]
end


;; ------------------------- UPDATE DE TRENES SEGÚN ESTADO ---------------------------

;; TREN - ESTADO 0
to update_tren_en_marcha
  ;; Realizar accion del estado (avanzar)
  fd 8 * (abs(ycor) + 1) / 50

  ;; Chequear entradas
  ;; Camino 1 - ¿Se encuentra en el lugar de espera?
  if (abs(ycor) < 0.08) [
    set estado 1                    ;; cambiar a estado "detenido"
    set t_anden tolerancia_anden    ;; asignar tiempo de espera

    ifelse (who = 0)                ;; indicar cuantos pasajeros deben crearse
      [set pasajeros_restantes_izq cant_bajan]
      [set pasajeros_restantes_der cant_bajan]
  ]

  ;; Camino 2 - ¿Se alejó el tren lo suficiente?
  if (abs(ycor) >= max-pycor) [
    set estado 2                                         ;; cambiar a estado "lejano"
    set espera (frecuencia_tren + random 400 - 300)      ;; asignar tiempo hasta próximo arribo
    set hidden? true                                     ;; volver invisible al usuario
  ]
end


;; TREN - ESTADO 1
to update_tren_detenido
  ;; Realizar acción del estado (disminuir espera si nadie baja)
  if (who = 0 and pasajeros_restantes_izq = 0) [set t_anden (t_anden - 8)]
  if (who = 1 and pasajeros_restantes_der = 0) [set t_anden (t_anden - 8)]

  ;; Camino 1 - ¿Se cumplió el tiempo de espera?
  if (t_anden <= 0) [
    set estado 0              ;; pasar al estado "en marcha"
    fd 0.16                   ;; avanzar para evitar bloqueo
  ]
end


;; TREN - ESTADO 2
to update_tren_lejano
  ;; Mientras no se alcance las 22 hs, seguir moviendo trenes
  if (hora_actual < 22) [
    ;; Realizar acción del estado (disminuir espera)
    set espera (espera - 8)

    ;; Camino 1 - ¿Se cumplió el tiempo de espera?
    if (espera <= 0) [
      ifelse who = 0 [set ycor min-pycor] [set ycor max-pycor]
      set hidden? false
      set estado 0
    ]
  ]

end


;; ------------------------- UPDATE DE PASAJEROS SEGÚN ESTADO ---------------------------

;; PASAJERO - ESTADO 10
;; las coordenadas x=12 a x=16 están reservadas para dirigirse a los molinetes
to update_pasajero_saliendo
  ;; Realizar acción del estado (acercarse a la salida)
  let signo (anden * 2 - 1)
  let x_esperada (12 + who mod 2) * signo

  ifelse (pxcor != x_esperada)
    [facexy x_esperada ycor]
    [facexy xcor molinete]

  ;; avanzar solo si no hay nadie delante
  avanzar_si_hay_espacio

  ;; Camino 1 - ¿Llegó al molinete?
  if (abs(ycor) >= max-pycor) [die]

end


;; PASAJERO - ESTADO 11
;; las coordenadas x=8 a x=11 están reservadas para dirigirse a la escalera
to update_pasajero_hacia_puente
  ;; Realizar acción del estado (acercarse a escalera)
  let signo (xcor / abs(xcor))
  let x_puente (7 * signo)
  let x_esperada x_elegida
  let y_esperada 11 + anden_dest mod 2

  ;; ifelse (ycor > 13) [set y_esperada 13] [set y_esperada (11 + who mod 2)]

  ;; ordenado según distancia al puente
  ifelse (pycor = y_esperada) [facexy x_puente y_esperada ]
  [
    ifelse (pxcor = x_esperada )
      [facexy xcor y_esperada]
      [facexy x_esperada ycor]
  ]

  ;; avanzar solo si no hay nadie delante
  avanzar_si_hay_espacio

  ;; Camino 1 - ¿Llego al puente?
  if (pycor = y_esperada) [
    set estado 12                 ;; cambiar a estado "cruzando"
    set color yellow              ;; color amarillo en contraste al puente
    ifelse (anden_dest = 1)
      [set heading 90]
      [set heading 270]
  ]
end


;; PASAJERO - ESTADO 12
to update_pasajero_cruzando
  ;; Realizar acción del estado (ir hacia andén destino)
  ;; avanzar solo si no hay nadie delante
  avanzar_si_hay_espacio

  ;; Si llegó al andén destino...
  if (abs(xcor) > 8) [
    set estado 13
    set color blue
  ]
end


;; PASAJERO - ESTADO 13
;; Para evitar choques con los que bajan, esperan en "y" impar
to update_pasajero_esperando
  ;; Realizar acción del estado (ubicarse en un lugar)
  let signo (anden * 2 - 1)
  let x_espera [pxcor] of lugar_elegido
  let y_espera [pycor] of lugar_elegido
  let avanzar? false

  ;; primero moverse a la coordenada "x" correcta
  ;;ifelse (abs(xcor - x_espera) > 0.8) [
  ifelse (pxcor != x_espera) [
    facexy x_espera ycor
    set avanzar? true
  ] [
    if (pycor != y_espera) [
      facexy xcor y_espera
      set avanzar? true
    ]
  ]

  if (avanzar?) [avanzar_si_hay_espacio]

  ;; Cambiar a estado "subiendo a tren" si llegó
  if (patch-here = lugar_elegido and [estado] of tren anden = 1) [set estado 14]
end


;; PASAJERO - ESTADO 14
;; No se acercarán al tren hasta que terminen de bajar
;; Para evitar choques con los que bajan, se sube por "y" impar
to update_pasajero_subiendo
  ;; Realizar acción del estado (acercarse a tren)
  let x_subida 6 * (anden * 2 - 1)
  ;; let y_subida 6 - (who * 2 + 1) mod 10
  let habilitado 0

  if (anden = 0 and pasajeros_restantes_izq = 0) [set habilitado 1]
  if (anden = 1 and pasajeros_restantes_der = 0) [set habilitado 1]

  if (habilitado = 1 and pxcor != x_subida) [
    facexy x_subida ycor
    avanzar_si_hay_espacio
  ]

  ;; Cambiar a estado "esperando" si se fue el tren
  if ([estado] of tren anden != 1) [set estado 13]

  ;; Eliminar agente si ya se encuentra en el tren
  if (abs(xcor) <= 6.8) [eliminar_pasajero]
end


;; ------------------------------------------ RESPUESTAS A BOTONES ----------------------------

to setup
  clear-all
  setup-patches
  setup-trenes
  reset-ticks

  set hora_actual hora_inicio

  ;; establecer cantidad que bajan según mes
  cant_bajan_segun_mes

end


;; para cada tick del sistema
to go

  let t (ticks mod 8)

  (ifelse
    ;; actualización de trenes según su estado
    t = 0 [ ask trenes with [estado = 0] [update_tren_en_marcha] ]
    t = 1 [ ask trenes with [estado = 1] [update_tren_detenido] ]
    t = 2 [ ask trenes with [estado = 2] [update_tren_lejano] ]

    ;; actualización de pasajeros según su estado
    t = 3 [ ask pasajeros with [estado = 10] [update_pasajero_saliendo] ]
    t = 4 [ ask pasajeros with [estado = 11] [update_pasajero_hacia_puente] ]
    t = 5 [ ask pasajeros with [estado = 12] [update_pasajero_cruzando] ]
    t = 6 [ ask pasajeros with [estado = 13] [update_pasajero_esperando] ]
    t = 7 [ ask pasajeros with [estado = 14] [update_pasajero_subiendo] ]
  )

  ;; update 4/11 - creación de pasajeros que bajan del tren
  ;; se crean cada 5 ticks = 5/100 min = 0.05 min = 3 segundos
  if (ticks mod 5 = 0 and pasajeros_restantes_izq > 0) [ create-pasajeros 1 [crear_pasajero_izq] ]
  if (ticks mod 5 = 2 and pasajeros_restantes_der > 0) [ create-pasajeros 1 [crear_pasajero_der] ]

  ;; generación espontánea de pasajeros (desde molinetes)
  ;; se crean cada 100 ticks = 1 minuto
  if (ticks mod 100 = 99 and ingresantes = true and hora_actual < 21) [create-pasajeros 1 [crear_pasajero_ingreso] ]

  ;; actualizar frecuencia (solo 1 vez por hora)
  if (ticks mod 6000 = 1) [
    set hora_actual floor(ticks / 6000) + hora_inicio

    if (autocorregir) [autocorregir_valores]

    ;; si son las 11 PM, terminar simulación
    if (hora_actual >= 23) [stop]
  ]

  ;; incrementar ticks
  tick
end


;; --------------------------------------------- FUNCIONES AUXILIARES ---------------------------------

to avanzar_si_hay_espacio
  ifelse (patch-ahead 1 = nobody or count pasajeros-on patch-ahead 1 < 3) [
    avanzar_segun_edad
    if (elevacion = 2) [set color yellow]
    if (elevacion = 1 and anden = 0) [set color green]
    if (elevacion = 1 and anden = 1) [set color blue]
  ] [
    set color orange
  ]
end

to avanzar_segun_edad
  ifelse (edad < 50) [fd 0.8] [fd 0.4]
end


to elegir_lugar_espera
  let aux anden
  let candidato nobody

  set candidato (
    one-of patches with [libre? = true and anden = aux and lugar_espera = 1]
  )

  if (candidato != nobody) [
    ask candidato [set libre? false]
    set lugar_elegido candidato
  ]

end

to eliminar_pasajero
  if (lugar_elegido != nobody) [
    ask lugar_elegido [set libre? true]
  ]

  die
end

to autocorregir_valores
  ;; corregir cantidad que bajan según sea hora pico o no
  cant_bajan_segun_mes

  (ifelse
    hora_actual = 5 [set frecuencia_tren 1200]      ;; cada 12 min
    hora_actual = 12 [set frecuencia_tren 1500]     ;; cada 15 min
    hora_actual = 17 [set frecuencia_tren 1200]     ;; cada 12 min
    hora_actual = 21 [set frecuencia_tren 1500]     ;; cada 15 min
  )
end


;; ------------------------------------------ CONSULTA DE DATOS REALES ------------------------------

to cant_bajan_segun_mes
  let total 0

  (ifelse
    mes = "Enero"      [set total 212613]
    mes = "Febrero"    [set total 231473]
    mes = "Marzo"      [set total 263145]
    mes = "Abril"      [set total 275146]
    mes = "Mayo"       [set total 285549]
    mes = "Junio"      [set total 218172]
    mes = "Julio"      [set total 197322]
    mes = "Agosto"     [set total 288444]
    mes = "Septiembre" [set total 308205]
    mes = "Octubre"    [set total 311335]
    mes = "Noviembre"  [set total 308932]
    mes = "Diciembre"  [set total 286682]
  )

  ;; Cálculo de la hora pico: de los 80 trenes que pasan por un andén
  ;; 55 de ellos pasan en hora pico (casi un 70%)
  ;; suponemos que en hora pico viaja el doble de gente que en hora no pico
  ;; resulta entonces que el 80% de los pasajeros viaja en hora pico

  let hora_pico (hora_actual < 12 or (hora_actual >= 17 and hora_actual < 21))

  ifelse (hora_pico)
    [set cant_bajan round(total * 0.8 / (30 * 110 * 2))]
    [set cant_bajan round(total * 0.2 / (30 * 50 * 2))]

end


;; --------------------------------------------- MONITORES -------------------------------------------

to-report mostrar-hora-actual
  let minutos floor((ticks mod 6000) / 100)

  ifelse (minutos < 10)
    [report (word hora_actual ":0" minutos)]
  [report (word hora_actual ":" minutos)]
end
@#$#@#$#@
GRAPHICS-WINDOW
233
25
670
463
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
149
25
212
58
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
150
65
213
98
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
22
206
202
239
tolerancia_anden
tolerancia_anden
100
500
200.0
10
1
ticks
HORIZONTAL

SLIDER
21
247
200
280
frecuencia_tren
frecuencia_tren
1000
4000
4000.0
100
1
ticks
HORIZONTAL

SLIDER
20
289
192
322
cant_bajan
cant_bajan
0
50
38.0
1
1
pasajeros
HORIZONTAL

PLOT
702
255
1044
461
Pasajeros en la estación
ticks
count pasajeros
0.0
6000.0
0.0
30.0
true
false
"" ""
PENS
"Andén Izq" 1.0 0 -10899396 true "" "plot count pasajeros with [elevacion = 1 and anden = 0]"
"Andén Der" 1.0 0 -11221820 true "" "plot count pasajeros with [elevacion = 1 and anden = 1]"
"Puente" 1.0 0 -2674135 true "" "plot count pasajeros with [elevacion = 2]"

MONITOR
704
74
857
119
Próximo Tren
word [round(espera / 100)] of tren 0 \" min\"
17
1
11

MONITOR
880
73
1034
118
Próximo Tren
word [round(espera / 100)] of tren 1 \" min\"
17
1
11

MONITOR
702
134
857
179
Actividad en Tren
word \npasajeros_restantes_izq \" por salir, \"\ncount pasajeros with [estado >= 13 and anden = 0] \" por entrar\"
17
1
11

MONITOR
880
133
1033
178
Actividad en Tren
word pasajeros_restantes_der \" por salir, \"\ncount pasajeros with [estado >= 13 and anden = 1] \" por entrar\"
17
1
11

MONITOR
22
37
138
94
HORA ACTUAL
mostrar-hora-actual
17
1
14

TEXTBOX
915
35
994
65
ANDEN 1
16
0.0
1

TEXTBOX
737
35
817
75
ANDEN 0
16
0.0
1

MONITOR
881
194
1036
239
Lugares libres
count patches with [anden = 1 and libre? = true]
17
1
11

MONITOR
704
193
855
238
Lugares libres
count patches with [anden = 0 and libre? = true]
17
1
11

SLIDER
22
113
194
146
hora_inicio
hora_inicio
5
21
5.0
1
1
:00
HORIZONTAL

CHOOSER
21
153
195
198
mes
mes
"Enero" "Febrero" "Marzo" "Abril" "Mayo" "Junio" "Julio" "Agosto" "Septiembre" "Octubre" "Noviembre" "Diciembre"
9

SWITCH
21
415
193
448
ingresantes
ingresantes
0
1
-1000

SLIDER
20
328
192
361
porcentaje_cruzan
porcentaje_cruzan
0
100
100.0
1
1
%
HORIZONTAL

SWITCH
21
376
193
409
autocorregir
autocorregir
1
1
-1000

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
