-- Reportar nombre, descripción y género de películas exhibidas en el Cine: ´Cine X´ pero que no 
-- tengan programadas funciones en dicho cine para el dia de hoy

-- Opción 2: para cada función de cada película exhibida en "Cine X"
-- se verifica que la película de dicha función NO tenga fecha para hoy en el mismo cine

SELECT DISTINCT nombre, P.descripción as descripción, genero
FROM Pelicula P
NATURAL JOIN Funcion F
INNER JOIN Sala as S ON S.#codS = F.#codS
INNER JOIN Cine as C ON C.#codC = S.#codC
WHERE nombreC = "Cine X" AND NOT EXISTS (
    SELECT * FROM Funcion F2
    WHERE P.#codP = F2.#codP AND C.#codC = F2.#codC AND fecha = "10-05-2022"
)