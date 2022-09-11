# Notitas

## Demora innecesaria
- Cuando un proceso solo necesita mandar mensajes de manera rápida (sin espera de respuesta), conviene usar un proceso intermediario (Admin) que los vaya recibiendo rápido para almacenarlos en una cola, y luego éste le va pasando de a uno al proceso que debe procesarlo cuando éste se lo solicite (pedido al estar libre)