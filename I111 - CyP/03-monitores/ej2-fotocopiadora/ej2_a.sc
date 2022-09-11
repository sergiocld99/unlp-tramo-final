/* Implemente una solución suponiendo que existe una única fotocopiadora compartida 
por todas las personas, y las mismas la deben usar de a una persona a la vez, sin 
importar el orden. Existe una función Fotocopiar() que simula el uso de la fotocopiadora. 
Sólo se deben usar los procesos que representan a las Personas (y los monitores que sean 
necesarios). */

Monitor Fotocopiadora
{
    Procedure Usar()
    {
        Fotocopiar();
    }
}

Process Persona[id: 0..N-1]
{
    // Utilizar sin importar el orden de llegada
    Fotocopiadora.Usar();
}