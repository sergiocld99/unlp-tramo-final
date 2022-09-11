cola C[3];
int esperando[3] = ([3] 0);
int pasaron = 0;
bool seguir = true;

sem mutex = 1;                  // uso de esperando[]
sem mutexCola[3] = ([3] 1);     // uso de C
sem solicitud[3] = ([3] 0);     // C no vacía
sem turno[150] = ([150] 0);
sem termino[3] = ([3] 0);       // el hisopado
sem retirado[3] = ([3] 0);      // que se retire el pasajero
sem mutexFin = 1;               // uso de pasaron y seguir

Process Pasajero[id: 0..149]
{
    int cola, min = 9999;
    
    P(mutex);
    for i = 0..2 -> if (esperando[i] < min)
                    {
                        cola = i;
                        min = esperando[i];
                    }
    esperando[cola]++;
    V(mutex);

    P(mutexCola[cola]);
    push(C[cola], id);
    V(mutexCola[cola]);

    V(solicitud[cola]);
    P(turno[id]);
    // Le realizan el hisopado
    P(termino[cola]);
    // Retirarse
    V(retirado[cola]);
}

Process Enfermera[id: 0..2]
{
    int actual;

    P(solicitud[id]);

    P(mutexFin);
    while (seguir)                              // evito while(true)
    {
        if (++pasaron == 150){
            seguir = false;
            for i = 0..2 -> V(solicitud[i]);
        }
        V(mutexFin);
        
        P(mutexCola[id]);
        pop(C[id], actual);
        V(mutexCola[id]);

        V(turno[actual]);
        Hisopar();
        V(termino[id]);
        P(retirado[id]);
        P(solicitud[id]);

        P(mutexFin);
    }

    V(mutexFin);
}