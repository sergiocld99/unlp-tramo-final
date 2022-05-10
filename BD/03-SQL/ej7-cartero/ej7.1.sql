-- Cartero = (DNI,nombreYApe, dirección, teléfono) 

-- Sucursal = (IDSUC, nombreS,direcciónS, teléfonoS) 

-- Envio = (NROENVIO, DNI, IDCLIENTEEnvia, IDCLIENTERecibe, IDSUC, fecha, recibido, fechaRecibe, direcciónEntrega)
-- //recibido es blanco sino se entregó aún el envío

-- Cliente = (IDCLIENTE, DNI,nombreYApe, dirección, teléfono)

-- 1. Reportar nombre y apellido, dirección y teléfono del cartero que realizó más envíos. 

-- Opción 1
SELECT nombreYApe, dirección, teléfono
FROM Cartero
NATURAL JOIN Envio          -- coincide columna "DNI"
GROUP BY DNI, nombreYApe, dirección, teléfono
HAVING COUNT(NROENVIO) >= ALL (
    SELECT COUNT(*) FROM Envio
    GROUP BY DNI 
)

-- Opción 2
SELECT nombreYApe, dirección, teléfono
FROM Cartero
NATURAL JOIN Envio          -- coincide columna "DNI"
GROUP BY DNI, nombreYApe, dirección, teléfono
ORDER BY COUNT(NROENVIO) DESC LIMIT 1