/* Implementar el acceso a una base de datos de solo lectura que puede atender a lo sumo 5 
consultas simultáneas */

Monitor BaseDeDatos
{
    int esperando = 0;
    int libres = 5;
    Cond cola;

    Procedure Solicitar()
    {
        if (libres == 0)
        {
            esperando++;
            wait(cola);
        } else libres--;
    }

    Procedure Salir()
    {
        if (esperando > 0)
        {
            esperando--;
            signal(cola);
        } else libres++;
    }
}

Procedure Proceso[id: 0..N-1]
{
    BaseDeDatos.Solicitar();
    LeerBD();
    BaseDeDatos.Salir();
}