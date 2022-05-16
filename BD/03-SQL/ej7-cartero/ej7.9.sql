-- Listar datos personales de carteros que realizaron envíos 
-- durante 2019 a clientes con DNI inferior a 27329882

SELECT DISTINCT CAR.DNI as DNI, CAR.nombreYApe as nombreYApe, CAR.dirección as dirección, CAR.teléfono as teléfono
FROM Cartero CAR
NATURAL JOIN Envio E
INNER JOIN Cliente C ON E.IDCLIENTERecibe = C.IDCLIENTE
WHERE Year(fecha) = 2019 AND C.DNI < 27329882