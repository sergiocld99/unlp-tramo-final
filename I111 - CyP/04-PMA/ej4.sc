chan Pedidos(int);
chan Pagos(int, int);
chan Turno[N](int), Finalizado[N](char);

// ESTA SOLUCIÓN FUNCIONA PERO TIENE BUSY WAITING, VER EJ4_BIEN

Process Cliente[id: 0..N-1]
{
    int cabina;

    send Pedidos(id);
    receive Turno[id](cabina);
    // usar la cabina indicada
    send Pagos(id, cabina);
    receive Finalizado[id]();
}

Process Empleado
{
    int idC, cabina, atendidos = 0;
    Cola libres;

    while (atendidos < N)
    {
        // cuando hay cabinas libres hay busy-waiting!!

        if (empty(libres) || not empty(Pagos))
        {
            receive Pagos(idC, cabina);
            Cobrar();
            push(libres, cabina);
            atendidos++;
            send Finalizado[idC]();
        } else if (not empty(Pedidos)) {
            receive Pedidos(idC);
            pop(libres, cabina);
            send Turno[idC](cabina);
        }
    }
}