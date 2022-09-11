// Entidades
EMPLEADO("dni", cuit, fechaNac, cantHijos, fechaIngreso, calle, nro, piso?, dpto?)
GERENTE("dni(fk)")
TITULO("titulo")
TELEFONO("telefono")

AREA("codArea", nombreArea)
DESC_FUNCION("descFuncion")

PROYECTO("nombreProy", fechaIni, fechaFinEst, costoEst, nombreTipo(fk))
TIPO_PROYECTO("nombreTipo", descripcion)
CARGO("codCargo", nombre, descripcion)
ASIGNACION("nombreProy(fk), dni(fk), codCargo(fk)", fechaIni, fechaFin?, cantHoras)

// Relaciones muchos a muchos
POSEE_TITULO("dni(fk), titulo(fk)")
CONTACTO("dni(fk), telefono(fk)")
MANEJA("dni(fk), codArea(fk), fechaIni", fechaFin?)
TRABAJA("dni(fk), codArea(fk), fechaIni", fechaFin?)
POSEE_FUNCION("codArea(fk), descFuncion(fk)")

// Relaciones opcional a muchos -> no hay