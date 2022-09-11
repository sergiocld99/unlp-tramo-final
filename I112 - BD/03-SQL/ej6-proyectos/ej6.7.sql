-- Listar información de departamentos que no tengan aplicaciones registradas.

SELECT nombreD, funciones, fecha_creacion
FROM Departamento as D
WHERE NOT EXISTS (
    SELECT * FROM Aplicacion
    NATURAL JOIN Area as AR
    WHERE D.codDepto = AR.codDepto
)