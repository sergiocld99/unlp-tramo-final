// Mediante un coordinador
int siguiente = -1;
Bool enUso = false;
ColaEspecial c;

process Persona[id: 0..N-1]
{
    Documento documento;
    int edad = ...;

    < agregar(c, id, edad); >
    < await(siguiente = id); >
    imprimir(documento);
    enUso = false;
}

process Coordinador
{
    while(true)
    {
        < await(not empty(c) & not enUso); >
        enUso = true;
        < siguiente = sacar(c); >
    }
}
