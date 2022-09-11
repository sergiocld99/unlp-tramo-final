/*
 * led.c
 *
 * Created: 4/7/2022 14:34:24
 *  Author: Calderón Sergio
 */ 

// Archivo de cabecera
#include "led.h"

// Declaración de tipos
typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} LED_UserValues;

// Prototipos de funciones privadas
static void setAlto(uint8_t);
static void setBajo(uint8_t);

// Variables privadas
static LED_UserValues userValues = {0,0,0};

// ----------------- Implementación de funciones públicas ------------------

void LED_Init()
{
	// Configurar cada pin conectado a un LED como salida
	DDRB |= (1<<PIN_RED)|(1<<PIN_GREEN)|(1<<PIN_BLUE);
	
	// Apagar todos los LEDs por defecto
	LED_OFF(PIN_RED);
	LED_OFF(PIN_GREEN);
	LED_OFF(PIN_BLUE);
}

void LED_KeepOnly(LED_Option option)
{
	switch(option)
	{
		case OP_RED:
			LED_UpdateGreen(0,1);
			LED_UpdateBlue(0,1);
			break;
		case OP_GREEN:
			LED_UpdateRed(0,1);
			LED_UpdateBlue(0,1);
			break;
		case OP_BLUE:
			LED_UpdateRed(0,1);
			LED_UpdateGreen(0,1);
			break;
		case OP_ALL:
			break;
	}
}

void LED_Update(uint8_t red, uint8_t green, uint8_t blue)
{
	
	// Asignar la intensidad de rojo
	LED_UpdateRed(red, 1);
	
	// Asignar la intensidad de verde
	LED_UpdateGreen(green, 1);
	
	// Asignar la intensidad de azul
	LED_UpdateBlue(blue, 1);
}

void LED_UpdateCustom(LED_Option option, uint8_t valor)
{
	switch(option)
	{
		case OP_RED:
			LED_UpdateRed(valor, 0);
			break;
		case OP_GREEN:
			LED_UpdateGreen(valor, 0);
			break;
		case OP_BLUE:
			LED_UpdateBlue(valor, 0);
			break;
		case OP_ALL:
			LED_UpdateBright(valor);
			break;
		default:
			break;
	}
}

void LED_UpdateRed(uint8_t bright, uint8_t save)
{
	// El LED rojo se controla mediante interrupciones de T0
	// La ISR de TIMER0_OVF enciende el LED
	// La ISR de TIMER0_COMPA apaga el LED
	
	// Si la intensidad es 0, se debe mantener apagado el LED
	if (bright == 0)
	{
		// Desactivar interrupción de Overflow
		RELOJ_T0_OVF_Interrupt_Disable();
		
		// Forzar apagado del LED
		LED_OFF(PIN_RED);
	}
	else if (bright == 255)
	{
		// Sino, si la intensidad es máxima, se debe mantener encendido
		// Desactivar interrupción de Compare A
		RELOJ_T0_COMPA_Interrupt_Disable();
		
		// Forzar encendido del LED
		LED_ON(PIN_RED);
	} else {
		// Sino...
		// Activar interrupciones de T0
		RELOJ_T0_OVF_Interrupt_Enable();
		RELOJ_T0_COMPA_Interrupt_Enable();
		
		// Actualizar valor de brillo
		RELOJ_T0_UpdateOCR(bright);
	}
	
	// Guardar intensidad personalizada
	if (save) userValues.red = bright;
}

void LED_UpdateGreen(uint8_t bright, uint8_t save)
{
	// El LED verde se controla mediante señal PWM de OC1B
	// Se trabaja en Fast PWM, modo invertido.
	
	// Si la intensidad es 0, se debe mantener apagado el LED
	if (bright == 0)
	{
		// Desconectar señal OC1B
		RELOJ_T1_PWM_B_Disable();
		
		// Forzar apagado del LED
		LED_OFF(PIN_GREEN);
	} else {
		// Sino...
		// Habilitar señal OC1B
		RELOJ_T1_PWM_B_Enable();
		
		// Actualizar valor de brillo
		RELOJ_T1_PWM_B_UpdateOCR(bright);
	}
	
	// Guardar intensidad personalizada
	if (save) userValues.green = bright;
}

void LED_UpdateBlue(uint8_t bright, uint8_t save)
{
	// El LED azul se controla mediante señal PWM de OC1A
	// Se trabaja en Fast PWM, modo invertido.
	
	// Si la intensidad es 0, se debe mantener apagado el LED
	if (bright == 0)
	{
		// Desconectar señal OC1A
		RELOJ_T1_PWM_A_Disable();
		
		// Forzar apagado del LED
		LED_OFF(PIN_BLUE);
	} else {
		// Sino...
		// Habilitar señal OC1A
		RELOJ_T1_PWM_A_Enable();
		
		// Actualizar valor de brillo
		RELOJ_T1_PWM_A_UpdateOCR(bright);
	}
	
	// Guardar intensidad personalizada
	if (save) userValues.blue = bright;
}

void LED_UpdateBright(uint8_t intensity)
{
	// Calcular valores de brillo a asignar
	uint8_t red = userValues.red * intensity / 255;
	uint8_t green = userValues.green * intensity / 255;
	uint8_t blue = userValues.blue * intensity / 255;
	
	// Asignar nuevo brillo de rojo, sin guardar
	LED_UpdateRed(red, 0);
	
	// Asignar nuevo brillo de verde, sin guardar
	LED_UpdateGreen(green, 0);
	
	// Asignar nuevo brillo de azul, sin guardar
	LED_UpdateBlue(blue, 0);
}


void LED_ON(uint8_t pin)
{
	// Poner un nivel alto o bajo según macro NIVEL_ON
	
	#if NIVEL_ON
		setAlto(pin);
	#else
		setBajo(pin);
	#endif
}

void LED_OFF(uint8_t pin)
{
	// Poner un nivel alto o bajo según macro NIVEL_OFF
	
	#if NIVEL_OFF
		setAlto(pin);
	#else
		setBajo(pin);
	#endif
}

static void setAlto(uint8_t pin)
{
	PORTB |= (1<<pin);
}

static void setBajo(uint8_t pin)
{
	PORTB &= ~(1<<pin);
}