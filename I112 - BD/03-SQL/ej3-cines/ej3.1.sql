-- Cine = (#codC, nombreC, direccion) 
-- Sala = (#codS, nombreS, descripción, capacidad, #codC) 
-- Película = (#codP, nombre, descripción, genero) 
-- Funcion = (#codF, #codS, #codP, fecha, hora, ocupación) //ocupación indica # de espectadores de la función

-- 1. Reportar información de películas exhibidas en cines de ‘Avellaneda’ y que posean funciones en 
-- cines de ‘La Plata’.

-- solución propuesta por la profe
SELECT nombre, descripción, genero
FROM Pelicula P
INNER JOIN Funcion F ON F.#codP = P.#codP
INNER JOIN Sala S ON F.#codS = S.#codS
INNER JOIN Cine C ON C.#codC = S.#codC
WHERE direccion LIKE "%Avellaneda%"
AND Exist (
    SELECT * FROM Funcion as F2
    INNER JOIN Sala as S2 ON S2.#codS = F2.#codS
    INNER JOIN Cine as C2 ON C2.#codC = S2.#codC
    WHERE F2.#codP = P.#codP AND direccion LIKE "%La Plata%"
)