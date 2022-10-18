/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

// Inlcusiones

#include "app.h"         // <= Su propia cabecera
#include "sapi.h"        // <= Biblioteca sAPI
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "joystick.h"

#define TICKRATE_HZ (1000)

static volatile uint32_t tick_ct = 0;
static volatile uint8_t FLAG_UP = 0;
static volatile uint8_t FLAG_DOWN = 0;
static volatile uint8_t FLAG_LEFT = 0;
static volatile uint8_t FLAG_RIGHT = 0;
static volatile uint8_t FLAG_ENTER = 0;

// Prototipos de funciones privadas
static void encenderPares();
static void encenderImpares();
static void apagarPares();
static void apagarImpares();

// Auxiliares
static void delayBloqueante(uint32_t);

char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}

/*
void SysTick_Handler(void) {
   tick_ct++;
}
*/

/*
void delay(uint32_t tk) {
   uint32_t end = tick_ct + tk;
   while(tick_ct < end)
       __WFI();
}
*/

static void delayBloqueante(uint32_t ms){
   uint32_t end = tick_ct + ms;
   while (tick_ct < end) tick_ct++;
}

static void encenderPares(){
   Board_LED_Set(0, true);
   Board_LED_Set(2, true);
   Board_LED_Set(4, true);
   // gpioWrite(LED2, ON);
}

static void encenderImpares(){
   Board_LED_Set(1, true);
   Board_LED_Set(3, true);
   Board_LED_Set(5, true);
   //gpioWrite(LED1, ON);
   // gpioWrite(LED3, ON);
}

static void apagarPares(){
   Board_LED_Set(0, false);
   Board_LED_Set(2, false);
   Board_LED_Set(4, false);
   //gpioWrite(LED2, OFF);
}

static void apagarImpares(){
   Board_LED_Set(1, false);
   Board_LED_Set(3, false);
   Board_LED_Set(5, false);
   // gpioWrite(LED1, OFF);
   // gpioWrite(LED3, OFF);
}

static void apagarTodos(){
   apagarPares();
   apagarImpares();
}

void checkForPressedKeys( void* unused )
{
  
   if( !gpioRead( TEC1 ) || FLAG_UP ){
      usbDeviceKeyboardPress( USB_KEY_W ); // 'c' or 'C'
   }
   
   if( !gpioRead( TEC2) || FLAG_RIGHT ){
      usbDeviceKeyboardPress( USB_KEY_D ); // 'i' or 'I'
   }
   
   if( !gpioRead( TEC3 ) || FLAG_LEFT ){         
      usbDeviceKeyboardPress( USB_KEY_A ); // 'a' or 'A'
   }
   
   if( !gpioRead( TEC4 ) || FLAG_DOWN ){         
      usbDeviceKeyboardPress( USB_KEY_S ); // Enter
   }
   
   if (FLAG_ENTER) usbDeviceKeyboardPress( USB_KEY_ENTER );
}

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------
   SystemCoreClockUpdate();
   Board_Init();
   SysTick_Config(SystemCoreClock / TICKRATE_HZ);
   
   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   // Configuration routine for HID Keyboard example   
   usbDeviceConfig(USB_HID_KEYBOARD);   
   usbDeviceKeyboardCheckKeysCallbackSet( checkForPressedKeys );
   
   // Habilitar ADC
   adcConfig( ADC_ENABLE ); /* ADC */
   
   // Habilitar UART (DEBUG)
   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_USB, 115200 );
   
   // Establecer T_COL1 como entrada digital
   gpioInit( T_COL1, GPIO_INPUT );

   // ---------- REPETIR POR SIEMPRE --------------------------
   while (1) {
      /* Do Keyboard tasks */      
      // usbDeviceKeyboardTasks();
      
      // Leer entrada CH3 (eje X: el 0 está izquierda)
      uint16_t valorEjeX = adcRead( CH3 );
      
      // Leer entrada CH2 (eje Y: el 0 está arriba)
      uint16_t valorEjeY = adcRead( CH2 );
      
      // MANDAR VALOR DE EJE X a TERMINAL
      /*
      char textoX[5], textoY[5];
      itoa(valorEjeX, textoX, 10);
      itoa(valorEjeY, textoY, 10);
      uartWriteString( UART_USB, textoX );
      uartWriteString( UART_USB, ", ");
      uartWriteString( UART_USB, textoY );
      uartWriteString( UART_USB, "\r\n");
      delay(1000);
      */
      
      Joystick_Direccion dirs[2];
      
      Joystick_LeerDirs(valorEjeX, valorEjeY, dirs);
      apagarTodos();
      
      FLAG_UP = 0;
      FLAG_DOWN = 0;
      FLAG_LEFT = 0;
      FLAG_RIGHT = 0;
      FLAG_ENTER = 0;
      
      switch(dirs[0]){
         case NONE:
            break;
         case LEFT:
            Board_LED_Set(3, true);
            FLAG_LEFT = 1;
            break;
         case RIGHT:
            Board_LED_Set(4, true);
            FLAG_RIGHT = 1;
            break;
         default:
            break;
      }
      
      switch(dirs[1]){
         case NONE:
            break;
         case UP:
            Board_LED_Set(0, true);
            FLAG_UP = 1;
            break;
         case DOWN:
            Board_LED_Set(1, true);
            FLAG_DOWN = 1;
            break;
         default:
            break;
      }
      
      // Lectura del botón SW
      if( !gpioRead( T_COL1 ) ){
         Board_LED_Set(5, true);
         FLAG_ENTER = 1;
      } else Board_LED_Set(5, false);
      
      /*
      if (valorEjeX > 400 && valorEjeX < 624){
         apagarPares();
         apagarImpares();
      } else if (valorEjeX <= 400) {
         encenderPares();
         apagarImpares();
      } else if (valorEjeX >= 624) {
         encenderImpares();
         apagarPares();
      }*/
      
      /*
      if (valorEjeX < 10){
         encenderPares();
      } else apagarPares();
      
      if (valorEjeY > 1000){
         encenderImpares();
      } else apagarImpares();*/
      
      /* Sleep until next Interrupt happens */
      // sleepUntilNextInterrupt();
      
      // Si el pulsador TEC_1 está presionado
      /*
      if (gpioRead(TEC1) == 0){
         encenderImpares();
         delay(200);
         apagarImpares();
         delay(200);
      } else {
         encenderPares();
         delay(100);
         apagarPares();
         delay(100);
      }
      */
      
      usbDeviceKeyboardTasks();
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}
