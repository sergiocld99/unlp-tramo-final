-- Interpretación: informar datos personales de CLIENTES que se realizaron alguna aplicación
-- en el año 2018 pero NO en 2019

-- En la subconsulta se obtienen todos los códigos de clientes que se realizaron alguna aplicación
-- en 2019 para luego restarlos a los que se realizaron alguna aplicación en 2018

-- Obvio no tiene sentido comparar los números de aplicaciones ya que NUNCA coincidirán

SELECT DISTINCT nombreYAp, DNI, telefono, direccion, sexo, edad
FROM Cliente
NATURAL JOIN Aplicacion
WHERE Year(fecha) = 2018 AND codCliente NOT IN (
    SELECT codCliente FROM Aplicacion as a1
    WHERE Year(a1.fecha) = 2019
)