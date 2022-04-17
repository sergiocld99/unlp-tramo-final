/* Existen N personas que desean utilizar un cajero automático. En este segundo
caso se debe tener en cuenta el orden de llegada de las personas. Suponga que
hay una función UsarCajero() que simula el uso del cajero. */

Monitor Cajero
{
    Bool libre = true;
    Cond cola;                  // Condicional para los wait()
    int esperando = 0;          // Contador para evitar pop() en cola vacía

    Procedure Pasar()
    { 
        if (not libre) { 
            esperando++;
            wait(cola);         // Dormirme respetando orden de llegada
        } else libre = false;
    }

    Procedure Salir()
    { 
        if (esperando > 0) { 
            esperando--;
            signal(cola);       // Despierta al primer proceso que haya realizado wait(cola)
        } else libre = true;
    }
}

Process Persona[id: 0..N-1]
{ 
    Cajero.Pasar();             // Me agrego a la fila (no vuelve hasta que alguien lo despierte)
    UsarCajero();               // Utilizo cuando sea mi turno (ahora no está en monitor)
    Cajero.Salir();             // Producir signal para despertar al primero de la fila
}