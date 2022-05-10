-- Listar para cada producto, la cantidad de aplicaciones en las que fue utilizado. Indicar nombre, 
-- descripción, stock, precio y cantidad de aplicaciones. Ordenar por cantidad de aplicaciones

-- Usamos left join porque también listamos productos con 0 aplicaciones
SELECT DISTINCT nombreP, descripcion, stock, p.precio as precioActual, COUNT(pa.nroAplicacion) as cantidad
FROM Producto as p
LEFT JOIN ProductoAplicado as pa ON (p.codProd = pa.codProd)
GROUP BY p.codProd, nombreP, descripcion, stock, p.precio
ORDER BY COUNT(pa.nroAplicacion)