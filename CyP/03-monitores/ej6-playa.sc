Monitor Playa
{
    
}

Monitor Equipo[id: 0..4]
{
    // Se espera hasta que el equipo esté completo
    Procedure Llegada()
    {
        
    }
}

Process Persona[id: 0..19]
{
    int nroEquipo = ...;    // se conoce este dato

    Equipo[nroEquipo].Llegada();
}