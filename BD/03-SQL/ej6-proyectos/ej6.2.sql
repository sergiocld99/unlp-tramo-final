-- Departamento = (codDepto, nombreD, funciones, fecha_creacion) 
-- // funciones es una descripción de las funciones más relevantes

-- Area = (codArea, codDepto, nombreA, descripción) 
-- // codDepto es el departamento al cual corresponde esa área

-- Proyecto = (codProyecto, nombreProyecto, avance, descProyecto, objetivos) 
-- // descripción del proyecto

-- Aplicacion = (codAplicacion, codProyecto, codArea, fechaIni, porcentajeFinalizado, fechaFin, fechaPrevistaFin)
-- // descripción de aplicaciones del proyecto en determinadas áreas indicando evolución del mismo

-- Reportar para cada área la cantidad de proyectos finalizados durante 2020. 
-- Informar nombre de área y cantidad de proyectos finalizados.

SELECT DISTINCT nombreA, COUNT(codProyecto) as cantProyectosFinalizados
FROM Area
NATURAL JOIN Aplicacion
WHERE Year(fechaFin) = 2020
GROUP BY codArea, nombreA