/* Una empresa de turismo posee 4 combis con capacidad para 25 personas cada una y UN 
vendedor que vende los pasajes a los clientes de acuerdo al orden de llegada. Hay C clientes
que al llegar intentan comprar un pasaje para una combi en particular (el cliente conoce este 
dato); si aún hay lugar en la combi seleccionada se le da el pasaje y se dirige hacia la combi; 
en caso contrario se retira. Cada combi espera a que suban los 25 pasajeros, luego realiza el 
viaje, y cuando llega al destino deja bajar a todos los pasajeros. Nota: maximizar la 
concurrencia; suponga que para cada combi al menos 25 clientes intentarán comprar pasaje */

Cola c;
sem mutex = 1;                      // Semáforo para uso atómico de cola
sem solicitud = 0;                  // Semáforo para evitar pop() en cola vacía
sem turno[C] = ([C] 0);             // Semáforo para despertar al cliente indicado
sem espera_combi[4] = ([4] 0);      // Semáforo para no arrancar las combis antes de tiempo
sem termino_viaje[4] = ([4] 0);     // Semáforo para bajar a los pasajeros al final del viaje
Bool disponible[C] = ([C] false);

process Cliente[id: 0..C-1]
{
    int nro_combi = ...;            // Ya conozco este dato
    
    P(mutex);
    push(c, (id, nro_combi));       // Encolar tupla
    V(mutex);

    V(solicitud);                   // Despertar al vendedor
    P(turno[id]);                   // Dormirse hasta que la solicitud se haya procesado
    
    if (disponible[id])
    {
        V(continuar);                   // Avisar que subí a la combi
        P(termino_viaje[nro_combi]);    // Esperar hasta el fin del recorrido
    } else {
        // Retirarse
    }
}

process Vendedor
{
    int idCliente, nro_combi;
    int contador[4] = ([4] 0);          // Control de límite de pasajeros

    while(true)
    {
        P(solicitud);                   // Esperar a un cliente en la fila
        P(mutex);
        pop(c, (idCliente, nro_combi));     // Desencolar tupla
        V(mutex);

        if (contador[nro_combi] < 25)
        {
            contador[nro_combi] = contador[nro_combi] + 1;
            disponible[idCliente] = true;   // Hay lugar en la combi
            V(turno[idCliente]);            // Despertar al cliente
            P(continuar);                   // Dormirse hasta que el cliente haya subido*
            if (contador[nro_combi] = 25) -> V(espera_combi);
        } else {
            V(turno[idCliente]);            // No hay lugar -> solo despertarlo (disponible en false)
        }
    }
}

process Combi[id: 0..3]
{
    P(espera_combi);                        // Dormirse hasta que hayan subido mis 25 pasajeros
    viajarHastaDestino();
    for i=1..25 -> V(termino_viaje[id]);    // Despertar a todos mis pasajeros
}

// * Si no esperamos, puede pasar que la combi arranque sin algunos pasajeros (quedaron a pata xd)