/*
 * reloj.c
 *
 * Created: 4/7/2022 14:49:56
 *  Author: Calderón Sergio
 */ 

// Archivo de cabecera
#include "reloj.h"

// ---------------- Implementación de funciones públicas -------------

void RELOJ_T0_Init()
{
	// Deshabilitar todas las interrupciones de Timer 0
	TIMSK0 &= ~((1<<TOIE0)|(1<<OCIE0A)|(1<<OCIE0B));
	
	// Asignar Modo Normal, se cuenta hasta 0xFF (255)
	TCCR0A = 0;
	
	// Detener reloj interno (CS0[2:0] = 0)
	TCCR0B = 0;
	
	// Al inicio, el LED está apagado (intensidad de color 0)
	OCR0A = 0;
	
	// Reiniciar contador a cero
	TCNT0 = 0;
}

void RELOJ_T1_Init()
{
	// Deshabilitar interrupciones de Timer 1
	TIMSK1 &= ~((1<<TOIE1)|(1<<OCIE1A)|(1<<OCIE1B));
	
	// Asignar Modo Fast PWM de 8 bits (WGM1[3:0] = 5)
	// Detener reloj interno (CS1[2:0] = 0)
	TCCR1B = (1<<WGM12);
	TCCR1A = (1<<WGM10); 
	
	// Reiniciar valores de comparación
	OCR1A = 0;
	OCR1B = 0;
	
	// Reiniciar contador a cero
	TCNT1 = 0;
}

void RELOJ_Start_Both()
{
	// Asignar prescaler de N = 1024 (CS0[2:0] = 5)
	// La frecuencia generada es 61 Hz, mayor a 50 Hz
	TCCR0B |= (1<<CS02)|(1<<CS00);
	TCCR1B |= (1<<CS12)|(1<<CS10);
}

// -------------------------------- TIMER 1 ---------------------------

void RELOJ_T1_PWM_A_Enable()
{
	// Establecer modo invertido para OC1A
	TCCR1A |= (1<<COM1A1)|(1<<COM1A0);
}

void RELOJ_T1_PWM_B_Enable()
{
	// Establecer modo invertido para OC1B
	TCCR1A |= (1<<COM1B1)|(1<<COM1B0);
}

void RELOJ_T1_PWM_A_Disable()
{
	// Desconectar salida OC1A
	TCCR1A &= ~((1<<COM1A1)|(1<<COM1A0));
}

void RELOJ_T1_PWM_B_Disable()
{
	// Desconectar salida OC1B
	TCCR1A &= ~((1<<COM1B1)|(1<<COM1B0));
}

// Modifica el valor del registro OCR1A
// En este proyecto se traduce como intensidad de azul
void RELOJ_T1_PWM_A_UpdateOCR(uint8_t valor)
{
	OCR1A = valor;
}

// Modifica el valor del registro OCR1B
// En este proyecto se traduce como intensidad de verde
void RELOJ_T1_PWM_B_UpdateOCR(uint8_t valor)
{
	OCR1B = valor;
}

// ------------------------------ TIMER 0 ----------------------------------

void RELOJ_T0_OVF_Interrupt_Enable()
{
	TIMSK0 |= (1<<TOIE0);
}

void RELOJ_T0_OVF_Interrupt_Disable()
{
	TIMSK0 &= ~(1<<TOIE0);
}

void RELOJ_T0_COMPA_Interrupt_Enable()
{
	TIMSK0 |= (1<<OCIE0A);
}

void RELOJ_T0_COMPA_Interrupt_Disable()
{
	TIMSK0 &= ~(1<<OCIE0A);
}

void RELOJ_T0_UpdateOCR(uint8_t valor)
{
	OCR0A = valor;
}