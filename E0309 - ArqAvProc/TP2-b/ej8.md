# Enunciado
Algunas arquitecturas RISC requieren que el compilador (o el programador en assembler) garantice que no se acceda a un registro durante un número determinado de ciclos después de cargarlo desde la memoria. Dé una ventaja y 
una desventaja de esta elección de diseño.

## Ventaja
Se reduce la complejidad del hardware, ya que al garantizar que el operando no será leído antes de completarse su carga entonces no resulta necesario un reordenamiento en tiempo de ejecución.

Entonces hay una simplificación de la <b>lógica del pipeline</b>, lo que deriva en una reducción del consumo de energía y área en chip.

## Desventaja
Aumenta la complejidad del compilador, ya que está obligado a insertar instrucciones NOP si no encuentra instrucciones útiles.

Además, esto <b>reduce las posibilidades de innovar en la implementación del procesador</b>.