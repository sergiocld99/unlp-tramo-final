/* Se debe simular una maratón con C corredores donde en la llegada hay UNA máquinas
expendedoras de agua con capacidad para 20 botellas. Además existe un repositor
encargado de reponer las botellas de la máquina. Cuando los C corredores han llegado al
inicio comienza la carrera. Cuando un corredor termina la carrera se dirigen a la máquina
expendedora, espera su turno (respetando el orden de llegada), saca una botella y se retira. Si
encuentra la máquina sin botellas, le avisa al repositor para que cargue nuevamente la
máquina con 20 botellas; espera a que se haga la recarga; saca una botella y se retira. Nota:
maximizar la concurrencia; mientras se reponen las botellas se debe permitir que otros
corredores se encolen. */

Monitor Carrera
{
    int cant = 0;
    Cond largada;

    Procedure Llegada()
    {
        cant++;
        if (cant == C) signal_all(largada);
        else wait(largada);
    }
}

Monitor Expendedora
{
    int botellas = 20;
    bool libre = true;
    Cond espera, rep, listo;

    Procedure Usar()
    {
        if (not libre) wait(espera);

        if (botellas == 0){
            libre = false;
            signal(rep);        // avisar a repositor
            wait(listo);        // permitir que se siga haciendo fila
            libre = true;
        }

        botellas--;
        signal(espera);         // despertar al siguiente (si hay)
    }

    Procedure Reponer()
    {
        if (botellas > 0) wait(rep);    // dormirme sólo si hay botellas
        botellas = 20;
        signal(listo);                  // despertar a corredor
    }
}

Process Corredor[id: 0..C-1]
{
    Carrera.Largada();
    // Correr hasta la meta...
    Expendedora.Usar();
}

Process Repositor
{
    while(true) -> Expendedora.Reponer();
}