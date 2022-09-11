-- Reportar para cada película, nombre y la cantidad de veces que fue exhibida durante 2020.

SELECT nombre, COUNT(#codF) as cantExhibiciones
FROM Pelicula
NATURAL JOIN Funcion
WHERE Year(fecha) = 2020
GROUP BY #codP, nombre