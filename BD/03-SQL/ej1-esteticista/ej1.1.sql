-- Interpretación: informar datos personales de esteticistas que realizaron alguna aplicación
-- solamente en el año 2019. Ordenar por nombre, apellido y DNI (en ese orden)

-- Se comparan codEst entre consulta principal y subconsulta ya que obviamente
-- el numero de aplicación NUNCA va a coincidir en diferentes años

SELECT DISTINCT nombre, apellido, DNI, fecha_nac, especialidad
FROM Esteticista
NATURAL JOIN Aplicacion
WHERE Year(fecha) = 2019 AND codEst NOT IN (
    SELECT codEst FROM Aplicacion as a1
    WHERE Year(a1.fecha) <> 2019
) ORDER BY nombre, apellido, DNI

-- Nota: para NATURAL JOIN no hace falta especificar ON 
-- (ver https://www.w3resource.com/sql/joins/natural-join.php)