# Sistemas de Tiempo Real

## FreeRTOS: Semáforos

- SemaphoreHandle_t xSerialSemaphore = xSemaphoreCreateMutex();
- xSemaphoreGive( xSerialSemaphore): equivalente al V(sem)
- xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ): equivalente al P(sem), excepto que tiene una espera máxima de 5 ticks (en este ejemplo)