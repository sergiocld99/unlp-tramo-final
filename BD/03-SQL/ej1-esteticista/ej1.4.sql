-- Interpretación: informar datos personales de esteticistas que NO realizaron aplicaciones 
-- a algún cliente menor de 25 años

-- En la subconsulta se obtienen los números de todas las aplicaciones realizadas a clientes
-- menores de 25 años, para luego quedarme con los esteticistas no involucrados a alguna de ellas

SELECT DISTINCT nombre, apellido, DNI, fecha_nac, especialidad
FROM Esteticista
NATURAL JOIN Aplicacion
WHERE nroAplicacion NOT IN (
    SELECT nroAplicacion FROM Aplicacion
    NATURAL JOIN Cliente WHERE edad < 25
)
