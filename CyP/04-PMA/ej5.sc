chan Documentos(texto), Pedido(int), Siguiente[3](texto);
chan DocumentosDirector(texto), Signal(char);

// VALIDADO POR EL PROFE
Process Usuario[id: 0..N-1]
{
    texto doc;

    while(true)
    {
        doc = generarDoc();
        send Documentos(doc);
        send Signal();
        // trabajar
    }
}

Process Director
{
    texto doc;

    while(true)
    {
        doc = generarDoc();
        send DocumentosDirector(doc);
        send Signal();
        // trabajar
    }
}

Process Impresora[id: 0..2]
{
    texto doc;

    while(true)
    {
        send Pedido(id);
        receive Siguiente[id](doc);
        // imprimir doc
    }
}

Process Coordinador
{
    texto doc;
    int idImp;

    while(true)
    {
        receive Pedido(idImp);
        receive Signal();

        if (not empty(DocumentosDirector)) receive DocumentosDirector(doc);
        else receive Documentos(doc);

        send Siguiente[idImp](doc);
    }
}