Monitor Partido
{
    int llegados[4] = ([4] 0);      // cuántos jugadores hay asignados en cada equipo
    int completos = 0;              // cuántos equipos tienen 5 jugadores
    int nroCancha[4] = ([4] -1);    // cuál es la cancha que le toca a cada equipo
    int enCancha[2] = ([2] 0);      // cuántos jugadores se encuentran en cada cancha

    Cond equipo[4], cancha[2];

    Procedure Llegada(nroEq, nroCancha : out int)
    {
        nroEq = DarEquipo();
        llegados[nroEq]++;

        if (llegados[nroEq] < 5) wait(equipo[nroEq]);
        else {
            // Soy el jugador que completa el equipo...
            nroCancha[nroEq] = completos / 2;
            completos++;
            signal_all(equipo[nroEq]);      // despertar a todo mi equipo
        }

        nroCancha = nroCancha[nroEq];       // devolver como salida al completar equipo
    }

    Procedure Jugar(nroCancha : in int)
    {
        enCancha[nroCancha]++;

        if (enCancha[nroCancha] < 10) wait(cancha[nroCancha]);
        else {
            // Soy el último jugador que llega de los 2 equipos...
            signal_all(cancha[nroCancha]);      // despertar a todos los jugadores de la cancha
        }
    }
}

Process Jugador[id: 0..19]
{
    int nroEq, nroCancha;

    Partido.Llegada(nroEq, nroCancha);
    Partido.Jugar(nroCancha);
    delay(50);
    // Retirarse sin esperar al resto...
}