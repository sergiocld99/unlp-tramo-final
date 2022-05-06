Monitor Martillo
{
    int esperando = 0;
    bool libre = true;
    Cond espera;

    Procedure Solicitar()
    {
        if (libre) libre = false;
        else {
            esperando++;
            wait(espera);
        }
    }

    Procedure Liberar()
    {
        if (esperando == 0) libre = true;
        else {
            esperando--;
            signal(espera);
        }
    }
}

Monitor Equipo[id: 0..4]
{
    int cant = 0;
    int posicion;
    Cond esperaFin;

    Procedure Llegada(pos : out int)
    {
        cant++;
        if (cant < 6) wait(esperaFin);
        else {
            Jugar();
            Coordinador.Terminar(posicion);
            signal_all(esperaFin);
        }

        pos = posicion;
    }
}

// JUSTIFICACIÓN: Si fuera un proceso, no sabría a qué Equipo atender primero
Monitor Coordinador
{
    int posActual = 1;

    Procedure Terminar(pos : out int)
    {
        pos = posActual;
        posActual++;
    }
}

Process Jugador[id: 0..29]
{
    int grupo, posicion;
    
    Martillo.Solicitar();
    Romper(grupo);
    Martillo.Liberar();

    Equipo[grupo].Llegada(posicion);
}