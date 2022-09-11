-- Listar información de proyectos que tengan no menos 50 aplicaciones finalizadas. 
-- Ordenar por cantidad de aplicaciones.

SELECT DISTINCT nombreProyecto, avance, descProyecto, objetivos
FROM Proyecto
NATURAL JOIN Aplicacion
WHERE porcentajeFinalizado = 100
GROUP BY codProyecto, nombreProyecto, avance, descProyecto, objetivos
HAVING COUNT(codAplicacion) >= 50
ORDER BY COUNT(codAplicacion)