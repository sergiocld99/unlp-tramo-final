/* Modifique la solución de (b) para el caso en que además haya un Empleado que le indica 
a cada persona cuando debe usar la fotocopiadora. */

Monitor Fotocopiadora
{
    int esperando = 0;
    Cond solicitud, terminado, espera;

    Procedure Solicitar()
    {
        esperando++;
        signal(solicitud);      // aviso por si empleado está dormido
        wait(espera);
    }

    Procedure Liberar()
    {
        signal(terminado);      // despertar a empleado para que llame al siguiente
    }

    Procedure Atender()
    {
        // Se queda dormido sólo si no había nadie esperando para imprimir
        // La primera persona que llegue tendrá que despertarlo para que siga su labor

        if (esperando = 0) wait(solicitud);      // evito busy-waiting
        esperando--;
        signal(espera);
        wait(terminado);                        // sólo 1 persona fotocopia a la vez
    }
}

Process Persona[id: 0..N-1]
{
    Fotocopiadora.Solicitar();
    Fotocopiar();
    Fotocopiadora.Liberar();
}

Process Empleado
{
    while(true)
    {
        Fotocopiadora.Atender();
    }
}