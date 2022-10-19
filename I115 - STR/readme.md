# Sistemas de Tiempo Real

## FreeRTOS: Semáforos

- SemaphoreHandle_t xSerialSemaphore = xSemaphoreCreateMutex();
- xSemaphoreGive( xSerialSemaphore): equivalente al V(sem)
- xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ): equivalente al P(sem), excepto que tiene una espera máxima de 5 ticks (en este ejemplo)


## Avance I
☑️  Creados los andenes, vias, puente rudimentario

🧑🏽  Pasajeros: tienen 3 estados (vagando, cruzando puente y por tomar tren)

🚈  Trenes: tienen 3 estados (avanzando, detenido, lejos del anden)

❌  Problema a resolver: investigar sentencia neighbors, ver si permite evitar que dos pasajeros se encuentren en un mismo lugar (pixel)

![avance](/I115%20-%20STR/NetLogo/avance09-30.png)
