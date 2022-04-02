/* Se debe simular una maratón con C corredores donde en la llegada hay UNA máquina
expendedora de agua con capacidad para 20 botellas. Además existe un repositor encargado
de reponer las botellas de la máquina. Cuando los C corredores han llegado al inicio comienza
la carrera. Cuando un corredor termina la carrera se dirigen a la máquina expendedora, espera
su turno (respetando el orden de llegada), saca una botella y se retira. Si encuentra la máquina
sin botellas, le avisa al repositor para que cargue nuevamente la máquina con 20 botellas; espera
a que se haga la recarga; saca una botella y se retira. Nota: maximizar la concurrencia; mientras
se reponen las botellas se debe permitir que otros corredores se encolen. */

sem barrera = 0;        // Semáforo de llegados a inicio
sem mutex = 1;          // Semáforo de SC para contador y cola/ocupado (no se mezclan)
sem solicitud = 0;      // Semáforo de aviso recibido por el repositor
sem recargado = 0;      // Semáforo de aviso emitido por el repositor

sem turno[N] = ([N] 0);     // Semáforo para cada corredor en la expendedora

int contador = 0;           // Contador de llegados a inicio
int botellas = 20;          // Contador de botellas disponibles
bool ocupado = false;       // Indicador de uso de la expendedora
cola C;                     // Cola para respetar orden de llegada a la expendedora

process Corredor[id: 0..C-1]
{
    int siguiente;

    P(mutex);                           // Esperar para modificar contador

    // SECCIÓN CRÍTICA 1
    contador = contador + 1;            // Incrementar atómicamente
    if (contador == C){
        for i = 1..C -> V(barrera);
    }

    V(mutex);                           // Liberar uso de contador
    P(barrera);                         // Esperar largada (todos llegaron a Inicio)

    // Correr maratón...
    // Llegar a la línea de meta...
    // Dirigirse a la expendedora...

    P(mutex);                           // Esperar para modificar cola y/o "ocupado"

    // SECCIÓN CRÍTICA 2
    if (ocupado){                       // Si alguien está usando la máquina...
        push(C, id);                    // Encolarse atómicamente
        V(mutex);                       // Liberar SC
        P(turno[id]);                   // Aguardar autorización de uso de expendedora
    } else {
        ocupado = true;                 // Todos los que lleguen después deberán encolarse
        V(mutex);                       // Liberar SC
    }

    // Estoy seguro que sólo hay 1 corredor ejecutando esta porción de código
    // Los demás se encuentran en carrera, ó agregándose a la cola, ó esperando su turno.

    if (botellas == 0){
        V(solicitud);           // Avisar al repositor
        P(recargado);           // Esperar recarga hecha (20 botellas)
    }

    botellas = botellas - 1;    // Decrementar (siempre botellas es mayor que 0)

    P(mutex);                   // Esperar para modificar cola y/o "ocupado"

    // SECCIÓN CRÍTICA 3
    if (not empty(C)){          // Si hay alguien esperando...
        pop(C, siguiente);      // Veo quién es el primero de la fila
        V(turno[siguiente]);    // Habilito su turno (y dejo "ocupado" en true)
    } else {
        ocupado = false;        // Liberar expendedora para el primero que llegue después
    }

    V(mutex);                   // Liberar uso de cola y/o "ocupado"
    
}

process Repositor
{
    while(true)
    {
        P(solicitud);           // Esperar que un corredor solicite botellas
        botellas = 20;          // De 0 a 20 botellas (sólo yo estoy modificando esto)
        V(recargado);           // Avisar al corredor que ya puede usar la máquina.
    }
}