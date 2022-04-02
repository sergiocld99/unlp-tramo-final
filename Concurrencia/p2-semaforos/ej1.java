/* Un sistema operativo mantiene 5 instancias de un recurso almacenadas en una cola, cuando
un proceso necesita usar una instancia del recurso la saca de la cola, la usa y cuando termina
de usarla la vuelve a depositar. */

// Variables globales
sem mutex = 1;
sem instancia = 5;
cola C;

// Procesos
process Proceso[id: 0..N-1]
{
  // Variables locales al proceso
  recurso rec;

  while(true)
  {
    P(instancia);       // Esperar hasta que haya una instancia libre

    P(mutex);           // Esperar turno para la sección crítica
    pop(C, rec);        // Sacar instancia de la cola
    V(mutex);           // Libero la sección crítica

    usar(rec);          // Hacer uso del recurso de manera atómica

    P(mutex);           // Esperar turno para la sección crítica
    push(C, rec);       // Depositar instancia en la cola
    V(mutex);           // Libero la sección crítica

    V(instancia);       // Libero la instancia
  }
}