-- Productos = (idProducto, nombre, presentación, stock, stock mínimo, precioActual)
-- Empleados = (codigoEmp, DNI, nombre,fn, dirección) 
-- Clientes = (codigoCte, DNI, nombre, dirección, telefono)
-- Ventas = (codVenta, nroTicket, codigoEmp, codigoCte, fecha, montoTotal)
-- DetalleVentas = (codVenta, idProducto, cantidad, precioUnitario)

-- Listar DNI, nombre, fn y dirección de empleados que realizaron ventas a todos los clientes.

-- Por cada empleado existente, en la subconsulta se tiene un conjunto de todos los clientes
-- y se le resta el conjunto de clientes que le compraron al empleado en cuestión

SELECT DNI, nombre, fn, direccion
FROM Empleados as e
WHERE NOT EXISTS(
    (
        SELECT codigoCte
        FROM Clientes
    ) EXCEPT (
        SELECT codigoCte
        FROM Ventas as v
        WHERE v.codigoEmp = e.codigoEmp
    )
)