// COMILLAS = SUBRAYADO

/* Jerarquia de Persona */
PERSONA("dni", apellido, nombre)
PACIENTE("dni(fk)")
MEDICO("dni(fk)", matricula)

/* Persona - Teléfono */
TELEFONO("telefono")
CONTACTO("dni(fk), telefono(fk)")

/* Paciente - Médico */
MED_CABECERA("dniPaciente(fk)", dniMedico(fk))

/* Médico - Especialización */
ESPECIALIZACION("nombre")
TIENE("dni(fk), nombre(fk)")

/* Médico - Clinica */
CLINICA("razon_social", calle, nro, piso?, dpto?)
TRABAJA("dni(fk), razon_social(fk), fecha_inicio", fecha_fin?)

// Comentario: fecha_inicio de trabaja debe pertenecer a la clave primaria
// porque puede ocurrir que un médico vuelva a trabajar en una clinica en otra ocasión