-- Listar información de productos utilizados en las aplicaciones realizadas al cliente con 
-- DNI: 38329663.

-- Opción 1 usando NATURAL JOIN
SELECT DISTINCT nombreP, descripcion, stock, precio
FROM Producto
NATURAL JOIN Aplicacion
NATURAL JOIN Cliente
WHERE DNI = 38329663

-- Opción 2 usando INNER JOIN
SELECT DISTINCT nombreP, descripcion, stock, precio
FROM Producto as p
INNER JOIN Aplicacion as a ON a.codProd = p.codProd 
INNER JOIN Cliente as c ON c.codCliente = a.codCliente
WHERE DNI = 38329663