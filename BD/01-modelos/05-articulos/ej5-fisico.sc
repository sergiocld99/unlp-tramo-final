// Entidades
CLIENTE("nroCliente")
PERSONA("nroCliente(fk)", dni, nombre, apellido, email, tel, calle, nro, piso?, dpto?)
EMPRESA("nroCliente(fk)", razonSoc, mail, sitioWeb?)
TELEFONO("telefono")

PRODUCTO("cod", tipo, descripcion, pAlquiler, pVen?)
COMPRA("#ticket", fecha, seña, valorTotal, nroCliente(fk))      // "realiza" fusionado
ALQUILER("nroCliente(fk), fechaAlq, fechaDev", costoTotal)      // "solicita" ya incluido
ENVIO("nroCliente(fk), fechaAlq(fk), fechaDev(fk)", direccion, costo)   // "enviado" incluido
TARJETA("nroTarj", codSeg, banco, entidad, tipo)

// Relaciones muchos a muchos
CONTACTO("nroCliente(fk), telefono(fk)")
ALQUILADO("cod(fk), nroCliente(fk), fechaAlq(fk), fechaDev(fk)", estado)
COMPRADO("cod(fk), #ticket(fk)")

// Relaciones opcional a muchos
PAGA_ALQUILER("nroTarj(fk), nroCliente(fk), fechaAlq(fk), fechaDev(fk)", #cuotas?)
PAGA_COMPRA("nroTark(fk), #ticket(fk)", #cuotas?)