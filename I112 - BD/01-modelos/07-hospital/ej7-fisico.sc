// Entidades
ENFERMERO("matricula", nombre, apellido, dni, fechaNac, #hijos)
NINO("dni", nombre, apellido, peso, edad, genero)
MEDICAMENTO("nombreCom, presentacion", nombreGen, nombreLab)
HABITACION("nro", tamano, ubicacion)
SERVICIO("descripcion")

VISITANTE("dni", nombre, apellido)
AUTORIZADO("dni(fk)")
TUTOR("dni(fk)", direccion, vinculo)            // vinculo deberia ser una relación!!!
TELEFONO("telefono")

VISITA("dni(fk)", fecha, hora, dniVisitante(fk))
TOMA("dni(fk)", fecha, hora, matricula(fk), nombreCom(fk), presentacion(fk))

// Relaciones opcional a muchos
SUPERVISA("matricula(fk), dniNino(fk)")

// Relaciones muchos a muchos
REQUIERE("nombreCom(fk), presentacion(fk), dni(fk)")
ALOJA("nro(fk), dni(fk)")
SE_SIRVE("nro(fk), descripcion(fk)")
CONTACTO("telefono(fk), dni(fk)")