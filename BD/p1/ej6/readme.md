# Enunciado

Se debe modelar la información necesaria para la gestión de huéspedes y personal de una
conocida cadena hotelera. De cada hotel de la cadena se registra: código único de hotel,
dirección detallada, ciudad, país, cantidad de estrellas del hotel y los servicios que ofrece. De
cada servicio se registra nombre único, descripción y un costo asociado por día. Además se
debe registrar información respecto de las habitaciones del hotel y una descripción de los
restaurantes que posee.

De las habitaciones se registra un número de habitación que es relativo al piso del hotel donde
se encuentra (el mismo número de habitación se repite en los diferentes pisos del hotel), piso,
ubicación, descripción y tipo de habitación.

De los clientes/huéspedes se registra: pasaporte, apellido, nombre, teléfono de contacto, uno o
varios emails y dirección detallada. El cliente puede reservar hospedaje telefónicamente o por
la web de la cadena hotelera. Cuando un cliente reserva se debe dejar constancia de: tipo/tipos
de habitaciones que reserva, fecha de inicio y fin prevista de la estadía, cantidad de huéspedes
mayores, cantidad de huéspedes menores, valor total de la reserva y además se debe registrar
el pago de la seña. El pago se puede hacer con tarjeta de débito o crédito, y deberá quedar
constancia de los datos completos de la tarjeta con la que abonó y, de ser necesario, cuantas
cuotas utilizó.

Cuando el cliente se registra en el hotel para efectivizar la reserva se debe solicitar toda la
documentación del resto de los huéspedes. Para ello se registra una estadía que cuenta con la
siguiente información: habitación/es ocupadas, fecha de ingreso, fecha de salida y, por cada
habitación, los huéspedes que se alojan. Además se debe registrar forma de pago del saldo a
abonar por las habitaciones y servicios que contrate durante la estadía.

De los empleados se registra la misma información que para los huéspedes y además, número
único de empleado, fecha de ingreso, cantidad de hijos, nacionalidad y el área del hotel en
donde trabaja. Además, los empleados van rotando de área, y debe quedar registro de esta
información. De las áreas de cada hotel se almacena: nombre único y descripción.
