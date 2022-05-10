-- Informar cantidad de envíos no entregados del mes de mayo de 2019

SELECT COUNT(*)
FROM Envio
WHERE recibido is null
AND Month(fecha) is 5 and Year(fecha) is 2019