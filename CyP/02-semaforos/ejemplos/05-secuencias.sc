/* En una empresa de genética hay N clientes que envían secuencias de ADN para
que sean analizadas y esperan los resultados para poder continuar. Para resolver
estos análisis la empresa cuenta con 2 servidores que van alternando su uso para
no exigirlos de más (en todo momento uno está trabajando y el otro
descansando); cada 5 horas cambia en servidor con el que se trabaja. El servidor
que está trabajando, toma un pedido (de a uno de acuerdo al orden de llegada de
los mismos), lo resuelve y devuelve el resultado al cliente correspondiente.
Cuando terminan las 5 horas se intercambian los servidores que atienden los
pedidos. Si al terminar las 5 horas el servidor se encuentre atendiendo un
pedido, lo termina y luego se intercambian los servidores */

int resultados[N];          // ¿Por qué es un vector de enteros?
Bool finTiempo = false;
Cola C;

sem mutex = 1;              // Semáforo de uso atómico de cola
sem pedidos = 0;            // Semáforo de solicitudes pendientes (cola no vacía)
sem inicio = 0;             // Semáforo de temporizador activado
sem espera[N] = ([N] 0);    // Semáforo para cada cliente
sem turno[2] = (1, 0);      // No estaba prohibido? (cara de La Roca con la ceja levantada)

process Cliente[id: 0..N-1]
{ 
    Secuencia S;

    while (true)
    { 
        // generar secuencia S
        P(mutex);
        push(C, (id, S));   // Encolar tupla
        V(mutex);
        V(pedidos);         // pedidos = solicitud
        P(espera[id]);      // espera = aviso de procesamiento finalizado
        // ver resultado de resultados[id]
    }
}

process Servidor[id: 0..1]          // Se tienen 2 servidores
{ 
    Secuencia sec; 
    int aux; 
    Bool ok;
    
    while (true)
    { 
        P(turno[id]);
        finTiempo = false;
        V(inicio);
        ok = true;

        while (ok)                  // No es busy-waiting
        { 
            P(pedidos);
            if (finTiempo) { 
                ok = false;
                V(turno[1-id]);     // Despertar al otro servidor (y yo me duermo)
            } else { 
                P(mutex);           // Recibir y procesar pedido
                pop(C, (aux, sec));
                V(mutex);
                resultados[aux] = resolver(sec);
                V(espera[aux]);
            }
        }
    }
}

process Reloj
{
    while (true)
    { 
        P(inicio);
        delay(5 hs);
        finTiempo = true;
        V(pedidos);
    }
}