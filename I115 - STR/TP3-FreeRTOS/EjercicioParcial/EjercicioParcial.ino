// Bibliotecas
#include "Arduino_FreeRTOS.h"
#include <semphr.h>

// Prototipos de tareas
void tarea1(void* arg);
void tarea2(void* arg);

// Variable compartida
int varCompartida = 0;

// Variable mutex
SemaphoreHandle_t mutex;


void setup() {
  Serial.begin(9600);
  mutex = xSemaphoreCreateMutex();
  
  xTaskCreate(tarea1, "tarea 1", 128, NULL, 200, NULL);
  xTaskCreate(tarea2, "tarea 2", 128, NULL, 100, NULL);
  xTaskCreate(tarea2, "tarea 3", 128, NULL, 100, NULL);
}

void loop(){
  
}

// Implementación de tareas
void tarea1(void* arg){

  while(1){
    if (xSemaphoreTake(mutex, 15) == pdTRUE){
      varCompartida++;
      xSemaphoreGive(mutex);

      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void tarea2(void* arg){

  while(1){
    Serial.println(varCompartida);
    
    while(xSemaphoreTake(mutex, 15) == pdFALSE);
    varCompartida += 2;
    xSemaphoreGive(mutex);
    
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
