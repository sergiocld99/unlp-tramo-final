/* Partiendo de la solución anterior
hacemos una cambio en el
enunciado, agregando que si llega
una persona anciana tiene prioridad. */

Monitor Cajero
{
    Bool libre = true;
    Cond espera[N];             // Condicional para cada persona (prioridad rompe orden de llegada)
    ColaOrdenada fila;          // Cola para ordenar según la edad de cada persona
    int esperando = 0;          // Contador para evitar sacar() en cola vacía
    int idAux;                  // Siguiente en la fila

    // Se recibe el identificador de proceso y su edad para la prioridad
    Procedure Pasar(idP, edad : in int)
    { 
        if (not libre) { 
            esperando++;
            insertar(fila, idP, edad);
            wait(espera[idP]);          // Me duermo en una cola única donde estoy solamente yo
        } else libre = false;
    }

    Procedure Salir()
    { 
        if (esperando > 0) { 
            esperando--;
            sacar(fila, idAux);         // Saco al de mayor edad
            signal(espera[idAux]);      // Despierto a esa persona (dicha cola queda vacía)
        } else libre = true;
    }
}

Process Persona[id: 0..N-1]
{ 
    int edad = ...;

    Cajero.Pasar(id, edad);     // Me agrego a la fila (no vuelve hasta que alguien lo despierte)
    UsarCajero();               // Utilizo cuando sea mi turno (ahora no está en monitor)
    Cajero.Salir();             // Producir signal para despertar al primero de la fila
}