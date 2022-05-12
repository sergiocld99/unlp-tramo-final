-- Listar datos personales de los empleados que tengan ventas con más de 50 artículos diferentes.

SELECT DISTINCT DNI, nombre, fn, direccion
FROM Empleado
NATURAL JOIN Ventas
NATURAL JOIN DetalleVentas
WHERE cantidad > 50