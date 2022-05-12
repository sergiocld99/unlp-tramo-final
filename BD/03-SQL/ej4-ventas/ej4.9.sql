-- Listar datos personales de empleados que participaron de ventas con algún producto con
-- precioActual superior a 1000.

SELECT DISTINCT DNI, E.nombre, fn, direccion
FROM Empleados E
NATURAL JOIN Ventas
NATURAL JOIN DetalleVentas DV
INNER JOIN Productos P ON P.idProducto = DV.idProducto
WHERE precioActual > 1000