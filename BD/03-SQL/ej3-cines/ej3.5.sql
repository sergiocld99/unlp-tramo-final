-- Cine = (#codC, nombreC, direccion) 
-- Sala = (#codS, nombreS, descripción, capacidad, #codC) 
-- Película = (#codP, nombre, descripción, genero) 
-- Funcion = (#codF, #codS, #codP, fecha, hora, ocupación) //ocupación indica # de espectadores de la función

-- Listar nombre y dirección de cines donde se exhiba la película: ‘007 Bond: Sin tiempo para morir’ o 
-- que tengan funciones con ocupación durante 2020.

-- Corrección 16/5: faltaba hacer join con Sala para Función
SELECT DISTINCT nombreC, dirección
FROM Cine
NATURAL JOIN Sala
NATURAL JOIN Funcion F
INNER JOIN Pelicula P ON F.#codP = P.#codP
WHERE nombre = "007 Bond: Sin tiempo para morir" 
OR (Year(fecha) = 2020 AND ocupación > 0)