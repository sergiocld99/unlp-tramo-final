/* Modifique la solución de (a) para el caso en que se deba respetar el orden de llegada */

Monitor Fotocopiadora
{
    Cond cola;
    Bool libre = true;
    int esperando = 0;

    Procedure Solicitar()
    {
        // Es necesario consultar por libre = true, sino nadie me podrá despertar
        if (not libre) { esperando++; wait(cola); } 
        else libre = false;
    }

    Procedure Liberar()
    {
        if (esperando > 0) { esperando--; signal(cola); } 
        else libre = true;
    }
}

Process Persona[id: 0..N-1]
{
    Fotocopiadora.Solicitar();
    Fotocopiar();                   // Otras personas pueden solicitar mientras tanto
    Fotocopiadora.Liberar();
}