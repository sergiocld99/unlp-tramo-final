// COMILLAS = SUBRAYADO

// Entidades
PERSONA("dni", apellido, nombre)
PACIENTE("dni(fk)")
MEDICO("dni(fk)", matricula)

TELEFONO("telefono")
ESPECIALIZACION("nombre")
CLINICA("razon_social", calle, nro, piso?, dpto?)

// Relaciones muchos a muchos
CONTACTO("dni(fk), telefono(fk)")
TIENE("dni(fk), nombre(fk)")
TRABAJA("dni(fk), razon_social(fk), fecha_inicio", fecha_fin?)

// Relaciones opcional a muchos
MED_CABECERA("dniPaciente(fk)", dniMedico(fk))

// Comentario: fecha_inicio de trabaja debe pertenecer a la clave primaria
// porque puede ocurrir que un médico vuelva a trabajar en una clinica en otra ocasión