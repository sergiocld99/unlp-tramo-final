-- Reportar nombres de modelos que estén en venta en concesionarios de 'La Plata'
-- y no de 'Trelew'

SELECT DISTINCT M.nombre as nombreModelo
FROM Modelo M
INNER JOIN Vehiculo V ON M.#modelo = V.#modelo
INNER JOIN Concesionario C ON V.#concesionario = C.#concesionario
WHERE ciudad = 'La Plata' AND M.#modelo NOT IN (
    SELECT #modelo
    FROM Vehiculo V2
    INNER JOIN Concesionario C2 ON V2.#concesionario = C2.#concesionario
    WHERE C2.ciudad = 'Trelew'
)