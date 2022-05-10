-- Reportar para cada cine la cantidad de espectadores por película durante 2020. Indicar nombre del 
-- cine, nombre de la película y cantidad de espectadores. Ordenar por cine y luego por película.

SELECT nombreC as nombreCine, nombre as nombrePelicula, SUM(ocupación) as cantEspectadores
FROM Funcion as F
INNER JOIN Cine as C ON C.#codC = F.#codC
INNER JOIN Pelicula as P ON P.#codP = F.#codP
WHERE Year(fecha) = 2020
GROUP BY C.#codC, C.nombreC, P.#codP, P.nombre
ORDER BY 1,2