-- Listar los datos de 
-- los carteros que aún no hayan realizado ninguna entrega.

SELECT DNI, nombreYApe, dirección, teléfono
FROM Cartero
WHERE DNI NOT IN (
    SELECT DNI FROM Envio
    WHERE recibido is NOT NULL
)