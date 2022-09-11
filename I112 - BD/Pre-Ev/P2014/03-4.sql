-- Reportar información de concesionarios con vehiculos de marca 'Ford' cuyo modelo
-- comience con 'Fo'

SELECT DISTINCT C.#concesionario as #concesionario, calle, nro, ciudad, telefono, razonSocial
FROM Concesionario C
INNER JOIN Vehiculo V ON C.#concesionario = V.#concesionario
INNER JOIN Marca MA ON V.#marca = MA.#marca
INNER JOIN Modelo MO ON V.#modelo = MO.#modelo
WHERE MA.nombre = 'Ford' AND (MO.nombre LIKE "Fo%")