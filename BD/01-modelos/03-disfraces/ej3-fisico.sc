// Entidades
CLIENTE("nroUnico", telefono, calle, nro, piso?, dpto?)
PERSONA("nroUnico(fk)", dni, nyap, email?)
INSTITUCION("nroUnico(fk)", nombre, descripcion)

DISFRAZ("nomPersonaje", descripcion, costoActual, tela)
EJEMPLAR("nomPersonaje(fk), codigoRel", talle, estado)
COLOR("color")

ALQUILER("nroUnico(fk), fechaIni", fechaDevProb, fechaDev?, costoTotal)     // "utiliza" incl.

// Relaciones muchos a muchos
DE_COLOR("nomPersonaje(fk), color(fk)")
PRESTA("nroUnico(fk), fechaIni(fk), nomPersonaje(fk), codigoRel(fk)", costo)