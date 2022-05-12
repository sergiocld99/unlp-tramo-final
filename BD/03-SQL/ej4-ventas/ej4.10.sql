-- Listar los datos de los productos que no fueron vendidos durante 2020.

SELECT nombre, presentacion, stock, stockMinimo, precioActual
NATURAL JOIN DetalleVentas
NATURAL JOIN Ventas
WHERE codVenta NOT IN (
    SELECT codVenta FROM Ventas
    WHERE Year(fecha) = 2020
)