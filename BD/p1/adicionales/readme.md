# Ejercicios Adicionales

Pasar modelos conceptuales ya realizados a sus correspondientes modelos lógicos y físicos


# Tips Conceptual a Lógico

* No puede haber atributos compuestos (como domicilio detallado) -> mover directamente sus "sub-atributos" a la entidad.
* No puede haber atributos **polivalentes** -> agregar como relación de correspondencia (0,N) o (1,N) según corresponda a otra entidad.
* No se permite la herencia (jerarquías) -> decidir si eliminar padre, hijos o agregar relaciones con correspondencia (0,1) para cada tipo.

Errores comunes:

* Confundir atributos opcionales (0,1) con polivalentes (0,N)
* Dejar entidades sin identificar, por ej, al eliminar jerarquias

Ayuditas:

* Jerarquia T,E: puedo hacer 3 cosas, dejar todas las entidades, dejar sólo al padre, o bien, dejar sólo las entidades hijas. Si opto por dejar todo, agregar las relaciones (0,1) de "es un", y conectar también el identificador externo del padre a los hijos.

# Tips Logico a Fisico

* Al menos, cada entidad se convertirá en una tabla, excepto si entre 2 hay una relación (1,1) a (1,1). Con las relaciones veremos su cardinalidad.
* Las relaciones "N a N" **siempre** se convierten a una tabla usando identificadores de ambas entidades y atributos del "rombo".
* Los atributos opcionales se marcan con un signo de pregunta al final

Errores comunes:

 * Al usar más de 1 identicador de otra entidad, poner (fk) en cada atributo en lugar de "(atributo1, atributo2) (fk)".
