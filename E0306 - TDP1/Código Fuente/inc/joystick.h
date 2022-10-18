/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/

#ifndef _JOYSTICK_H_
   #define _JOYSTICK_H_

   /*==================[inclusiones]============================================*/
   #include "sapi.h"
   #include <stdlib.h>

   /*==================[macros]=================================================*/
   #define NEUTRO_MIN 400
   #define NEUTRO_MAX 624

   /*==================[tipos de datos declarados por el usuario]===============*/
   typedef enum {
      UP, LEFT, RIGHT, DOWN, NONE
   } Joystick_Direccion;

   /*==================[declaraciones de datos externos]========================*/

   /*==================[declaraciones de funciones públicas]====================*/
   
   Joystick_Direccion Joystick_ProcesarDir(uint16_t valorEjeX, uint16_t valorEjeY);
   void Joystick_LeerDirs(uint16_t valorEjeX, uint16_t valorEjeY, Joystick_Direccion* dirs);
   
/*==================[end of file]============================================*/
#endif /* _JOYSTICK_H_ */