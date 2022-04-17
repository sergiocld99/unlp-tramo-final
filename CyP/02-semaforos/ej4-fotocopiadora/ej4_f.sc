// Nuevamente, sólo importa el orden de llegada, pero hay 10 impresoras

sem mutex = 1;
sem mutexImp = 1;
sem solicitud = 0;
sem libres = 0;
sem turno[N] = ([N] 0);

int asignado[N] = ([N] -1);     // ID de impresora para cada persona (0 a 9)
cola impresoras;                // Cola de IDs de impresoras libres
cola C;                         // Cola de IDs de personas (solicitudes)

process Persona[id: 0..N-1]
{
    int impresoraId;

    P(mutex);       // Esperar para modificar cola de solicitudes
    push(C,id);     // Encolar atómicamente
    V(mutex);       // Liberar uso de cola de solicitudes

    V(solicitud);       // Confirmar que "C" no está vacía
    P(turno[id]);       // Esperar autorización para imprimir

    impresoraId = asignado[id];
    Fotocopiar(impresoraId);            // Usar impresora que Empleado me asignó

    P(mutexImp);                        // Esperar para modificar cola de impresoras
    push(impresoras, impresoraId);      // Depositar como libre (atómicamente)
    V(mutexImp);                        // Liberar uso de cola de impresoras

    V(libres);                          // Confirmar que "impresoras" no está vacía
}

process Empleado
{
    int siguiente, impresoraId;

    // Generar impresoras
    for i=0..9
    {
        push(impresoras, i);
        V(libres);
    }

    while(true)
    {
        P(solicitud);       // Aguardar por encolamiento confirmado ("C" no vacía)
        P(libres);          // Esperar por impresora libre ("impresoras" no vacía)

        P(mutex);               // Esperar para modificar cola de solicitudes
        pop(C, siguiente);      // Obtener primero de la fila (atómicamente)
        V(mutex);               // Liberar uso de cola de solicitudes

        P(mutexImp);                            // Esperar para modificar cola de impresoras
        pop(impresoras, impresoraId);           // Obtener impresora libre (atómicamente)
        V(mutexImp);                            // Liberar uso de cola de impresoras

        asignado[siguiente] = impresoraId;      // Asignar ID de impresora que debe usar
        V(turno[siguiente]);                    // Autorizar utilización a la persona
    }
}