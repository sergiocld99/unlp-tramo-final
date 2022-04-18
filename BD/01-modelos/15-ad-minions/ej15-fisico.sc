// Entidades independientes
VILLANO("nombre", maldades, f_nac, calle, nro?, piso?)
TELEFONO("tel")

// Entidades con relación 1 a muchos
MINION("codigo", nombre, nombreVillano(fk), f_inicio)           // f_inicio es foreign key??

// Entidades con PK foránea
VIOLETA("codigo(fk)")

// ¿cómo tratar una relación 1 a muchos pero con atributos en el rombo?
// esto no se debería hacer, no?    -> ACTUAL("codigo(fk)", nombre(fk), f_inicio)

// Relaciones muchos a muchos
CONTACTO("nombre(fk), tel(fk)")
HISTORICO("codigo(fk), nombre(fk), f_inicio", f_fin)
MUTÓ("codigoMinion(fk), codigoVioleta(fk), fecha, tipo")        // ¿cuando muta cambia de código?