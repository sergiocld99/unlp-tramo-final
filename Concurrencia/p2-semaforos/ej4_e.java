// En este ejercicio sólo importa el orden de llegada

sem mutex = 1;
sem solicitud = 0;
sem continuar = 0;
sem turno[N] = ([N] 0);
cola C;

process Persona[id: 0..N-1]
{
    P(mutex);       // Esperar para modificar cola
    push(C,id);     // Encolar atómicamente
    V(mutex);       // Liberar uso de cola

    V(solicitud);   // Avisar a Empleado que alguien quiere usar impresora (yo)
    P(turno[id]);   // Esperar autorización de Empleado
    
    Fotocopiar();
    V(continuar);   // Permitir que Empleado siga escuchando solicitudes
}

process Empleado
{
    int siguiente;

    while(true)
    {
        P(solicitud);           // Aguardar por encolamiento confirmado

        P(mutex);               // Esperar para modificar cola
        pop(C, siguiente);      // Desencolar atómicamente
        V(mutex);               // Liberar uso de cola

        V(turno[siguiente]);    // Autorizar impresión al primero de la fila
        P(continuar);           // Aguardar que termine de imprimir (1 persona a la vez)
    }
}