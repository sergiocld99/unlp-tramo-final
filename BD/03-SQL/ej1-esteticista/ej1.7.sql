-- Reportar nombre, descripción,stock y precio de productos que se utilizaron en mujeres y que tengan 
-- alguna aplicación donde el precio al momento de la aplicación fue inferior a $500

-- Interpretación 1: listar información de productos que cuando fueron aplicados costaban
-- menos de $500 y además dichos PRODUCTOS se aplicaron ALGUNA VEZ A UNA CLIENTE MUJER

-- Opción 1: primero busco si fue aplicado a alguna mujer y luego el precio
SELECT DISTINCT nombreP, descripcion, stock, p.precio as precioActual
FROM Producto as p
INNER JOIN ProductoAplicado as pa ON (p.codProd = pa.codProd)
INNER JOIN Aplicacion as a ON (a.nroAplicacion = pa.nroAplicacion)
INNER JOIN Cliente as c ON (c.codCliente = a.codCliente)
WHERE sexo = 'F' AND p.codProd IN (
    SELECT codProd FROM ProductoAplicado as pa1
    WHERE pa1.precio < 500
)

-- Opción 2: primero busco por el precio y luego si aparte fueron aplicados a mujeres
SELECT DISTINCT nombreP, descripcion, stock, p.precio as precioActual
FROM Producto as p
INNER JOIN ProductoAplicado as pa ON (p.codProd = pa.codProd)
WHERE pa.precio < 500 AND p.codProd IN (
    SELECT pa1.codProd FROM ProductoAplicado as pa1
    INNER JOIN Aplicacion as a1 ON (a1.nroAplicacion = pa1.nroAplicacion)
    INNER JOIN Cliente as c1 ON (c1.codCliente = a1.codCliente)
    WHERE sexo = 'F'
)

-- Interpretación 2: listar información de productos que cuando fueron aplicados costaban
-- menos de $500 y a su vez DICHA APLICACIÓN fue realizada a UNA CLIENTE MUJER

SELECT DISTINCT nombreP, descripcion, stock, p.precio as precioActual
FROM Producto as p
INNER JOIN ProductoAplicado as pa ON (p.codProd = pa.codProd)
INNER JOIN Aplicacion as a ON (a.nroAplicacion = pa.nroAplicacion)
INNER JOIN Cliente as c ON (c.codCliente = a.codCliente)
WHERE pa.precio < 500 AND sexo = 'F'