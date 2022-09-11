sem mutex = 1;              // Semáforo de restricción de uso de "ocupado" y cola
sem turno[N] = ([N] 0);
bool ocupado = false;       // Indicador de uso de impresora
cola C;

process Persona[id: 0..N-1]
{
    int siguiente;

    P(mutex);               // Necesito modificar cola y/o "ocupado"

    // SECCIÓN CRÍTICA 1
    if (ocupado){           // Consultar por ocupado
        push(C, id);        // Encolar para respetar orden
        V(mutex);           // Liberar SC
        P(turno[id]);       // Esperar que alguien habilite mi turno
    } else {
        ocupado = true;     // Apropiar impresora modificando "ocupado"
        V(mutex);           // Liberar SC
    }

    Fotocopiar();           // Usar recurso mientras los demás esperan o se encolan

    P(mutex);               // Necesito modificar cola y/o "ocupado"

    // SECCIÓN CRÍTICA 2
    if (not empty(C)){          // Si hay alguien esperando...
        pop(C, siguiente);      // Veo quién es el primero de la fila
        V(turno[siguiente]);    // Habilito su turno para imprimir (y dejo "ocupado" en true)
    } else {
        ocupado = false;        // Liberar impresora para el primero que llegue después
    }

    V(mutex);               // Liberar SC
}