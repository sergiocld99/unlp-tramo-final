/* En una playa hay 5 equipos de 4 personas cada uno (en total son 20 personas donde cada
una conoce previamente a que equipo pertenece). Cuando las personas van llegando
esperan con los de su equipo hasta que el mismo esté completo (hayan llegado los 4
integrantes), a partir de ese momento el equipo comienza a jugar. El juego consiste en que
cada integrante del grupo junta 15 monedas de a una en una playa (las monedas pueden ser
de 1, 2 o 5 pesos) y se suman los montos de las 60 monedas conseguidas en el grupo. Al
finalizar cada persona debe conocer grupo que más dinero junto. Nota: maximizar la
concurrencia. Suponga que para simular la búsqueda de una moneda por parte de una
persona existe una función Moneda() que retorna el valor de la moneda encontrada */

procedure Playa1 is

    task type Persona is
        ENTRY ConocerGanador(nroE : in integer);
    End Persona;

    task type Equipo is
        ENTRY Identificar(id : in integer);
        ENTRY Llegada;
        ENTRY Terminar(monto : in integer);
    End Equipo;

    task Juego is
        ENTRY Terminar(monto : in integer, nroE : in integer);
    End Juego;

    arrP : array(1..20) of Persona;
    arrE : array(1..4) of Equipo;

    task body Persona is
        nroEquipo : integer := ...;
        ganador, suma : integer := 0;
    begin
        arrE(nroEquipo).Llegada();
    
        for i in 1..15 loop
            suma := suma + Moneda();
        end loop;

        arrE(nroEquipo).Terminar(suma);
    
        accept ConocerGanador(nroE : in integer) do 
            ganador := nroE;
        End ConocerGanador;
    end Persona;

    task body Equipo is
        nro, suma : integer := 0;
    begin
        accept Identificar(id : in integer) do
            nro := id;
        End Identificar;

        accept Llegada() do
            accept Llegada() do
                accept Llegada() do
                    accept Llegada(); End Llegada;
                End Llegada;
            End Llegada;
        End Llegada;

        for i in 1..4 loop
            accept Terminar(monto : in integer) do
                suma := suma + monto;
            End Terminar;
        end loop

        Juego.Terminar(suma, nro);
    end Equipo;

    task body Juego is
        equipoMax, montoMax : integer := 0;
    begin
        for i in 1..4 loop
            accept Terminar(monto : in integer, nroE : in integer) do
                if (monto > montoMax) then
                    montoMax := monto;
                    equipoMax := nroE;
                end if;
            End Terminar;
        end loop

        for i in 1..20 loop
            arrP(i).ConocerGanador(equipoMax);
        end loop
    end Juego;

begin
    for i in 1..4 loop
        arrE(i).Identificar(i);
    end loop
end Playa1;