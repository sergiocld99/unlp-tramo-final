/* Existen N personas que desean utilizar un cajero automático. En este primer
caso no se debe tener en cuenta el orden de llegada de las personas (cuando está
libre cualquiera lo puede usar). Suponga que hay una función UsarCajero() que
simula el uso del cajero. */

Monitor Cajero
{
    Procedure PasarAlCajero()
    {
        UsarCajero();           // Recordemos: sólo 1 proceso a la vez puede usar el monitor
    }
}

Process Persona[id: 0..N-1]
{
    Cajero.PasarAlCajero();     // Espero a que el monitor esté libre y lo libero una vez usado
}