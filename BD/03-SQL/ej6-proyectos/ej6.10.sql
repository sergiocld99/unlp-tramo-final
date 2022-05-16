--  Listar información de proyectos que sólo tengan aplicaciones finalizadas durante 2019. 

-- Corrección 16/5: una sola subconsulta y faltaba agregar DISTINCT
SELECT DISTINCT nombreProyecto, avance, descProyecto, objetivos
FROM Proyecto P
NATURAL JOIN Aplicacion
WHERE Year(fechaFin) = 2019 AND NOT EXISTS (
    SELECT * FROM Aplicacion A2
    WHERE A2.codProyecto = P.codProyecto
    AND Year(A2.fechaFin) <> 2019
)