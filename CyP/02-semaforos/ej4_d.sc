// Corrección: todas las personas deben tener un código similar
int inicio = ...;
int restantes = N;
sem turno[N] = ([N] 0);

process Persona[id: 0..N-1]
{
    // Parte 1: me fijo si tengo exclusividad en usar primero
    if (not inicio = id) -> P(turno[id]);

    Fotocopiar();

    // Parte 2: liberar o pasar turno (considero caso cíclico)
    restantes = restantes - 1;
    if (restantes > 0) -> V(turno[(id+1) % N]);
}