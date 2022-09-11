/* Resolver con PMS (Pasaje de Mensajes SINCRÓNICOS) el siguiente problema. En un
examen final hay P alumnos y 3 profesores. Cuando todos los alumnos han llegado
comienza el examen. Cada alumno resuelve su examen, lo entrega y espera a que alguno de
los profesores lo corrija y le indique la nota. Los profesores corrigen los exámenes
respectando el orden en que los alumnos van entregando. Nota: maximizar la concurrencia
y no generar demora innecesaria. */

Process Alumno[id: 0..P-1]
{
    int nota;

    Admin!llegada();        // avisar que llegué
    Admin?inicio();         // esperar por inicio de examen

    // resolver examen...

    Admin!entrega(id);                  // entregar y esperar corrección -> enviar id
    Profesor[*]?correccion(nota);       // me guardo mi nota (cualquier profe me corrige -> comodín)
}

Process Admin
{
    Cola examenes;
    int aux;

    for i = 1..P -> Alumno[*]?llegada();    // recibir de cualquier alumno hasta que estén todos
    for i = 0..P-1 -> Alumno[i]!inicio();   // notificar comienzo de examen a todos los alumnos

    do Alumno[*]?entrega(aux) -> push(examenes, aux);
    [] not empty(examenes); Profesor[*]?pedido(aux) -> Profesor[aux]!entrega(pop(examenes));
    od
}

Process Profesor[id: 0..2]
{
    int idAlumno, nota;

    while(true)
    {
        Admin!pedido(id);           // estoy libre -> solicitar, somos varios -> enviar id
        Admin?entrega(idAlumno);    // recibir entrega de idAlumno

        nota = corregirExamen();

        Alumno[idAlumno]!correccion(nota);
    }
}