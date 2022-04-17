// Entidades
DEPORTISTA("dni", nombre, apellido, fechaNac, direccion)
SOCIO("dni(fk)", #socio, estCivil, fechaAlta)
TELEFONO("telefono")

DEPORTE("nombre", descuentoSocios, horarioDiasHabiles, horarioFinde?)
INCIDENTE("dni(fk), fecha, hora", descripcion, huboAsistencia, nombre(fk))      // nombre -> del deporte
ASISTENCIA("dni(fk), fecha", presente, nombre(fk))                              // idem
ABONO("nombre(fk), frecuencia", precio)                                         // idem

// Relaciones muchos a muchos
CONTACTO("dni(fk), telefono(fk)")
A_CARGO("dni(fk), dni_subordinado(fk)")
ELIGE("dni(fk), nombre(fk), frecuencia, fechaIni", fechaFin?, alDia)            // nombre -> del deporte

// DUDA: ¿Es necesario tener registro de los costos de abono pasados? (agregaría precio en "elige")