--  Listar datos personales de carteros que entregaron envíos a todas las sucursales.

SELECT nombreYApe, dirección, teléfono
FROM Cartero as CAR
WHERE NOT EXISTS (
    (
        SELECT IDSUC
        FROM Sucursal
    ) EXCEPT (
        SELECT IDSUC
        FROM Envio as E
        WHERE E.DNI = CAR.DNI
    )
)