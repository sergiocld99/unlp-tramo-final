/* Resolver con PMS (Pasaje de Mensajes SINCRÓNICOS) el siguiente problema. En un
estadio de fútbol hay una máquina expendedora de gaseosas que debe ser usada por E
Espectadores de acuerdo al orden de llegada. Cuando el espectador accede a la máquina en
su turno usa la máquina y luego se retira para dejar al siguiente. Nota: cada Espectador una
sólo una vez la máquina. */

// OPCIÓN 2: LOS ESPECTADORES SE ORGANIZAN PARA PASARSE EL TURNO

Process Espectador[id: 0..E-1]
{
    Expendedora!pedido(id);
    Expendedora?usar();

    // usa la máquina...

    Expendedora!retiro();
}

Process Expendedora
{
    Cola C;
    int aux;
    bool libre = true;

    do Espectador[*]?pedido(aux) -> if (libre) {libre = false; Espectador[aux]!usar(); }
                                    else push(C, aux);

    [] Espectador[*]?retiro() ->    if (empty(C)) libre = true;
                                    else {pop(C, aux); Espectador[aux]!usar(); } 
    od
}