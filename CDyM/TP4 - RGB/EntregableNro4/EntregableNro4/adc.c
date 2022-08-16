/*
 * adc.c
 *
 * Created: 10/7/2022 16:14:46
 *  Author: Calderón Sergio
 */ 

// Archivo de cabecera
#include "adc.h"

// ---------------- Implementación de funciones públicas ---------------

void ADC_Init()
{
	// Configurar pin de ADC3 como entrada analógica (PC3)
	DDRC &= ~(1<<PORTC3);
	DIDR0 |= (1<<PORTC3);
	
	// Establecer VCC como referencia (REFS1:0 = 1)
	ADMUX = (1<<REFS0);
	
	// Seleccionar ADC3 como entrada analógica (MUX3:0 = 3)
	ADMUX |= (1<<MUX1)|(1<<MUX0);
	
	// Ubicar los 10 bits desde la izquierda (ADLAR = 1)
	ADMUX |= (1<<ADLAR);
	
	// Establecer prescaler de 128, mediante ADPS2:0 = 7
	// Se garantiza una buena performance hasta 200 kHz
	// Para 16 MHz, la frecuencia del ADC será 125 kHz
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	// Habilitar interrupción de conversión finalizada
	ADCSRA |= (1<<ADIE);
}

void ADC_Enable()
{
	// Habilitar conversor ADC mediante bit ADC Enable
	ADCSRA |= (1<<ADEN);
}

void ADC_Disable()
{
	// Deshabilitar conversor ADC
	ADCSRA &= ~(1<<ADEN);
}

void ADC_Run()
{
	// Iniciar conversión (se realiza una sola)
	ADCSRA |= (1<<ADSC);
}

uint8_t ADC_GetResult()
{
	// Devuelvo parte alta del registro ADC
	// ADC utiliza 10 bits (1024 niveles), con 8 bits: 4 niveles x bit
	return ADCH;
}