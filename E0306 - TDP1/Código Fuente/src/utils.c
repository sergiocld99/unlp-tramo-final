// Cabecera propia del archivo
#include "utils.h"

// --------------------------- IMPLEMENTACION DE FUNCIONES PÚBLICAS ------------------

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

void encenderPares(){
   Board_LED_Set(0, true);
   Board_LED_Set(2, true);
   Board_LED_Set(4, true);
   // gpioWrite(LED2, ON);
}

void encenderImpares(){
   Board_LED_Set(1, true);
   Board_LED_Set(3, true);
   Board_LED_Set(5, true);
}

void apagarPares(){
   Board_LED_Set(0, false);
   Board_LED_Set(2, false);
   Board_LED_Set(4, false);
}

void apagarImpares(){
   Board_LED_Set(1, false);
   Board_LED_Set(3, false);
   Board_LED_Set(5, false);
}

void apagarTodos(){
   apagarPares();
   apagarImpares();
}