/*
 * led.h
 *
 * Created: 4/7/2022 14:34:38
 *  Author: Calderón Sergio
 */ 


#ifndef LED_H_
	#define LED_H_

	// ------------------------- Includes ------------------------------
	
	// Archivo de cabecera del Microcontrolador
	#include <avr/io.h>

	// Archivo de cabecera de Timer 0 y 1
	#include "reloj.h"

	// ------------------------ Tipos de Datos -------------------------
	
	// LEDs disponibles para la modificación de su brillo
	typedef enum {OP_RED, OP_GREEN, OP_BLUE, OP_ALL} LED_Option;

	// ------------------------- Constantes -----------------------------
	
	// Valor a escribir en la salida
	// Si es ánodo común (VCC), se escribe un 0 para encender
	// Si es cátodo común (GND), se escribe un 1 para encender
	
	#define ANODO_COMUN 1
	
	#if ANODO_COMUN
		#define NIVEL_ON 0
		#define NIVEL_OFF 1
	#else
		#define NIVEL_ON 1
		#define NIVEL_OFF 0
	#endif
	
	// Pin conectado a cada LED
	#define PIN_RED PORTB5
	#define PIN_GREEN PORTB2
	#define PIN_BLUE PORTB1

	// ------------ Prototipos de funciones públicas -----------------

	// Configura los pines conectados al LED como salida
	void LED_Init();
	
	// Mantiene el brillo únicamente del LED indicado
	void LED_KeepOnly(LED_Option option);
	
	// Actualiza los valores de intensidad para cada color
	void LED_Update(uint8_t red, uint8_t green, uint8_t blue);
	
	// Actualiza el valor de intensidad del LED especificado
	void LED_UpdateCustom(LED_Option option, uint8_t valor);
	
	// Actualiza el valor de intensidad del LED rojo
	void LED_UpdateRed(uint8_t bright, uint8_t save);
	
	// Actualiza el valor de intensidad del LED verde
	void LED_UpdateGreen(uint8_t bright, uint8_t save);
	
	// Actualiza el valor de intensidad del LED azul
	void LED_UpdateBlue(uint8_t bright, uint8_t save);
	
	// Modifica el brillo del color actual
	void LED_UpdateBright(uint8_t intensity);
	
	// Apaga el LED ubicado en el PIN de Port B especificado
	void LED_ON(uint8_t);
	
	// Enciende el LED ubicado en el PIN de Port B especificado
	void LED_OFF(uint8_t);

#endif /* LED_H_ */