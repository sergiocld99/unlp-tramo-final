-- Listar para cada empleado, la cantidad de ventas realizadas durante 2019. Reportar DNI, nombre,
-- fn, dirección y cantidad de ventas. El listado debe estar ordenado por nombre y fn.

SELECT DISTINCT DNI, nombre, fn, direccion, COUNT(codVenta) as cantVentas
FROM Empleado
NATURAL JOIN Ventas
WHERE Year(fecha) = 2019
GROUP BY codigoEmp, DNI, nombre, fn, direccion
ORDER BY nombre, fn