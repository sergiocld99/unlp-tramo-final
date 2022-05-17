/* Suponga que existe un antivirus distribuido en él hay R procesos robots que continuamente
están buscando posibles sitios web infectados; cada vez que encuentran uno avisan la
dirección y continúan buscando. Hay un proceso analizador que se encargue de hacer todas
las pruebas necesarias con cada uno de los sitios encontrados por los robots para
determinar si están o no infectados. */

Process Robot[id: 0..R-1]
{
    texto direccion;

    while(true)
    {
        direccion = findSitioPeligroso();
        Admin!virus(direccion);                // no esperar respuesta -> no enviar id
    }
}

Process Admin
{
    texto dir;
    Cola C;

    // en la primera sentencia se reciben direcciones de cualquier robot -> sin condicional
    // en la segunda sentencia se envia al analizador -> condicional C no vacia

    do Robot[*]?virus(dir) -> push(C, dir);
    [] not empty(C); Analizador?pedido() -> Analizador!virus(pop(C));
    od
}

Process Analizador
{
    texto dir, resultado;

    while(true)
    {
        Admin!pedido();                 // solicitar porque estoy libre
        Admin?virus(dir);               // recibir dirección a revisar
        resultado = analizar(dir);      // guardar resultado para si mismo
    }
}