/* Se tiene un curso con 40 alumnos, la maestra entrega una tarea distinta a cada alumno, luego
cada alumno realiza su tarea y se la entrega a la maestra para que la corrija, esta revisa la tarea
y si está bien le avisa al alumno que puede irse, si la tarea está mal le indica los errores, el
alumno corregirá esos errores y volverá a entregarle la tarea a la maestra para que realice la
corrección nuevamente, esto se repite hasta que la tarea no tenga errores. */

// Suposición: los 40 alumnos ya se encuentran en el aula

// Variables globales
sem mutex = 1;
sem barrera = 0;
sem entregas = 0;
sem continuar = 0;
sem notif[40] = ([40] 0);
bool perfectos[40] = ([40] false);
tarea trabajos[40];
cola C;

// Procesos
process Alumno[id: 0..39]
{
    // Variables locales al proceso
    bool termine = false;
    tarea tar;

    P(barrera);                 // Esperar arranque
    tar = trabajos[id];         // Obtener mi tarea

    while(not termine)
    {
        resolver(tar);          // Hacer o corregir tarea

        P(mutex);               // Esperar que se libere la SC
        push(C, id);            // Entregar tarea
        V(mutex);               // Liberar SC

        V(entregas);            // Avisar a Maestra
        P(notif[id]);           // Esperar llamado
        termine = perfectos[id];

        V(confirmar);           // Permitir que la maestra siga corrigiendo
    }

    // Se retira del salón
}

process Maestra
{
    // Variables locales al proceso
    bool ok;
    tarea tar;
    int idAlumno;

    // Asignar una tarea a cada alumno
    for i = 0..39 -> trabajos[i] = crearTarea();

    // Una vez creadas todas las tareas, permitir que resuelvan
    for i = 0..39 -> V(barrera);

    while(true)
    {
        P(entregas);                // Esperar tarea para corregir

        P(mutex);                   // Esperar que se libere SC
        pop(C, idAlumno);           // Identificar alumno
        V(mutex);                   // Liberar SC

        tar = trabajos[idAlumno];   // Obtener tarea
        ok = corregir(tar);         // Corregir tarea
        
        if(ok) perfectos[idAlumno] = true;     // Establecer como tarea terminada
        else {
            // Llamar al alumno
            // Indicarle los errores
            // Devolverle la tarea
        }

        V(notif[idAlumno]);         // Notificar al alumno
        P(continuar);               // Esperar confirmación de alumno
    }
    
}