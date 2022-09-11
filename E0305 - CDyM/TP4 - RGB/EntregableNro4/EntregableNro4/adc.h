/*
 * adc.h
 *
 * Created: 10/7/2022 16:19:55
 *  Author: Calderón Sergio
 */ 


#ifndef ADC_H_
	#define ADC_H_

	// ------------------------ Includes -----------------
	
	// Archivo de cabecera del Microcontrolador
	#include <avr/io.h>

	
	// ----------------- Prototipos de funciones públicas ---------
	
	void ADC_Init();
	
	void ADC_Enable();
	
	void ADC_Disable();
	
	void ADC_Run();
	
	uint8_t ADC_GetResult();


#endif /* ADC_H_ */