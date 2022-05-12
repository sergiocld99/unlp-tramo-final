-- Listar datos personales de clientes que realizaron compras en 2019 pero no realizaron compras
-- durante 2020.

SELECT DISTINCT DNI, nombre, direccion, telefono
FROM Clientes C
NATURAL JOIN Ventas
WHERE Year(fecha) = 2019 AND NOT EXISTS (
    SELECT * FROM Ventas V2
    WHERE V2.codigoCte = C.codigoCte AND Year(V2.fecha) = 2020
)