-- Listar información de proyectos que tengan aplicación en todas las áreas

SELECT nombreProyecto, avance, descProyecto, objetivos
FROM Proyecto as P
WHERE NOT EXISTS (
    (
        SELECT codArea 
        FROM Area
    ) EXCEPT (
        SELECT codArea FROM Area
        NATURAL JOIN Aplicacion as A2
        WHERE P.codProyecto = A2.codProyecto
    )
)