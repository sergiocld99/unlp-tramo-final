/*
 * controlador.c
 *
 * Created: 30/7/2022 04:31:26
 *  Author: Calderón Sergio
 */ 

// Archivo de cabecera
#include "controlador.h"

// Variables privadas
static LED_Option selectedLed = OP_ALL;
static uint8_t adcEnabled = 0;

// Funciones públicas

void Controlador_Select(LED_Option option)
{
	// Actualizar LED seleccionado
	selectedLed = option;
	
	// Apagar los otros LEDs
	LED_KeepOnly(option);
}

void Controlador_Set_RGB(uint8_t r, uint8_t g, uint8_t b)
{
	if (adcEnabled) CLI_EscribirMensaje("--> Advertencia: ADC esta activo");
	LED_Update(r,g,b);
}

void Controlador_Clear()
{
	// Asignar un brillo de 0 a todos los LEDs
	LED_Update(0,0,0);
	
	// Borrar el LED seleccionado
	selectedLed = OP_ALL;
}

// ------------------------------- Manejo de ADC -----------------------

void Controlador_ADC_Update(uint8_t bright)
{
	LED_UpdateCustom(selectedLed, bright);
}

void Controlador_ADC_ON()
{
	ADC_Enable();
	ADC_Run();
	
	adcEnabled = 1;
}

void Controlador_ADC_OFF()
{
	ADC_Disable();
	
	adcEnabled = 0;
}