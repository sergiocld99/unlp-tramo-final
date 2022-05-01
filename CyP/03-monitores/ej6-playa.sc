Monitor Playa
{
    
    // ¿sólo 1 persona puede juntar 1 moneda a la vez?
    Procedure JuntarMoneda(valor : out int)
    {
        // Conseguir moneda de $1, $2 o $5
        valor = Moneda();       // ¿esto es inmediato?
    }
}

Monitor Equipo[id: 0..4]
{
    int llegaron = 0, total = 0, terminaron = 0;
    Cond espera_inicio, espera_fin;

    // Se espera hasta que el equipo esté completo
    Procedure Llegada()
    {
        llegaron++;
        if (llegaron == 4) signal_all(espera_inicio);
        else wait(espera_inicio);
    }

    // Se espera hasta que todo el equipo haya terminado
    Procedure Sumar(miSuma : in int, sumaGrupo : out int)
    {
        total = total + miSuma;     // sumar a partir de entrada
        terminaron++;

        if (terminaron == 4) signal_all(espera_fin);
        else wait(espera_fin);

        sumaGrupo = total;          // devolver total como salida
    }
}

Process Persona[id: 0..19]
{
    int nroEquipo = ...;    // se conoce este dato
    int valor, sumaGrupo, miSuma = 0;

    Equipo[nroEquipo].Llegada();
    
    for i = 1..15 -> {
        Playa.JuntarMoneda(valor);
        miSuma = miSuma + valor;
    }

    Equipo[nroEquipo].Sumar(miSuma, sumaGrupo);
}