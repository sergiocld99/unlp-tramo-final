-- Interpretación: Obtener la edad promedio de los clientes que se aplicaron algún producto
-- cuyo nombre termina en "ura"

-- Corrección 16/5: un mismo cliente no puede aparecer repetido en la consulta principal
SELECT AVG(edad) as promedioEdad
FROM Cliente
WHERE codCliente IN (
    SELECT codCliente FROM Aplicacion
    NATURAL JOIN ProductoAplicado PA
    INNER JOIN Producto P ON PA.codProd = P.codProd
    WHERE nombreP LIKE "%ura"
)