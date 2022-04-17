/* Suponga una comisión con 50 alumnos. Cuando los alumnos llegan forman una fila, una 
vez que están los 50 en la fila el jefe de trabajos prácticos les entrega el número de grupo 
(número aleatorio del 1 al 25) de tal manera que dos alumnos tendrán el mismo número de 
grupo (suponga que el jefe posee una función DarNumero() que devuelve en forma aleatoria 
un número del 1 al 25, el jefe de trabajos prácticos no guarda el número que le asigna a 
cada alumno). Cuando un alumno ha recibido su número de grupo comienza a realizar la 
práctica. Al terminar de trabajar, el alumno le avisa al jefe de trabajos prácticos y espera la 
nota. El jefe de trabajos prácticos, cuando han llegado los dos alumnos de un grupo les 
devuelve a ambos la nota del GRUPO (el primer grupo en terminar tendrá como nota 25, el 
segundo 24, y así sucesivamente hasta el último que tendrá nota 1) */

Monitor Comision
{
    int faltanLlegar = 50, asignados[50], notas[25];
    Cond barrera, solicitud, espera_alu[50], espera_grupo[25];
    Cola c;

    Procedure Llegada(idAlum: in int, G: out int)
    {
        faltanLlegar--;
        if (faltanLlegar = 0) signal(barrera);
        
        wait(espera_alu[idAlum]);           // Esperar por num de grupo
        G = asignados[idAlum];
    }

    Procedure AsignarGrupos()
    {
        if (faltanLlegar > 0) wait(barrera);
        
        for i = 0..49
        {
            asignados[i] = DarNumero();     // 2 alumnos tendrán el mismo número (1 al 25)
            signal(espera_alu[id]);
        }
    }

    Procedure Entregar(G: in int, N: out int)
    {
        push(c, G);                         // Encolo numero de grupo
        signal(solicitud);                  // Por si JTP está dormido...

        wait(espera_grupo[G-1]);            // Dormir hasta corrección
        N = notas[G-1];                     // Recibir nota
    }

    Procedure RecibirTrabajo(G: out int)
    {
        if (empty(c)) wait(solicitud);      // Dormir si no hay correcciones pendientes
        pop(c, G);
    }

    Procedure Calificar(G, N: in int)
    {
        notas[G-1] = N;                     // Publicar nota
        signal_all(espera_grupo[G-1]);      // Despertar a los 2 del grupo
    }
}

Process Alumno[id: 0..49]
{
    int num_grupo, nota;

    Comision.Llegada(id, num_grupo);        // Mando id, recibo num de grupo
    resolverPractica();
    Comision.Entregar(num_grupo, nota);     // Mando num de grupo, recibo nota
}

Process JTP
{
    int faltanEntregar = 25;                // Coincide con nota según orden
    int entregasGrupo[25] = ([25] 0);       // De 0 a 2 por grupo
    int num_grupo;                          // Actual

    Comision.AsignarGrupos();
    
    while(faltanEntregar > 0)
    {
        Comision.RecibirTrabajo(num_grupo);
        corregirTrabajo();

        if (entregasGrupo[num_grupo-1] = 2)
        {
            Comision.Calificar(num_grupo, faltanEntregar);
            faltanEntregar--;
        }
    }
}