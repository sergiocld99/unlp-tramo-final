Monitor Negocio
{
    cola D;                     // empleados disponibles
    Cond espera;

    Procedure Llegada(nroE : out int)
    {
        if (empty(D)) wait(espera);
        pop(D, nroE);
    }

    Procedure Liberar(nroE : in int)
    {
        push(D, nroE);
        signal(espera);
    }
}

Monitor Atencion[id: 0..4]
{
    text L, P;
    boolean listo = false;
    Cond espera_empleado, espera_cliente;
    
    Procedure Entregar(lista : in text, presupuesto : out text)
    {
        L = lista;
        listo = true;
        signal(espera_empleado);
        wait(espera_cliente);
        presupuesto = P;
        signal(espera_empleado);
    }

    Procedure Atender(lista : out text)
    {
        if (not listo) wait(espera_empleado);
        lista = L;
    }

    Procedure Cobrar(presupuesto : in text)
    {
        P = presupuesto;
        signal(espera_cliente);
        wait(espera_empleado);
    }
}

Process Persona[id: 0..N-1]
{
    int idE;
    text listaP, presupuesto;

    Negocio.Llegada(idE);
    Atencion[idE].Entregar(listaP, presupuesto);
}

Process Empleado[id: 0..4]
{
    text lista, presupuesto;

    while(true)
    {
        Atencion[id].Atender(lista);
        presupuesto = HacerPresupuesto(lista);
        Atencion[id].Cobrar(presupuesto);
        Negocio.Liberar(id);
    }
}