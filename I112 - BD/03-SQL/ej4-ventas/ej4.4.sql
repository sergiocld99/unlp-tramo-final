-- Informar datos personales del mejor cliente. Aquel cuyo monto de ventas realizadas supera al resto
-- de los clientes.

SELECT DNI, nombre, dirección, telefono
FROM Cliente
NATURAL JOIN Ventas
GROUP BY codigoCte, DNI, nombre, direccion, telefono
HAVING SUM(montoTotal) >= ALL (
    SELECT SUM(montoTotal) FROM Ventas
    GROUP BY codigoCte
)