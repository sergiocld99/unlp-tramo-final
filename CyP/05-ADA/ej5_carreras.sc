/* En un sistema para acreditar carreras universitarias, hay UN Servidor que atiende pedidos
de U Usuarios de a uno a la vez y de acuerdo al orden en que se hacen los pedidos. Cada
usuario trabaja en el documento a presentar, y luego lo envía al servidor; espera la respuesta
del mismo que le indica si está todo bien o hay algún error. Mientras haya algún error vuelve
a trabajar con el documento y a enviarlo al servidor. Cuando el servidor le responde que
está todo bien el usuario se retira. Cuando un usuario envía un pedido espera a lo sumo 2
minutos a que sea recibido por el servidor, pasado ese tiempo espera un minuto y vuelve a
intentarlo (usando el mismo documento). */

procedure Carreras1 is

    task type Usuario;

    task Servidor is
        ENTRY Pedido(D : in texto, error : out bool);
    end Servidor;

    arrU : array(1..U) of Usuario;

    task body Usuario is
        doc : texto;
        error, listo : bool := false;
    begin
        doc := prepararDocumento();

        while (not listo) loop
            SELECT
                Servidor.Pedido(doc, error);
                if (not error) then
                    listo := true;
                end if;
            OR DELAY(120)
                delay(60);
            END SELECT
        end loop
    end Usuario;

    task body Servidor is
    begin
        loop
            accept Pedido(D : in texto, error : out bool) do
                error := analizar(D);
            End Pedido;
        end loop
    end Servidor;

begin
    null;
end Carreras1;