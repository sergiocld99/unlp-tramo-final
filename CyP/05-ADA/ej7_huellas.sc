/* Hay un sistema de reconocimiento de huellas dactilares de la policía que tiene 8 Servidores
para realizar el reconocimiento, cada uno de ellos trabajando con una Base de Datos propia;
a su vez hay un Especialista que utiliza indefinidamente. El sistema funciona de la
siguiente manera: el Especialista toma una imagen de una huella (TEST) y se la envía a los
servidores para que cada uno de ellos le devuelva el código y el valor de similitud de la
huella que más se asemeja a TEST en su BD; al final del procesamiento, el especialista debe
conocer el código de la huella con mayor valor de similitud entre las devueltas por los 8
servidores. Cuando ha terminado de procesar una huella comienza nuevamente todo el
ciclo. Nota: suponga que existe una función Buscar(test, código, valor) que utiliza cada Servidor
donde recibe como parámetro de entrada la huella test, y devuelve como parámetros de
salida el código y el valor de similitud de la huella más parecida a test en la BD correspondiente.
Maximizar la concurrencia y no generar demora innecesaria */

procedure Policia1 is

    task type Servidor is
        ENTRY Recibir(H : in texto);
    End Servidor;

    task Especialista is
        ENTRY Resultado(cod : in integer, val : in integer);
    End Especialista;

    arrS : array(1..8) of Servidor;

    task body Servidor is
        test : texto;
        codigo, valor : integer;
    begin
        loop
            accept Recibir(H : in texto) do
                test := H;
            End Recibir;

            Buscar(test, codigo, valor);
            Especialista.Resultado(codigo, valor);
        end loop
    end Servidor;

    task body Especialista is
        test : texto;
        mejorCodigo, mejorValor : integer := 0;
    begin
        loop
            test := generarHuella();

            for i in 1..8 loop
                arrS(i).Recibir(test);
            end loop

            for i in 1..8 loop
                accept Resultado(cod : in integer, val : in integer) do
                    if (val > mejorValor) then
                        mejorValor := val;
                        mejorCodigo := cod;
                    end if;
                End Resultado;
            end loop
        end loop
    end Especialista;

begin
    null;
end Policia1;