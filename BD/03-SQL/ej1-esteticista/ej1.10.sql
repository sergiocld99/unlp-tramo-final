--  Listar el/los clientes que gastaron más el peluqueria (suma de costo total de sus aplicaciones).

-- En la subconsulta se obtiene lo que gastó en total cada cliente para luego compararlo
-- filtrando el/los máximos gastos como los que superan/igualan a todos los de la subconsulta

-- solución
SELECT DISTINCT codCliente, nombreYAp, DNI, telefono, direccion, sexo, edad
FROM Cliente
NATURAL JOIN Aplicacion
GROUP BY codCliente, nombreYAp, DNI, telefono, direccion, sexo, edad
HAVING SUM(costoTotal) >= ALL (
    SELECT SUM(costoTotal) FROM Aplicacion
    GROUP BY codCliente
)

-- Nota: las funciones de agregación (SUM, AVG, MAX, MIN) devuelven UN VALOR POR CADA GRUPO
-- (ver: https://www.w3resource.com/sql/aggregate-functions/sum-with-group-by.php)