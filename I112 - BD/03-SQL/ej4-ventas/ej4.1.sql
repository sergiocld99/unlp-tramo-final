-- Productos = (idProducto, nombre, presentación, stock, stock mínimo, precioActual)
-- Empleados = (codigoEmp, DNI, nombre,fn, dirección) 
-- Clientes = (codigoCte, DNI, nombre, dirección, telefono)
-- Ventas = (codVenta, nroTicket, codigoEmp, codigoCte, fecha, montoTotal)
-- DetalleVentas = (codVenta, idProducto, cantidad, precioUnitario)

-- 1. Reportar nombre, dirección y teléfono de clientes que compraron a todos los empleados que viven 
-- en su localidad. (Asumir dirección=localidad).

-- En la subconsulta se tiene un conjunto de todos los empleados que viven en la localidad del cliente
-- y se le resta el conjunto de empleados que le vendieron a dicho cliente

SELECT nombre, direccion, telefono
FROM Clientes as c
WHERE NOT EXISTS(
    (
        SELECT codigoEmp 
        FROM Empleados as e
        WHERE e.direccion = c.direccion
    ) EXCEPT (
        SELECT codigoEmp
        FROM Ventas as v
        WHERE v.codigoCte = c.codigoCte
    )
)