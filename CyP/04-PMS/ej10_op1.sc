/* Resolver con PMS (Pasaje de Mensajes SINCRÓNICOS) el siguiente problema. En un
estadio de fútbol hay una máquina expendedora de gaseosas que debe ser usada por E
Espectadores de acuerdo al orden de llegada. Cuando el espectador accede a la máquina en
su turno usa la máquina y luego se retira para dejar al siguiente. Nota: cada Espectador una
sólo una vez la máquina. */

// OPCIÓN 1: LA EXPENDEDORA ES UN PROCESO "EMPLEADO" QUE DELEGA TURNOS

Process Espectador[id: 0..E-1]
{
    Fila!pedido(id);
    Expendedora?usar();

    // usa la máquina...

    Expendedora!retiro();
}

Process Fila
{
    Cola C;
    int aux;

    do Espectador[*]?pedido(aux) -> push(C, aux);
    [] not empty(C); Expendedora?pedido() -> Expendedora!turno(pop(C));
    od
}

Process Expendedora
{
    int idEspectador;

    for i = 1..E -> {
        Fila!pedido();
        Fila?turno(idEspectador);
        Espectador[idEspectador]!usar();
        Espectador[idEspectador]?retiro();
    }
}