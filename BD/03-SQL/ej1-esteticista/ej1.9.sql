-- Listar datos de productos utilizados por todos los esteticistas.

-- NOT EXISTS devuelve TRUE si la subconsulta especificada tuvo 0 tuplas como resultado
-- EXCEPT devuelve los datos que están en el primer conjunto pero NO en el segundo

-- Para cada producto de la consulta principal se verifica que la siguiente pregunta sea FALSA:
-- ¿Existe algún Esteticista que NO aplicó el producto X alguna vez?

-- la profe dijo que funciona bien! otra solución: not exists encadenados
SELECT nombreP, descripcion, stock, precio
FROM Producto as p
WHERE NOT EXISTS(
    (
        SELECT codEst 
        FROM Esteticista
    ) EXCEPT (
        SELECT codEst
        FROM Aplicacion as a
        INNER JOIN ProductoAplicado as pa ON pa.nroAplicacion = a.nroAplicacion
        WHERE pa.codProd = p.codProd
    )
)