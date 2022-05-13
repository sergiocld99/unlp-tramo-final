chan Llegada(int), Desocupa(int);
chan RespuestaFila[P](int);
chan LlegadaFila[5](int), Finaliza[5](char), Salida[5](char);
chan Turno[P](char);

Process Persona[id: 0..P-1]
{
    int caja;

    send Llegada(id);
    receive RespuestaFila[id](caja);
    send LlegadaFila[caja](id);
    receive Turno[id]();
    // soy atendido en la caja
    receive Finaliza[caja]();
    send Salida[caja]();
}

Process Coordinador
{
    int cant[5] = ([5] 0), min, idP, aux;

    while (true)
    {
        receive Llegada(idP);
        
        do (not empty(Desocupa)) -> {
            receive Desocupa(aux);
            cant[aux]--;
        } od

        min = Minimo(cant);
        cant[min]++;
        send RespuestaFila[idP](min);
    }
}

Process Caja[id: 0..4]
{
    int idP;

    while (true)
    {
        receive LlegadaFila[id](idP);
        send Turno[idP]();
        // Atiendo a la persona idP
        send Finaliza[id]();
        receive Salida[id]();
        send Desocupa(id);
    }
}