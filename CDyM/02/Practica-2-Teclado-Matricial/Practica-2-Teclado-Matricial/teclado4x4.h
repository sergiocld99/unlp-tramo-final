/*
 * teclado4x4.h
 *
 * Created: 25/4/2022 14:31:13
 *  Author: Barcala
 */ 


#ifndef TECLADO4X4_H_
#define TECLADO4X4_H_

#include "main.h"

// Prototipos de funciòn

/************************************************************************/
/* ESTABLECE LOS PUERTOS PARA LA LECTURA DE LOS 16 PULSADORES                                                                     */
/************************************************************************/
void KEYPAD_Init();

// Permite encuestar el teclado para determinar si se presionó una tecla y el valor de la misma. La función debe retornar 0
// si no se presionó ninguna tecla o 1 si se presionó alguna. 
//El valor de la tecla (o carácter ASCII) debe retornarse por el puntero pasado como parámetro (*key).
uint8_t KEYPAD_Scan(uint8_t *key);

#endif /* TECLADO4X4_H_ */