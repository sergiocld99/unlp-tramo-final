procedure Clinica1 is

    task type Enfermera;
    task type Persona;

    task Medico is
        ENTRY AtiendeEnfermera;
        ENTRY AtiendePaciente;
    end Medico;

    task Consultorio is
        ENTRY Dejar(N : in texto);
        ENTRY Sacar(N : out texto);
    end Consultorio;

    arrE : array(1..E) of Enfermera;
    arrP : array(1..P) of Persona;

    task body Persona is
        intentos : integer := 0;
        atendido : bool := false;
    begin
        while (not atendido && intentos < 3) loop
            SELECT
                Medico.AtiendePaciente();
                atendido := true;
            OR DELAY 300
                delay(600); intentos++;
            END SELECT
        end loop
    end Persona;

    task body Enfermera is
        nota : texto;
    begin
        loop
            SELECT
                Medico.AtiendeEnfermera();
            ELSE
                nota := hacerNota();
                Consultorio.Dejar(nota);
            END SELECT
        end loop
    end Enfermera;

    task body Medico is
        nota : texto;
    begin
        loop
            SELECT
                accept AtiendePaciente();
            OR
                when(AtiendePaciente.count == 0) => accept AtiendeEnfermera();
            ELSE
                SELECT
                    Consultorio.Sacar(nota);
                ELSE
                    null;
                END SELECT
            END SELECT
        end loop
    end Medico;

    task body Consultorio is
        notas : cola;
    begin
        loop
            SELECT
                accept Dejar(N : in texto) do 
                    push(notas, N); 
                End Dejar;
            OR
                when(not empty(notas)) => accept Sacar(N : out texto) do 
                    pop(notas, N);
                End Sacar;
            END SELECT
        end loop
    end Consultorio;

begin
    null;
end Clinica1;