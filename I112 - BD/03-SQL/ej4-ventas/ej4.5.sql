-- Agregar una venta para el empleado Castelli Juan Manuel con nroTicket 1000 con la fecha y monto
-- que desee para el cliente DNI 22369659.

-- Pregunta: ¿se puede hacer SELECT dentro de VALUES?
-- Respuesta: si

INSERT INTO Ventas (nroTicket, codigoEmp, codigoCte, fecha, montoTotal)
VALUES (    1000, 
            SELECT codigoEmp WHERE nombre = "Castelli Juan Manuel", 
            SELECT codigoCte WHERE DNI = 22369659,
            "2022-05-12", 
            500
        )