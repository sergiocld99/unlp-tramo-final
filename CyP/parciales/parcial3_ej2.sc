int cant[5] = ([5] 0);      // cada juntada
int siguiente;

cola C;
bool libre = true;          // vestuario

sem mutex = 1;
sem mutexGrupo[5] = ([5] 1);
sem barrera[5] = ([5] 0);

Process Empleado[id: 0..39]
{
    int grupo = ...;

    P(mutexGrupo[grupo]);
    if (++cant[grupo] == 8)
    {
        cant[grupo] = 0;
        for i = 1..8 -> V(barrera[grupo]);
    }
    V(mutexGrupo[grupo]);

    P(barrera[grupo]);
    // Dirigirse hacia el vestuario

    P(mutexGrupo[grupo]);
    if (++cant[grupo] == 8)
    {
        P(mutex);
        if (libre)
        {
            libre = false;
            V(mutex);
        } else {
            push(C, grupo);
            V(mutex);

            P(turno[grupo]);
        }
        
        cant[grupo] = 0;
        for i = 1..8 -> V(barrera[grupo]);
    } 
    V(mutexGrupo[grupo]);

    P(barrera[grupo]);
    // Entrar al vestuario
    // Cambiarse la ropa
    // Retirarse del vestuario

    P(mutexGrupo[grupo]);
    if (++cant[grupo] == 8)
    {
        P(mutex);
        if (empty(C)) libre = true;
        else {
            pop(C, siguiente);
            V(turno[siguiente]);
        }
        V(mutex);
    }
    V(mutexGrupo[grupo]);
}