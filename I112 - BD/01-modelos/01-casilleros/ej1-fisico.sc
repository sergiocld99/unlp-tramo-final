// ENTIDADES
PERSONA("dni", nombre, apellido, estCivil, fechaNac, calle, nro, piso?, dpto?)
SOCIO("dni(fk)", nro, fechaAlta)
ASISTENTE("dni(fk)", ultimoAcceso)
TELEFONO("telefono")

ACTIVIDAD("nombre", descripcion, fechaIni, fechaFin)
CASILLERO("cod", ubicacion, tamaño)
CASILLERO_JOCKEY("cod(fk)")

JUGADOR("dni", posJuego, fechaNac, calle, nro, piso?, dpto?)
ENFERMEDAD("enfermedad")
ALERGIA("alergia")
DIVISION("nombre")

// Relaciones muchos a muchos
CONTACTO("dni(fk), telefono(fk)")
TIENE_VINCULO("dni_socio(fk), dni_asistente(fk)")
INSCRIPTO("dni(fk), nombre")                            // nombre -> de la actividad
TIENE_ACCESO("dni(fk), cod(fk)")                        // cod -> del casillero
CRONOLOGIA("dni(fk), cod(fk), fechaIni", fechaFin?)     // idem cod, dni -> de jugador
ALERGICO("dni(fk), alergia(fk)")
ENFERMO("dni(fk), enfermedad(fk)")

// Relaciones opcional a muchos
CORRESPONDE("cod(fk)", nombre(fk))                      // nombre -> de la división
JUEGA_EN("dni(fk)", nombre(fk))                         // idem nombre, dni -> de jugador