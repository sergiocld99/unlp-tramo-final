-- Listar los datos de los productos que no fueron vendidos durante 2020.

-- Corrección 16/5: FROM productos, Ventas solo en subconsulta
SELECT nombre, presentacion, stock, stockMinimo, precioActual
FROM Productos
NATURAL JOIN DetalleVentas
WHERE codVenta NOT IN (
    SELECT codVenta FROM Ventas
    WHERE Year(fecha) = 2020
)