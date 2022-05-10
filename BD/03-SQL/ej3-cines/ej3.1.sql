-- Cine = (#codC, nombreC, direccion) 
-- Sala = (#codS, nombreS, descripción, capacidad, #codC) 
-- Película = (#codP, nombre, descripción, genero) 
-- Funcion = (#codF, #codS, #codP, fecha, hora, ocupación) //ocupación indica # de espectadores de la función

-- 1. Reportar información de películas exhibidas en cines de ‘Avellaneda’ y que posean funciones en 
-- cines de ‘La Plata’.

-- Interpretación 1: devolver datos de peliculas que tienen funciones en cines de Avellaneda y La Plata

SELECT nombre, descripción, genero
FROM Pelicula
WHERE #codP IN (
    SELECT #codP FROM Funcion as F
    INNER JOIN Sala as S ON S.#codS = F.#codS
    WHERE #codC IN (
        SELECT #codC FROM Cine as C
        WHERE direccion LIKE "%Avellaneda%" OR direccion LIKE "%La Plata%"
    )
)

-- Interpretación 2: ya tuvieron funciones anteriores en cines de Avellaneda y actuales en La Plata

SELECT nombre, descripción, genero
FROM Pelicula
WHERE #codP IN (
    SELECT #codP FROM Funcion as F
    INNER JOIN Sala as S ON S.#codS = F.#codS
    INNER JOIN Cine as C ON C.#codC = S.#codC
    WHERE direccion LIKE "%Avellaneda%" AND fecha < "10-05-2022"
) AND #codP IN (
    SELECT #codP FROM Funcion as F
    INNER JOIN Sala as S ON S.#codS = F.#codS
    INNER JOIN Cine as C ON C.#codC = S.#codC
    WHERE direccion LIKE "%La Plata%" AND fecha >= "10-05-2022"
)