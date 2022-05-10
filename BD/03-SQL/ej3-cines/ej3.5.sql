-- Cine = (#codC, nombreC, direccion) 
-- Sala = (#codS, nombreS, descripción, capacidad, #codC) 
-- Película = (#codP, nombre, descripción, genero) 
-- Funcion = (#codF, #codS, #codP, fecha, hora, ocupación) //ocupación indica # de espectadores de la función

-- Listar nombre y dirección de cines donde se exhiba la película: ‘007 Bond: Sin tiempo para morir’ o 
-- que tengan funciones con ocupación durante 2020.

SELECT nombreC, dirección
FROM Cine
WHERE #codC IN (
    SELECT #codC FROM Funcion
    NATURAL JOIN Pelicula
    WHERE nombre = "007 Bond: Sin tiempo para morir"
) OR #codC IN (
    SELECT #codC FROM Funcion
    WHERE Year(fecha) = 2020 AND ocupación > 0
)