// Entidades independientes y sin claves foráneas
PARADA("nombre", latitud, longitud, tipo)

// Entidades con relaciones 1 a muchos
VIAJE("fecha, horaIni", horaFin?, tipo, linea?, ramal?, nombrePdaInicio(fk), nombrePdaFin(fk))