procedure Sistema1 is

    task Central is
        ENTRY Señal1(D : in texto);
        ENTRY Señal2(D : in texto);
        ENTRY Temporizador;
    end Central;

    task Reloj is
        ENTRY Empezar;
    end Reloj;

    task body Proceso1 is
        datos : texto;
    begin
        loop
            datos := generarSeñal();
            SELECT
                Central.Señal1(datos);
            OR DELAY 120
                null;
            END SELECT
        end loop
    end Proceso1;

    task body Proceso2 is
        datos : texto;
    begin
        datos := generarSeñal();
        loop
            SELECT
                Central.Señal2(datos);
                datos := generarSeñal();
            ELSE
                delay(60);
            END SELECT
        end loop
    end Proceso2;

    task body Central is
        datos : texto;
        timeout : bool := false;
    begin
        accept Señal1(D : in texto) do 
            datos := D;
        End Señal1;

        loop
            SELECT
                accept Señal1(D : in texto) do 
                    datos := D; 
                End Señal1;
            OR
                accept Señal2(D : in texto) do 
                    datos := D;
                End Señal2;
                
                Reloj.Empezar();
                timeout := false;
                
                while (not timeout) loop
                    SELECT
                        when(Temporizador.count == 0) => accept Señal2(D : in texto) do 
                            datos := D;
                        End Señal2;
                    OR
                        accept Temporizador() do 
                            timeout := true;
                        End Temporizador;
                    END SELECT
                end loop
            END SELECT
        end loop
    end Central;

    task body Reloj is
    begin
        loop
            accept Empezar();
            delay(180);
            Central.Temporizador();
        end loop
    end Reloj;

begin
    null;
end Sistema1;