Monitor Club
{
    int llegados[4] = ([4] 0);      // cuántos jugadores hay asignados en cada equipo
    int completos = 0;              // cuántos equipos tienen 5 jugadores
    int nroCancha[4] = ([4] -1);    // cuál es la cancha que le toca a cada equipo
    Cond equipo[4];

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
}

Monitor Cancha[id: 0..2]
{
    int llegaron = 0;
    Cond espera_inicio, espera_fin;

    Procedure Llegada(){
        llegaron++;
        if (llegaron < 10) wait(espera_inicio);
        else signal_all(espera_inicio);
    }

    Procedure EsperaComienzo(){
        wait(espera_inicio);
    }

    Procedure Jugar(){
        wait(espera_fin);
    }

    Procedure Terminar(){
        signal_all(espera_fin);
    }
}

Process Jugador[id: 0..19]
{
    int nroEq, nroCancha;

    Club.Llegada(nroEq, nroCancha);
    Cancha[nroCancha].Llegada();
    Cancha[nroCancha].Jugar();
    // Retirarse sin esperar al resto...
}

Process Partido[id: 0..1]
{
    Cancha[id].EsperaComienzo();
    delay(50);
    Cancha[id].Terminar();   
}