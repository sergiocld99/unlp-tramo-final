#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// macros para evitar tocar el programa C++
#define CANT_TAREAS 3
#define PRIMERA_TAREA 3
#define EJECUCIONES_SEGUIDAS(num) (num == 3 ? 3 : 1)
#define TAREA_SIGUIENTE(num) (num == 1 ? 2 : num == 2 ? 3 : 1)

// alias
#define CANT_EJECUCIONES(num) EJECUCIONES_SEGUIDAS(num)

// un semaforo para cada tarea
SemaphoreHandle_t semaforos[CANT_TAREAS];

// funcion común utilizada por cada tarea (difiere en parámetro)
void TareaGenerica(void *pvParameters){
  int impresiones = 0;
  int* param = (int*) pvParameters;
  int id = *param;
  int num = id+1;
  int siguiente = TAREA_SIGUIENTE(num);

  // si soy la tarea que debe arrancar primero, me autoincremento semaforo
  if (num == PRIMERA_TAREA){
    for (int i=0; i<EJECUCIONES_SEGUIDAS(num); i++) xSemaphoreGive(semaforos[id]);
  }

  while(true){
    if (xSemaphoreTake(semaforos[id], (TickType_t) 15) == pdTRUE){
      Serial.print("tarea ");
      Serial.println(num);
      impresiones++;

      // si ya imprimí los mensajes que debía...
      if (impresiones == EJECUCIONES_SEGUIDAS(num)) {
        // aumentar el semáforo de la siguiente tareas las veces que corresponda
        for (int i=0; i<EJECUCIONES_SEGUIDAS(siguiente); i++) xSemaphoreGive(semaforos[siguiente-1]);
        impresiones = 0;
      }
    }

    vTaskDelay( 100 / portTICK_PERIOD_MS );
  }
}

void TaskMillis(void *pvParameters){
  while(true){
    Serial.println();
    Serial.println(millis());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// -----------------------------------

void setup() {
  // variables
  int* ids = (int*) malloc(sizeof(int) * CANT_TAREAS);
  
  // configuro uart a 9600 baudios
  Serial.begin(9600);

  // Creamos los semáforos de tareas, inicializados en cero
  // tambien creamos los indices (parámetros)
  for (int i=0; i<CANT_TAREAS; i++){
    semaforos[i] = xSemaphoreCreateCounting(EJECUCIONES_SEGUIDAS(i+1), 0);
    ids[i] = i;
  }

  // creacion de tareas (el orden de creación NO determinará la ejecución)
  xTaskCreate(TareaGenerica, "tarea 1", 128, &ids[0], 1, NULL);
  xTaskCreate(TareaGenerica, "tarea 2", 128, &ids[1], 1, NULL);
  xTaskCreate(TareaGenerica, "tarea 3", 128, &ids[2], 1, NULL);
}

void loop() {
 
}
