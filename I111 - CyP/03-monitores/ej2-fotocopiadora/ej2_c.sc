/* Modifique la solución de (b) para el caso en que se deba dar prioridad de acuerdo a la 
edad de cada persona (cuando la fotocopiadora está libre la debe usar la persona de 
mayor edad entre las que estén esperando para usarla). */

Monitor Fotocopiadora
{
    Cond espera[N];         // 1 cola para cada persona (contendrá hasta 1 proceso dormido cada una)
    Bool libre = true;
    ColaOrdenada c;         // Priorizada por edad 
    int siguiente;

    Procedure Solicitar(idP, edad : in int)
    {
        if (not libre) { 
            insertar(c, idP, edad);
            wait(espera[idP]);
        } else libre = false;
    }

    Procedure Liberar()
    {
        if (not empty(c)) { 
            sacar(c, siguiente);
            signal(espera[siguiente]); 
        } else libre = true;
    }
}

Process Persona[id: 0..N-1]
{
    int edad = ...;

    Fotocopiadora.Solicitar(id, edad);
    Fotocopiar();
    Fotocopiadora.Liberar();
}
