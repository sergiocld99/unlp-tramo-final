-- Interpretación: informar datos personales de esteticistas que NO realizaron aplicaciones 
-- a algún cliente menor de 25 años

-- Corrección 16/5: deben devolverse también esteticistas con 0 aplicaciones asociadas
SELECT nombre, apellido, DNI, fecha_nac, especialidad
FROM Esteticista
WHERE codEst NOT IN (
    SELECT codEst FROM Aplicacion
    NATURAL JOIN Cliente WHERE edad < 25
)
