/*
 * EntregableNro4.c
 *
 * Created: 4/7/2022 14:31:36
 * Author : Calderón Sergio
 */ 

// Archivo de cabecera
#include "main.h"

static volatile uint8_t FLAG_DATOS = 0;
static volatile uint8_t FLAG_ADC = 0;

int main(void)
{
	/* Setup */
	
	// Inicialización de USART
	SerialPort_Init(F_CPU);
	SerialPort_TX_Enable();
	SerialPort_RX_Enable();
	SerialPort_RX_Interrupt_Enable();
	
	// Inicialización de Timer
	RELOJ_T1_Init();
	RELOJ_T0_Init();
	RELOJ_Start_Both();
	
	// Inicialización de LEDs
	LED_Init();
	
	// Inicialización de Conversor ADC
	ADC_Init();
	
	// Visualización de Menú
	CLI_MostrarMenu();
	
	// Habilitar interrupciones globales
	sei();
	
    /* Loop */
    while (1) 
    {
		// Si se convirtió el valor de potenciómetro...
		if (FLAG_ADC)
		{
			// Leer valor
			uint8_t valor = ADC_GetResult();
			
			// Actualizar brillo
			Controlador_ADC_Update(valor);
			
			// Iniciar nueva conversión
			ADC_Run();
			
			// Desactivar flag
			FLAG_ADC = 0;
		}
		
		// Si se ingresaron datos... (Flag de Datos activo)
		if (FLAG_DATOS)
		{
			char texto[20];
			
			// Obtener texto ingresado (máx 20 caracteres)
			Buffer_RX_PopString(texto, 20);
			
			// Interpretar y ejecutar comando
			CLI_LeerComando(texto);
			
			// Desactivar flag
			FLAG_DATOS = 0;
		}
		
		// Suspender uso de CPU hasta próxima interrupción
		sleep_mode();
    }
	
	return 0;
}

// Rutina de Servicio de Interrupción del Conversor Analógico-Digital
ISR(ADC_vect)
{
	FLAG_ADC = 1;
}

// Rutina de Servicio de Interrupción de Timer 0 - Overflow
// De forma análoga al Modo Invertido de Timer 1, empieza en nivel bajo
ISR(TIMER0_OVF_vect)
{
	LED_ON(PIN_RED);
}

// Rutina de Servicio de Interrupción de Timer 0 - Compare Match A
// Luego de haberse generado un nivel bajo, se pasa a nivel alto
ISR(TIMER0_COMPA_vect)
{
	LED_OFF(PIN_RED);
}

// Rutina de Servicio de Interrupción de Byte Recibido
// Tarea en Foreground - Productor de Comandos
ISR(USART_RX_vect)
{
	// Leer dato de UDR (USART)
	volatile uint8_t aux = SerialPort_Receive_Data();
	
	// Si se presionó Enter
	if (aux == '\r')
	{
		// Agregar un fin de cadena al Buffer RX
		Buffer_RX_Push('\0');
		
		// Activar Flag "Hay Datos"
		FLAG_DATOS = 1;
	} else {
		// Sino, agregar dato al Buffer RX
		Buffer_RX_Push(aux);
	}
}

// Rutina de Servicio de Interrupción "Libre para Transmitir"
// Tarea en Foreground - Consumidor de Mensajes
ISR(USART_UDRE_vect)
{
	// Retirar dato del Buffer TX
	volatile uint8_t dato = Buffer_TX_Pop();
	
	// Escribir dato en UDR (USART)
	SerialPort_Send_Data(dato);
	
	// Si no hay más datos...
	if (Buffer_TX_Empty())
	{
		// Deshabilitar interrupciones de transmisión
		SerialPort_TX_Interrupt_Disable();
	}
}

