-- Reportar nombre, descripción,stock y precio de productos que se utilizaron en mujeres y que tengan 
-- alguna aplicación donde el precio al momento de la aplicación fue inferior a $500

-- Interpretación 1: listar información de productos que cuando fueron aplicados costaban
-- menos de $500 y además dichos PRODUCTOS se aplicaron ALGUNA VEZ A UNA CLIENTE MUJER

-- Interpretación 2: listar información de productos que cuando fueron aplicados costaban
-- menos de $500 y a su vez DICHA APLICACIÓN fue realizada a UNA CLIENTE MUJER

-- Esta es la correcta
SELECT DISTINCT nombreP, descripcion, stock, p.precio as precioActual
FROM Producto as p
INNER JOIN ProductoAplicado as pa ON (p.codProd = pa.codProd)
INNER JOIN Aplicacion as a ON (a.nroAplicacion = pa.nroAplicacion)
INNER JOIN Cliente as c ON (c.codCliente = a.codCliente)
WHERE pa.precio < 500 AND sexo = 'F'