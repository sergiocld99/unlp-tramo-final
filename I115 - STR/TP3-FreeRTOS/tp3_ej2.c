#include <Arduino_FreeRTOS.h>

void TaskSTR1(void *pvParameters){
  while(true){
    Serial.println("task 1");
    vTaskDelay( 4000 / portTICK_PERIOD_MS );
  }
}

void TaskSTR2(void *pvParameters){
  while(true){
    Serial.println("task 2");
    vTaskDelay( 2000 / portTICK_PERIOD_MS );
  }
}

void TaskSTR3(void *pvParameters){
  while(true){
    Serial.println("task 3");
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}

void TaskMillis(void *pvParameters){
  while(true){
    Serial.println(millis());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // creacion de tareas
  xTaskCreate(TaskSTR1, "Tarea 1", 128, NULL, 1, NULL);
  xTaskCreate(TaskSTR2, "Tarea 2", 128, NULL, 1, NULL);
  xTaskCreate(TaskSTR3, "Tarea 3", 128, NULL, 1, NULL);
  xTaskCreate(TaskMillis, "millis", 128, NULL, 2, NULL);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
