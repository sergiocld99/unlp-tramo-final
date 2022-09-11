// Entidades y relaciones con (1,1)
PERSONA("DNI", nombre, apellido)
EMPLEADO("DNI(fk)", nro_empleado)
CLIENTE("DNI(fk)")
TELEFONO("telefono")

SUCURSAL("nombre", calle, nro, piso?, dpto?)
VENTA("nroTicket", online, total, fecha, DNI(fk), nombre(fk))
PRODUCTO("codigo", nombre, descripcion, color, precio)

// Relaciones muchos a muchos
CONTACTO("DNI(fk), telefono(fk)")
TRABAJA("DNI(fk), nombre(fk), fecha_inicio", fecha_fin?)
INTERVINO("DNI(fk), nroTicket(fk)")                         // dni de empleado
CONTIENE("nroTicket(fk), codigo(fk)", precio, cantidad)     // código de producto