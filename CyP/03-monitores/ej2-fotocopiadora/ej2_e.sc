/* Modifique la solución de (b) para el caso en que además haya un Empleado que le indica 
a cada persona cuando debe usar la fotocopiadora. */

Monitor Fotocopiadora
{
    Cola c;
    Cond solicitud, terminado;
    Cond espera[N];

    Procedure Solicitar(idP : in int)
    {
        push(c, idP);           // encolar respetando orden de llegada
        signal(solicitud);      // aviso por si empleado está dormido

        wait(espera[idP]);      // dormirse hasta que haya autorización
    }

    Procedure Liberar()
    {
        signal(terminado);      // despertar a empleado para que llame al siguiente
    }

    Procedure EsperarSolicitud(siguiente : out int)
    {
        // Se queda dormido sólo si no había nadie esperando para imprimir
        // La primera persona que llegue tendrá que despertarlo para que siga su labor

        if (empty(c)) wait(solicitud);      // evito busy-waiting
        pop(c, siguiente);                  // la cola no está vacía
    }

    Procedure Autorizar(id_autorizado : in int)
    {
        signal(espera[id_autorizado]);      // despierto a la persona
        wait(terminado);                    // sólo 1 persona fotocopia a la vez
    }
}

Process Persona[id: 0..N-1]
{
    Fotocopiadora.Solicitar(id);
    Fotocopiar();
    Fotocopiadora.Liberar();
}

Process Empleado
{
    int id_siguiente;

    while(true)
    {
        Fotocopiadora.EsperarSolicitud(id_siguiente);
        Fotocopiadora.Autorizar(id_siguiente);
    }
}