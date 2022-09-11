Monitor Entrenamiento
{
    int cant = 0;

    Procedure ObtenerCancha(nro : out int)
    {
        nro = cant % 2;
        cant++;
    }
}

Monitor Cancha[id: 0..1]
{
    int cant = 0;
    Cond esperaFin, inicio;

    Procedure Llegada()
    {
        if (++cant == 10) signal(inicio);
        wait(esperaFin);
    }

    Procedure Comenzar()
    {
        if (cant < 10) wait(inicio);
    }

    Procedure Terminar()
    {
        signal_all(esperaFin);
    }
}


Process Jugador[id: 0..19]
{
    int nroC;

    Entrenamiento.ObtenerCancha(nroC);
    // Dirigirse hacia la cancha nroC
    Cancha[nroC].Llegada();
    // Retirarse
}

Process Partido[id: 0..1]
{
    Cancha[id].Comenzar();
    delay(40 minutos);
    Cancha[id].Terminar();
}