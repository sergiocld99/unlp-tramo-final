-- Reportar información de sucursales que realizaron
-- envíos durante 2020 
-- o que tengan dirección en Tucumán

-- Puede haber sucursales en Tucumán sin envios
SELECT DISTINCT IDSUC, nombreS, direcciónS, teléfonoS
FROM Sucursal S
LEFT JOIN Envio E ON S.IDSUC = E.IDSUC
WHERE Year(fecha) = 2020
OR direcciónS LIKE "%Tucumán%"