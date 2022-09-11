/*
 * controlador.h
 *
 * Created: 30/7/2022 04:31:12
 *  Author: Calderón Sergio
 */ 


#ifndef CONTROLADOR_H_
	#define CONTROLADOR_H_

	// ----------------------- Includes ---------------
	
	// Archivo de cabecera del Módulo LED
	#include "led.h"
	
	// Archivo de cabecera del Conversor Analógico-Digital
	#include "adc.h"

	// Archivo de cabecera para manejo de mensajes
	#include "CLI.h"

	// ----------------- Prototipos de funciones públicas ----------
	
	// Selecciona un LED para su posterior regulación con ADC
	void Controlador_Select(LED_Option option);
	
	void Controlador_Set_RGB(uint8_t r, uint8_t g, uint8_t b);
	
	void Controlador_Clear();
	
	void Controlador_ADC_Update(uint8_t bright);
	
	void Controlador_ADC_ON();
	void Controlador_ADC_OFF();

#endif /* CONTROLADOR_H_ */