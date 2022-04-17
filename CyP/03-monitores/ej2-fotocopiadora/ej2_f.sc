/* Modificar la solución (e) para el caso en que sean 10 fotocopiadoras. El empleado le 
indica a la persona cuando puede usar una fotocopiadora, y cual debe usar.  */

Monitor Fotocopiadora
{
    Cola c;
    Cond solicitud, liberado;
    Cond espera[N];
    Cola disponibles;               // Fotocopiadoras sin persona asignada
    int asignada[N] = ([N] -1);     // Fotocopiadora asignada a cada persona (0 a 9)

    // AHORA SE AGREGA EL ID DE RECURSO COMO SALIDA
    Procedure Solicitar(idP : in int, idRec : out int)
    {
        push(c, idP);               // encolar respetando orden de llegada
        signal(solicitud);          // aviso por si empleado está dormido

        wait(espera[idP]);          // dormirse hasta que haya autorización
        idRec = asignada[idP];      // parámetro de salida
    }

    // AHORA SE ENCOLA COMO DISPONIBLE
    Procedure Liberar(idRec : in int)
    {
        push(disponibles, idRec);
        signal(liberado);           // aviso por si empleado está dormido
    }

    Procedure Crear()
    {
        for i = 0..9 -> push(disponibles, i);
    }

    // SIN CAMBIOS
    Procedure EsperarSolicitud(siguiente : out int)
    {
        // Se queda dormido sólo si no había nadie esperando para imprimir
        // La primera persona que llegue tendrá que despertarlo para que siga su labor

        if (empty(c)) wait(solicitud);              // evito busy-waiting por cola de solicitudes
        pop(c, siguiente);                          // la cola de solicitudes no está vacía
    }

    // AHORA SE DEBE INDICAR CUÁL USAR
    Procedure Autorizar(id_autorizado : in int)
    {
        // Se queda dormido sólo si no había ninguna fotocopiadora libre para asignar
        // La primera persona que libere un recurso tendrá que despertarlo para que siga su labor

        pop(disponibles, asignada[id_autorizado]);  // la cola de recursos no está vacía
        signal(espera[id_autorizado]);              // despierto a la persona

        if (empty(disponibles)) wait(liberado);     // evito busy-waiting por cola de recursos
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

    Fotocopiadora.Crear();

    while(true)
    {
        Fotocopiadora.EsperarSolicitud(id_siguiente);
        Fotocopiadora.Autorizar(id_siguiente);
    }
}