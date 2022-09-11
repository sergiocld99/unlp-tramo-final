cola C;
bool libre = true;
int llegaron = 0;
int idAux;
int reservadas[5] = ([5] 0);
int producidas[5] = ([5] 0);
int juntados[5] = ([5] 0);

sem mutex = 1;                  // libre y C
sem mutexGrupo[5] = ([5] 1);    // reservadas[], juntados[] y producidas[]
sem turno[15] = ([15] 0);
sem termino[5] = ([5] 0);
sem barrera[5] = ([5] 0);       // para juntados

Process Empleado[id: 0..14]
{
    int grupo;
    
    P(mutex);
    if (libre)
    {
        libre = false;
        V(mutex);
   } else {
       push(C, id);
       V(mutex);

       P(turno[id]);
   }

    grupo = llegados / 3;
    llegados++;

    P(mutex);
    if (empty(C)) libre = true;
    else {
        pop(C, idAux);
        V(turno[idAux]);
    }
    V(mutex);

    P(mutexGrupo[grupo]);
    while (reservadas[grupo] < P)
    {
        reservadas[grupo]++;
        V(mutexGrupo[grupo]);

        // Producir unidad...
        P(mutexGrupo[grupo]);
        if (++producidas[grupo] == P)
        {
            for i = 1..3 -> V(termino[grupo]);
        }
    }

    P(termino[grupo]);
    P(mutexGrupo[grupo]);
    if (++juntados == 3) for i = 1..3 -> V(barrera[grupo]);
    V(mutexGrupo[grupo]);

    P(barrera[grupo]);
    // Retirarse en grupo
}