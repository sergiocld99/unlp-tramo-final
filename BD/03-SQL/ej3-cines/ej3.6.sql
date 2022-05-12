-- Reportar nombre, descripción y género de películas exhibidas en el Cine: ´Cine X´ pero que no 
-- tengan programadas funciones en dicho cine para el dia de hoy

-- Opción 1: la más literal. NO es una buena solución porque se hacen 2 subconsultas
SELECT nombre, descripción, genero
FROM Pelicula as P
WHERE EXIST (
    SELECT * FROM Funcion as F1
    INNER JOIN Sala as S1 ON S1.#codS = F1.#codS
    INNER JOIN Cine as C1 ON C1.#codC = S1.#codC
    WHERE P.#codP = F1.#codP AND nombreC = "Cine X"
) AND NOT EXISTS (
    SELECT * FROM Funcion as F2
    INNER JOIN Sala as S2 ON S2.#codS = F2.#codS
    INNER JOIN Cine as C2 ON C2.#codC = S2.#codC
    WHERE P.#codP = F2.#codP AND nombreC = "Cine X" AND fecha = "10-05-2022"
)

-- Opción 2: para cada función de cada película exhibida en "Cine X"
-- se verifica que la película de dicha función NO tenga fecha para hoy en el mismo cine

SELECT DISTINCT nombre, P.descripción as descripción, genero
FROM Pelicula as P
INNER JOIN Funcion as F ON F.#codP = P.#codP
INNER JOIN Sala as S ON S.#codS = F.#codS
INNER JOIN Cine as C ON C.#codC = S.#codC
WHERE nombreC = "Cine X" AND NOT EXISTS (
    SELECT * FROM Funcion F2
    WHERE P.#codP = F2.#codP AND C.#codC = F2.#codC AND fecha = "10-05-2022"
)