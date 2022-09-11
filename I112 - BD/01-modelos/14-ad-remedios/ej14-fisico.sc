// Entidades independientes
CLINICA("razon_social", calle, nro, piso?, dpto?)
PRODUCTO("codigo", nombre, presentacion, stock, precio)
ANIMAL("animal")

// Relación 1 a muchos
ATENCION("nro_atencion", fecha_hora, requiere_domicilio, notas, razon_social(fk))

// Entidades con ID dependiente
REMEDIO("codigo(fk)", accionTerapeutica)

// Relaciones muchos a muchos
VENDE("razon_social(fk), codigo(fk)")
PARA("codigo(fk), animal(fk)")