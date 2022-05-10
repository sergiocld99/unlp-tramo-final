-- Interpretación: Obtener la edad promedio de los clientes que se aplicaron algún producto
-- cuyo nombre termina en "ura"

-- En la subconsulta se obtienen los códigos de todos los productos terminados en "ura"
-- para luego filtrar aquellas aplicaciones donde se haya utilizado alguno de ellos

-- Opcion 1 usando NATURAL JOIN
SELECT AVG(edad) as promedioEdad
FROM Cliente
NATURAL JOIN Aplicacion
NATURAL JOIN ProductoAplicado
WHERE codProd IN (
    SELECT codProd FROM Producto
    WHERE (nombreP LIKE "%ura")
)

-- Opcion 2 usando INNER JOIN
SELECT AVG(edad) as promedioEdad
FROM Cliente as c
INNER JOIN Aplicacion as a ON (a.codCliente = c.codCliente)
INNER JOIN ProductoAplicado as pa ON (pa.nroAplicacion = a.nroAplicacion)
WHERE codProd IN (
    SELECT codProd FROM Producto
    WHERE (nombreP LIKE "%ura")
)