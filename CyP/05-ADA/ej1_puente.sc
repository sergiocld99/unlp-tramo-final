procedure Puente1 is
    
    task type Auto;
    task type Camioneta;
    task type Camion;

    task Admin is
        ENTRY Salida(peso : in integer);
        ENTRY PasoAuto;
        ENTRY PasoCamioneta;
        ENTRY PasoCamion;
    end Admin;

    arrA : array(1..A) of Auto;
    arrB : array(1..B) of Camioneta;
    arrC : array(1..C) of Camion;

    task body Admin is
        total : integer := 0;
    begin
        loop
            SELECT
                accept Salida(peso : in integer) do 
                    total := total - peso; 
                End Salida;
            OR
                when(total <= 4) => accept PasoAuto() do 
                    total := total + 1; 
                End PasoAuto;
            OR
                when(total <= 3) => accept PasoCamioneta() do 
                    total := total + 2; 
                End PasoCamioneta;
            OR
                when(total <= 2) => accept PasoCamion() do 
                    total := total + 3; 
                End PasoCamion;
            END SELECT
        end loop
    end Admin;

    task body Auto is
    begin
        Admin.PasoAuto();
        Admin.Salida(1);
    end Auto;

    task body Camioneta is
    begin
        Admin.PasoCamioneta();
        Admin.Salida(2);
    end Camioneta;

    task body Camion is
    begin
        Admin.PasoCamion();
        Admin.Salida(3);
    end Camion;

begin
    null;
end Puente1;