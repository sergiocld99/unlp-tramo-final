// Respetando orden de llegada
int siguiente = -1;
Cola c;

process Persona[id: 0..N-1]
{
    Documento documento;
    
    < if (siguiente = -1) siguiente = id;
    else push(c, id); >

    < await(siguiente = id); >
    imprimir(documento);

    < if (empty(c)) siguiente = -1;
    else pop(c, siguiente); >
}