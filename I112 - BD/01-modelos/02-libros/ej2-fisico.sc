// Entidades
LIBRO("isbn", titulo, genero, editorial, prologo, añoED, stockP, precioP, stockVen, precioVen)
AUTOR("#pasaporte, nacionalidad", nyap)
AGRADECIMIENTO("agradecimiento")

COPIA("isbn(fk), nroRel")
COPIA_PREST("isbn(fk), nroRel(fk)", estado)
COPIA_VEN("isbn(fk), nroRel(fk)")

CLIENTE("dni", nroUnico, nombre, telefono, calle, nro, piso?, dpto?)
PRESTAMO("dni(fk), fechIni, horaIni", fechaDevProb, fechaDev?)      // "adquiere" ya incluido
VENTA("#ticket", fecha, hora, dni(fk))                              // "realiza" fusionado

// Relaciones muchos a muchos
AGRADECE("isbn(fk)", "agradecimiento(fk)")
ESCRIBE("isbn(fk), #pasaporte(fk), nacionalidad(fk)")

// Relaciones opcional a muchos
PRESTA("isbn(fk), nroRel(fk), dni(fk), fechaIni(fk), horaIni(fk)", precio)
VENDE("isbn(fk), nroRel(fk), #ticket(fk)", precio)

// DUDA: precio debería estar en préstamo o en el rombo?