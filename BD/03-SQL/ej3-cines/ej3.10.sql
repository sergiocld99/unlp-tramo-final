-- Listar las películas que no se hayan exhibido en 2020 en ningún cine de La Plata.

-- Para cada película de la consulta principal, se verifica si esta pregunta es FALSA:
-- ¿Existe alguna función para dicha película que sea de 2020 en un cine de La Plata?

SELECT nombre, descripción, genero
FROM Pelicula as P
WHERE NOT EXISTS (
    SELECT * FROM Funcion as F
    INNER JOIN Sala as S ON S.#codS = F.#codS
    INNER JOIN Cine as C ON C.#codC = S.#codC
    WHERE P.#codP = F.#codP AND Year(fecha) = 2020 AND direccion LIKE "%La Plata%"
)