Process Uno
{
    texto M;

    while(true)
    {
        M = generar();
        Admin!muestras(M);
    }
}

Process Dos
{
    texto M, A, R;

    while(true)
    {
        Admin!pedido();         // indicar que estoy libre
        Admin?muestras(M);      // recibir muestras 
        A = armarSet(M);
        Tres!analisis(A);       // pedir a Tres que haga el análisis
        Tres?resultados(R);     // esperar por resultados y archivarlo
    }
}

Process Tres
{
    texto A, R;

    while(true)
    {
        Dos?analisis(A);        // recibir set de analisis
        R = analizar(A);
        Dos!resultados(R);      // mandar resultados
    }
}

Process Admin
{
    texto M;
    Cola C;

    // primera sentencia no tiene condicional -> siempre verdadera -> bucle infinito
    // segunda sentencia tiene condicional not empty(C) -> evitar pop en C vacía

    do Uno?muestras(M) -> push(C, M);
    [] not empty(C); Dos?pedido() -> Dos!muestras(pop(C));
    od
}