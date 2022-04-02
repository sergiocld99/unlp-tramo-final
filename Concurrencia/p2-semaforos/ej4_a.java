sem mutex = 1;

process Persona[id: 0..N-1]
{
    P(mutex);
    Fotocopiar();
    V(mutex);
}