# Etapas

Cuando se requiere juntar las particiones de varios nodos para aplicar una transformación <b>wide</b> decimos que pasamos a una nueva etapa o stage.

- Ejemplos de transformaciones narrow: map y filter.
- Ejemplos de transformaciones wide: groupBy, join.

# Ej 3-b

## Transformaciones ejecutadas
Revisamos cuales son las que realmente se ejecutan debido a la acción count() al final. Resulta que se ejecutan todas:

    A = A.map(fMap1)            # T1 (Narrow)
    A = A.distinct()            # T2 (Wide)
    B = B.filter(fFilter1)      # T3 (Narrow)
    C = C.filter(fFilter1)      # T4 (Narrow)
    E = C.join(B)               # T5 (Wide)
    B = A.map(fMap2)            # T6 (Narrow)
    C = E.map(fMap3)            # T7 (Narrow)
    D = B.union(C)              # T8 (Wide)
    F = E.map(fMap4)            # T9 (Narrow)
    E = F.filter(fFilter2)      # T10 (Narrow)
    D = D.union(E).union(B)     # T11 (Wide x2)
    B = D.substract(E)          # T12 (Wide)

Etapa 1:

    A = A.map(fMap1)            # T1 (Narrow)
    B = B.filter(fFilter1)      # T3 (Narrow)
    C = C.filter(fFilter1)      # T4 (Narrow)

Etapa 2, que depende de Etapa 1:

    E = C.join(B)               # T5 (Wide)
    C = E.map(fMap3)            # T7 (Narrow)
    F = E.map(fMap4)            # T9 (Narrow)
    E = F.filter(fFilter2)      # T10 (Narrow)

Etapa 3, que depende de Etapa 1:

    A = A.distinct()            # T2 (Wide)
    B = A.map(fMap2)            # T6 (Narrow)

Etapa 4, que depende de Etapas 2 y 3:

    D = B.union(C)              # T8 (Wide)

Etapa 5, que depende de Etapa 4:

    D = D.union(E)              # T11-a (Wide)

Etapa 6, que depende de Etapa 5:

    D = D.union(B)              # T11-b (Wide)

Etapa 7, que depende de Etapa 6:

    B = D.substract(E)          # T12 (Wide)
