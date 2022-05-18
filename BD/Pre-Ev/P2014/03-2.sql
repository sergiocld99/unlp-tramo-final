-- Obtener concesionarios que vendan vehiculos de todas las marcas, ordenar el resultado por razón social

SELECT *
FROM Concesionario C
WHERE NOT EXISTS (
    (
        SELECT #marca
        FROM Marca
    ) EXCEPT (
        SELECT #marca
        FROM Vehiculo V
        WHERE V.#concesionario = C.#concesionario
    )
) ORDER BY razonSocial