// Entidades independientes
VILLANO("nombre", maldades, f_nac, calle, nro?, piso?)
TELEFONO("tel")

// Entidades con relación 1 a muchos
MINION("codigo", nombre, nombreVillano(fk), f_inicio(fk))           // f_inicio es foreign key??

// Entidades con PK foránea
VIOLETA("codigo(fk)")

// Relaciones muchos a muchos
CONTACTO("nombre(fk), tel(fk)")
HISTORICO("codigo(fk), nombre(fk), f_inicio", f_fin)
MUTÓ("codigoMinion(fk), codigoVioleta(fk), fecha", tipo)

// No pasa nada si ambos codigos de Mutó son iguales
// Fecha debe ser PK, tipo no es necesario