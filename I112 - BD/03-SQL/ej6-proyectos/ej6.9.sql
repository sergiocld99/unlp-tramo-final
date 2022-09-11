-- Listar información de de todas las áreas que componen el departamento de ‘Redes’.

SELECT DISTINCT nombreA, descripcion
FROM Area
NATURAL JOIN Departamento
WHERE nombreD = "Redes"