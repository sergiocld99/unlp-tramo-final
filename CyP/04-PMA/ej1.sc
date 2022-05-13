chan Llegada(int), Finaliza[2](char), Salida[2](char);
chan Turno[N](int);

Process Persona[id: 0..N-1]
{
    int emp;

    send Llegada(id);
    receive Turno[id](emp);
    // la persona es atendida por el empleado emp
    receive Finaliza[emp]();
    send Salida[emp]();
}

Process Empleado[id: 0..1]
{
    int idP;

    while (true)
    {
        receive Llegada(idP);
        send Turno[idP](id);
        // atiendo a la persona idP
        send Finaliza[id]();
        receive Salida[id]();
    }
}