-- Departamento = (codDepto, nombreD, funciones, fecha_creacion) 
-- // funciones es una descripción de las funciones más relevantes

-- Area = (codArea, codDepto, nombreA, descripción) 
-- // codDepto es el departamento al cual corresponde esa área

-- Proyecto = (codProyecto, nombreProyecto, avance, descProyecto, objetivos) 
-- // descripción del proyecto

-- Aplicacion = (codAplicacion, codProyecto, codArea, fechaIni, porcentajeFinalizado, fechaFin, fechaPrevistaFin)
-- // descripción de aplicaciones del proyecto en determinadas áreas indicando evolución del mismo

-- Listar información de departamentos que no tengan proyectos terminados.

-- Interpretación 1: avance en Proyecto es un porcentaje de finalización
SELECT nombreD, funciones, fecha_creacion
FROM Departamento
WHERE codDepto NOT IN (
    SELECT codDepto FROM Area
    NATURAL JOIN Aplicacion
    NATURAL JOIN Proyecto
    WHERE avance = 100
)

-- Interpretación 2: no tienen aplicaciones terminadas
SELECT nombreD, funciones, fecha_creacion
FROM Departamento
WHERE codDepto NOT IN (
    SELECT codDepto FROM Area
    NATURAL JOIN Aplicacion
    WHERE porcentajeFinalizado = 100
)