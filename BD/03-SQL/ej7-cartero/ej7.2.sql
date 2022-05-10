--  Listar para cada sucursal, la cantidad de envíos realizados a alguna dirección de envío que 
-- contenga el string ‘Ju’. Informar nombre de sucursal y cantidad de envíos correspondiente. Ordenar 
-- por nombre sucursal

SELECT nombreS, COUNT(NROENVIO) as cantEnvios
FROM Sucursal
NATURAL JOIN Envio
WHERE direcciónEntrega LIKE "%Ju%"
GROUP BY IDSUC, nombreS
ORDER BY 1