/* Resolver con PMS (Pasaje de Mensajes SINCRÓNICOS) el siguiente problema. En una
exposición aeronáutica hay un simulador de vuelo (que debe ser usado con exclusión
mutua) y un empleado encargado de administrar el uso del mismo. Hay P personas que
esperan a que el empleado lo deje acceder al simulador, lo usa por un rato y se retira. El
empleado deja usar el simulador a las personas respetando el orden de llegada. Nota: cada
persona usa sólo una vez el simulador. */

Process Persona[id: 0..P-1]
{
    Empleado!pedido(id);        // solicitar y esperar respuesta -> enviar id
    Empleado?subir();

    // usar simulador por un rato...
    
    Empleado!retiro();
}

Process Empleado
{
    int idPersona;

    for i = 1..P -> {
        Persona[*]?pedido(idPersona);       // ESTO NO RESPETA EL ORDEN DE LLEGADA -> Admin
        Persona[idPersona]!subir();

        // idPersona usa el simulador con exclusión mutua

        Persona[idPersona]?retiro();
    }
}

