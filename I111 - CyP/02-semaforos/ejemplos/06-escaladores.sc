/* En una montaña hay 30 escaladores que en una parte de la subida deben
utilizar un único paso de a uno a la vez y de acuerdo al orden de llegada al
mismo. */

Cola C;
Boolean libre = true;

sem mutex = 1;
sem espera[30] = ([30] 0);

process Escalador[id: 0..29]
{
    int aux;                // aux = siguiente

    // llega al paso
    
    P(mutex);
    if (libre)
    {
        libre = false;
        V(mutex);
    } else {
        push(C, id); 
        V(mutex);
        P(espera[id]); 
    }

    // Usa el paso con exclusión mutua

    P(mutex);
    if (empty (C)){
        libre = true;
    } else { 
        pop(C, aux); 
        V(espera[aux]); 
    }
    
    V(mutex);
}