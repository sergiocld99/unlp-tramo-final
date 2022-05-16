-- Reportar información de ventas (nroTicket, empleado, cliente, fecha, montoTotal) que tengan monto
-- total superior a 10000 y el cliente no sea de ´Tandil´.

-- Opción 1
SELECT DISTINCT nroTicket, E.nombre as nombreEmpleado, C.nombre as nombreCliente, fecha, montoTotal
FROM Ventas V
INNER JOIN Empleados E ON E.codigoEmp = V.codigoEmp
INNER JOIN Clientes C ON C.codigoCte = V.codigoCte
WHERE montoTotal > 10000 AND NOT EXISTS (
    SELECT * FROM Clientes C2
    WHERE C2.codigoCte = V.codigoCte AND C2.direccion LIKE "%Tandil%"
)

-- Opción 2
SELECT DISTINCT nroTicket, E.nombre as nombreEmpleado, C.nombre as nombreCliente, fecha, montoTotal
FROM Ventas V
INNER JOIN Empleados E ON E.codigoEmp = V.codigoEmp
INNER JOIN Clientes C ON C.codigoCte = V.codigoCte
WHERE montoTotal > 10000 AND NOT C.direccion LIKE "%Tandil%"