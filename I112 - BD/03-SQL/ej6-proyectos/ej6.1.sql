-- Departamento = (codDepto, nombreD, funciones, fecha_creacion) 
-- // funciones es una descripción de las funciones más relevantes

-- Area = (codArea, codDepto, nombreA, descripción) 
-- // codDepto es el departamento al cual corresponde esa área

-- Proyecto = (codProyecto, nombreProyecto, avance, descProyecto, objetivos) 
-- // descripción del proyecto

-- Aplicacion = (codAplicacion, codProyecto,codArea, fechaIni, porcentajeFinalizado, fechaFin, fechaPrevistaFin)
-- // descripción de aplicaciones del proyecto en determinadas áreas indicando evolución del mismo

-- 1.Listar aplicaciones de proyectos que se terminaron luego de la fecha prevista de fin. Indicar nombre
-- del proyecto, área de aplicación, departamento correspondiente, fecha de inicio y fin, y fecha 
-- prevista de fin. Ordenar por nombre de proyecto y área.

SELECT DISTINCT nombreProyecto, nombreA, nombreD, fechaIni, fechaFin, fechaPrevistaFin
FROM Aplicacion as a
INNER JOIN Proyecto as p ON p.codProyecto = a.codProyecto
INNER JOIN Area as AR ON AR.codArea = a.codArea
INNER JOIN Departamento as D ON D.codDepto = AR.codDepto
WHERE fechaFin > fechaPrevistaFin
ORDER BY nombreProyecto, nombreA