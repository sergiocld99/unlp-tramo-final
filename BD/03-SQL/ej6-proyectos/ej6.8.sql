--  Listar información de proyectos que tenga alguna aplicación con fecha de comienzo inferior a 2010 
-- y aún no se terminó. Informar nombre de proyecto, área, fecha inicio y fecha prevista fin.

SELECT DISTINCT nombreProyecto, nombreA, fechaIni, fechaFin
FROM Proyecto as P
NATURAL JOIN Aplicacion
NATURAL JOIN Area
WHERE Year(fechaIni) < 2010 AND porcentajeFinalizado < 100