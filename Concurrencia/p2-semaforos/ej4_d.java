sem usado[N] = ([N] 0);

process Persona[id: 0..N-1]
{
    if (not id == 0){
        // Si soy la persona X, esperar que X-1 haya usado la impresora
        P(usado[id-1]);
    }

    Fotocopiar();

    // Marcar como usado una vez terminada la impresión
    V(usado[id]);
}