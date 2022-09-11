-- Listar datos personales de clientes que no 
-- realizaron envíos a la sucursal con nombre ‘La Amistad 1’.

SELECT DNI, nombreYApe, dirección, teléfono
FROM Cliente
WHERE IDCLIENTE NOT IN (
    SELECT IDCLIENTEEnvia FROM Envio
    NATURAL JOIN Sucursal
    WHERE nombreS = "La Amistad 1"
)