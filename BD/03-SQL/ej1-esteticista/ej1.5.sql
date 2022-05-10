-- Interpretación: aumentar un 20% el precio de todos los productos cuyo nombre sea 'tintura'

UPDATE Producto
SET precio = precio * 1.2
WHERE nombreP = 'tintura'