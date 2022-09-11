// primero las entidades (marca y proveedor se fusionaron por correspondencia 1-1)
MARCA("nombreMarca", calle, nro, piso?, dpto?, email, telefono)
PRODUCTO("nombreMarca(fk), codProd", nombre, descripcion, tamaño, precioAct)
SUPERFICIE_APLICABLE("superficie_aplicable")

CLIENTE("nroCliente", direccion, telefono, tipo, cuit?)
VENTA("#ticket", monto, montoDesc?, nroCliente(fk))
TARJETA("nro", codSeg, banco, tipo, nombreEntidad(fk))
ENTIDAD("nombreEntidad")

PROMOCION("codPromo", fechaIni, fechaFin?, porcentajeDesc)
PROMO_TARJETA("codPromo(fk)")
PROMO_PRODUCTOS("codPromo(fk)")

// relaciones muchos a muchos
APLICA_SUP("nombreMarca(fk), codProd(fk), superficie_aplicable(fk)")
SE_VENDE("nombreMarca(fk), codProd(fk), #ticket(fk)", precio)

// relaciones opcional a muchos
SE_PAGA("#ticket(fk)", nro(fk), #cuotas?)
APLICA_ENTIDAD("codPromo(fk), nombreEntidad(fk)", #cuotasSinInteres)
APLICA_PROD("codPromo(fk), nombreMarca(fk), codProd(fk)")