/* En un corralón de materiales hay un empleado que debe atender a N clientes de acuerdo al 
orden de llegada. Cuando un cliente es llamado por el empleado para ser atendido, le da una 
lista con los productos que comprará, y espera a que el empleado le entregue el 
comprobante (recibo) de la compra realizada. */

Monitor Corralon
{
    Cola c;         // para que el empleado conozca el orden de llegada
    Cond solicitud, entrega_lista, entrega_com, espera[N];
    text lista, recibo;
    int idAux;

    Procedure Llegada(idC : in int)
    {
        push(c, idC);
        signal(solicitud);
        wait(espera[idC]);
    }

    Procedure Comprar(L: in text; R: out text)
    {
        lista = L;
        signal(entrega_lista);

        wait(entrega_com);
        R = recibo;
    }

    Procedure Atender(L: out text)
    {
        if (empty(c)) wait(solicitud);
        pop(c, idAux);
        signal(espera[idAux]);

        wait(entrega_lista);
        L = lista;
    }

    Procedure EntregarRecibo(R: in text)
    {
        recibo = R;
        signal(entrega_com);
    }
}

Process Cliente[id: 0..N-1]
{
    text lista = ...;
    text recibo;

    Corralon.Llegada(id);
    Corralon.Comprar(lista, recibo);
    // retirarme
}

Process Empleado
{
    text lista_productos, recibo;

    while(true)
    {
        Corralon.Atender(lista_productos);
        recibo = facturar(lista_productos);
        Corralon.EntregarRecibo(recibo);
    }
}