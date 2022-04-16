/* En una herrería hay 15 empleados que forman 5 grupos de 3 personas; los grupos se forman 
de acuerdo al orden de llegada (los 3 primeros pertenecen al grupo 1, los 3 siguientes al grupo 
2, y así sucesivamente). Ni bien conoce el grupo al que pertenece el empleado comienza a 
trabajar (no debe esperar al resto de grupo para comenzar). Cada grupo debe hacer 
exactamente P unidades de un producto (cada unidad es hecha por un único empleado). Al 
terminar de hacer las P unidades de un grupo, sus 3 empleados se retiran. Nota: maximizar 
la concurrencia; ningún grupo puede hacer unidades de más */

// La asignación de grupo es una sección crítica (1 empleado a la vez)
// Los empleados se encolan para respetar orden de llegada mientras se asignan grupos

Cola c;
Bool ocupado = false;               // Indicador de asignación de grupo en proceso
int num_grupo = 0;                  // Asignador de grupo para próximo empleado (entre 0 y 4)
int restantes_act = 3;              // Limitador de empleados asignados en num_grupo
int restaProducir[5] = ([5] P);
int restaTerminar[5] = ([5] P);

sem mutex = 0;                      // Semáforo de uso atómico de cola y "ocupado"
sem mutexGrupo[5] = ([5] 0);        // Semáforo de uso atómico de realizados por grupo
sem retirarse[5] = ([5] 0);         // Semáforo de cada grupo para retirarse todos al final

process Empleado[id: 0..14]
{
    int mi_grupo;
    int siguiente;

    // PARTE 1: ASIGNACIÓN DE GRUPOS POR ORDEN DE LLEGADA
    P(mutex);    
    if (ocupado){
        push(C,id);                 // Encolar mi identificador
        V(mutex);                   // Permitir que más empleados se encolen
        P(turno[id]);               // Aguardar turno para poder tener mi grupo
    } else {
        ocupado = true;             // Asignación de grupo en proceso
        V(mutex);                   // Permitir que más empleados se encolen
    }

    mi_grupo = num_grupo;           // Guardar mi número de grupo localmente
    restantes_act = restantes_act - 1;

    if (restantes_act = 0) {
        num_grupo = num_grupo + 1;  // Incrementar N° grupo (hasta 5 -> no hay empleados de sobra)
        restantes_act = 3;          // Dejar espacio para 3 miembros al próximo grupo
    }

    P(mutex);
    if (not empty(c))
    {
        pop(c, siguiente);
        V(turno[siguiente]);        // Despertar al siguiente en la fila (ocupado sigue en true)
    } else {
        ocupado = false;            // El próximo que llegue se autoasigna inmediatamente
    }
    V(mutex);

    // PARTE 2: PRODUCCIÓN DE UNIDADES HASTA ALCANZAR LIMITE SOLICITADO
    // No debo esperar a que el grupo esté completo para poder trabajar
    P(mutexGrupo[mi_grupo])         // Para consultar restantes, necesito SC de grupo

    while (restaProducir[mi_grupo] > 0)    // No quiero producir de más
    { 
        restaProducir[mi_grupo] = restaProducir[mi_grupo] - 1;
        V(mutexGrupo[mi_grupo]);           // Libero variables de grupo mientras laburo

        realizarTrabajo();                 // Puede no durar lo mismo en cada ejecución

        P(mutexGrupo[mi_grupo]);           // Sección crítica de grupo
        restaTerminar[mi_grupo] = restaTerminar[mi_grupo] - 1;
        if (restaTerminar[mi_grupo] == 0)
        {
            // Habilitar semáforo 3x, incluso si alguno de los miembros nunca llegó
            for i = 1..3 -> V(retirarse[mi_grupo]);
            V(mutexGrupo[mi_grupo]);       // No olvidar liberar SC de grupo
        }
    }

    P(retirarse[mi_grupo]);         // No me retiro hasta se hayan terminado las P unidades

    /* Si a un empleado se le asigna grupo después de haberse empezado a producir las P unidades, 
    sólo se consultará cuántas faltan (0) y esperará a que se terminen para retirarse */

    /* Si a un empleado se le asigna grupo después de haberse TERMINADO las P unidades,
    sólo se consultará cuántas faltan (0) y se retirará inmediatamente (semáforo habilitado) */
}