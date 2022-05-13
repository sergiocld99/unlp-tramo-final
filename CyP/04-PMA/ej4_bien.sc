chan Pedidos(int);
chan Pagos(int, int);
chan Turno[N](int), Finalizado[N](char), Salida(char);
chan Signal(char);

Process Cliente[id: 0..N-1]
{
    int cabina;

    send Pedidos(id);
    send Signal();

    receive Turno[id](cabina);
    // usar la cabina indicada
    
    send Pagos(id, cabina);
    send Signal();
    
    receive Finalizado[id]();
    send Salida();
}

Process Empleado
{
    int idC, cabina, atendidos = 0;
    Cola libres;

    while (atendidos < N)
    {
        receive Signal();

        if (empty(libres) || not empty(Pagos))
        {
            receive Pagos(idC, cabina);
            Cobrar();
            push(libres, cabina);
            atendidos++;
            send Finalizado[idC]();
            receive Salida();
        } else if (not empty(Pedidos)) {
            receive Pedidos(idC);
            pop(libres, cabina);
            send Turno[idC](cabina);
        }
    }
}