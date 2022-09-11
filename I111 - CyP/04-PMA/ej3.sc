chan Pedidos(texto, int);
chan Solicitud(int);
chan Siguiente[3](texto, int);
chan Ordenes(texto, int);
chan Entregas[C](texto);

// SOLUCIÓN PERFECTA!! Validado por el profe

Process Cliente[id: 0..C-1]
{
    texto P,E;

    send Pedidos(P, id);
    receive Entregas[id](E);
}

Process Coordinador
{
    texto P;
    int idV, idC = -1;

    while (true)
    {
        receive Solicitud(idV);
        
        if (empty(Pedidos)) P = "VACIO"
        else receive Pedidos(P, idC);

        send Siguiente[idV](P, idC);
    }
}

Process Vendedor[id: 0..2]
{
    texto P, orden;
    int idC;

    while (true)
    {
        send Solicitud(id);
        receive Siguiente[id](P, idC);

        if (P <> "VACIO")
        {
            orden = Convertir(P);
            send Ordenes(orden, idC);
        } else {
            delay(rand(120) + 60);      // reponer pack
        }
    }
}

Process Cocinero[id: 0..1]
{
    texto P, E;
    int idC;

    while (true)
    {
        receive Ordenes(P, idC);
        E = cocinar(P);
        send Entregas[idC](E);
    }
}