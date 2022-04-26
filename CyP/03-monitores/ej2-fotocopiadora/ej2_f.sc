/* Modificar la solución (e) para el caso en que sean 10 fotocopiadoras. El empleado le 
indica a la persona cuando puede usar una fotocopiadora, y cual debe usar.  */

Monitor Fotocopiadora
{
    Cola c;
    Cond solicitud, liberado, espera;
    Cola disponibles = ...;         // Fotocopiadoras sin persona asignada (cola ya cargada)
    int asignada[N] = ([N] -1);     // Fotocopiadora asignada a cada persona (0 a 9)
    int idAux;

    // AHORA SE AGREGA EL ID DE RECURSO COMO SALIDA
    Procedure Solicitar(idP : in int, idRec : out int)
    {
        push(c, idP);               // encolar respetando orden de llegada
        signal(solicitud);          // aviso por si empleado está dormido
        wait(espera);
        idRec = asignada[idP];      // parámetro de salida
    }

    // AHORA SE ENCOLA COMO DISPONIBLE
    Procedure Liberar(idRec : in int)
    {
        push(disponibles, idRec);
        signal(liberado);           // aviso por si empleado está dormido
    }

    Procedure Atender()
    {
        // Se queda dormido sólo si no había nadie esperando para imprimir
        // La primera persona que llegue tendrá que despertarlo para que siga su labor

        if (empty(c)) wait(solicitud);              // evito busy-waiting por cola de solicitudes
        pop(c, idAux);

        // Se queda dormido sólo si no había ninguna fotocopiadora libre para asignar
        // La primera persona que libere un recurso tendrá que despertarlo para que siga su labor

        if (empty(disponibles)) wait(liberado);     // evito busy-waiting por cola de recursos
        pop(disponibles, asignada[idAux]);
        signal(espera);
    }
}

Process Persona[id: 0..N-1]
{
    int rec_asignado;

    Fotocopiadora.Solicitar(id, rec_asignado);
    Fotocopiar(rec_asignado);
    Fotocopiadora.Liberar(rec_asignado);
}

Process Empleado
{
    int id_siguiente;

    while(true)
    {
        Fotocopiadora.Atender();
    }
}