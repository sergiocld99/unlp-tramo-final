/* Una empresa de limpieza se encarga de recolectar residuos en una ciudad por medio de 3
camiones. Hay P personas que hacen continuos reclamos hasta que uno de los camiones
pase por su casa. Cada persona hace un reclamo, espera a lo sumo 15 minutos a que llegue
un camión y si no vuelve a hacer el reclamo y a esperar a lo sumo 15 minutos a que llegue
un camión y así sucesivamente hasta que el camión llegue y recolecte los residuos; en ese
momento deja de hacer reclamos y se va. Cuando un camión está libre la empresa lo envía a
la casa de la persona que más reclamos ha hecho sin ser atendido. Nota: maximizar la
concurrencia. */

procedure Recoleccion1 is

    task type Camion;

    task type Persona is
        ENTRY Identificar(id : in integer);
        ENTRY Atendido;
    end Persona;

    task Empresa is
        ENTRY Siguiente(idP : out integer);
        ENTRY Reclamo(idP : in integer);
    end Empresa;

    arrC : array(1..3) of Camion;
    arrP : array(1..P) of Persona;

    task body Camion is
        idP : integer;
    begin
        loop
            Empresa.Siguiente(idP);
            arrP(idP).Atendido();
        end loop
    end Camion;

    task body Persona is
        listo : bool := false;
        nro : integer;
    begin
        accept Identificar(id : in integer) do
            nro := id;
        End Identificar;

        while (not listo) loop
            Empresa.Reclamo(nro);

            SELECT
                accept Atendido(); End Atendido;
                listo := true;
            OR DELAY(15*60)
                null;
            END SELECT
        end loop
    end Persona;

    task body Empresa is
        cantR : array(1..P) of integer;
        total : integer := 0;
    begin
        loop
            SELECT
                accept Reclamo(idP : in integer) do
                    cantR(idP) := cantR(idP) + 1;
                End Reclamo;
                total := total + 1;
            OR
                when(total > 0) => accept Siguiente(idP : out integer) do
                    idP := maximo(cantR);
                    total := total - cantR(idP);
                    cantR(idP) := 0;
                End Siguiente;
            END SELECT
        end loop
    end Empresa;

begin
    for i in 1..P loop
        arrP(i).Identificar(i);
    end loop;
end Recoleccion1;