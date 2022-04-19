// Entidades
HOTEL("codHotel", ciudad, pais, cantEstrellas, calle, nro)
RESTAURANTE("descripcion")
SERVICIO("nombre", descripcion, costoDia)

PERSONA("#pasaporte", apellido, nombre, telefono, calle, nro, piso?, dpto?)
EMPLEADO("#pasaporte(fk)", nroEmp, fechaIngreso, cantHijos, nacionalidad)
CLIENTE("#pasaporte(fk)")
EMAIL("email")
AREA_HOTEL("nombre", descripcion, codHotel(fk))     // "pertenece" incluido

HABITACION("nroHab, piso", ubicacion, descripcion, codHotel(fk), nombreTipo(fk))
TIPO_HAB("nombreTipo")

RESERVA("#pasaporte(fk), fechaInicio", cantHuespedesMayores, cantHuespedesMenores, fechaFin, valorTotal)
ESTADIA("#pasaporte(fk), fechaInicio(fk), fechaIngreso", fechaSalida?, saldo)
TARJETA("nroTarj", codSeg, banco, entidad, tipo)

OCUPACION("nroHab(fk), piso(fk), #pasaporte(fk), fechaInicio(fk), fechaIngreso(fk)")

// Relaciones muchos a muchos
POSEE_RESTO("codHotel(fk), descripcion(fk)")
OFRECE("codHotel(fk), nombre(fk)")              // nombre -> del servicio
CONTACTO("#pasaporte(fk), email(fk)")
TRABAJA("#pasaporte(fk), nombre(fk)", fechaIni, fechaFin?)           // nombre -> del área
SE_RESERVA("nombreTipo(fk), #pasaporte(fk)")}

// Relaciones opcional a muchos
SEÑADA_CON("nroTarj(fk), #pasaporte(fk), fechaInicio(fk)", cuotas?)
PAGADA_CON("nroTarj(fk), #pasaporte(fk), fechaInicio(fk), fechaIngreso(fk)", cuotas?)

// CONSULTAR POR OCUPACIÓN: #PASAPORTE(FK) REPETIDO???