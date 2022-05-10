--  Listar información de proyectos que sólo tengan aplicaciones finalizadas durante 2019. 

SELECT nombreProyecto, avance, descProyecto, objetivos
FROM Proyecto as P
WHERE Exist (
    SELECT * FROM Aplicacion A1
    WHERE A1.codProyecto = P.codProyecto
    AND Year(A1.fechaFin) = 2019
) AND NOT EXISTS (
    SELECT * FROM Aplicacion A2
    WHERE A2.codProyecto = P.codProyecto
    AND Year(A2.fechaFin) <> 2019
)