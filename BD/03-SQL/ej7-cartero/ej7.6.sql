-- Listar datos personales de carteros que 
-- no entregaron envío a clientes(receptor) 
-- residentes en ‘La Plata’ pero si realizaron 
-- envíos de clientes (emisor) que residen en ‘Wilde’ 
-- (el cliente que envía vive en Wilde).

SELECT DISTINCT CAR.DNI as DNI, CAR.nombreYApe as nombreYApe, CAR.dirección as dirección, CAR.teléfono as teléfono
FROM Cartero CAR
NATURAL JOIN Envio E
INNER JOIN Cliente C ON E.IDCLIENTEEnvia = C.IDCLIENTE
WHERE C.dirección LIKE "%Wilde%" AND NOT EXISTS (
    SELECT * FROM Envio E2
    INNER JOIN Cliente C2 ON E2.IDCLIENTERecibe = C2.IDCLIENTE
    WHERE E2.DNI = CAR.DNI AND dirección LIKE "%La Plata%"
)