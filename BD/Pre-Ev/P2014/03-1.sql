-- Reportar TODA la información de marcas y modelos que tengan más de 100 autos a la venta

SELECT MA.#marca as #marca, MA.nombre as nombreMarca, MA.descripcion as descripcionMarca, MO.#modelo as #modelo, MO.nombre as nombreModelo, MO.descripcion as descripcionModelo
FROM Vehiculo V
INNER JOIN Modelo MO ON V.#modelo = MO.#modelo
INNER JOIN Marca MA ON V.#marca = MA.#marca
GROUP BY MA.#marca, MA.nombre, MA.descripcion, MO.#modelo, MO.nombre, MO.descripcion
HAVING COUNT(#motor) > 100