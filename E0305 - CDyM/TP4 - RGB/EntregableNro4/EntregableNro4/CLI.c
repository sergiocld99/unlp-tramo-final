/*
 * GUI.c
 *
 * Created: 7/7/2022 03:09:19
 *  Author: Calderón Sergio
 */ 

// Archivo de cabecera
#include "CLI.h"


// ------------------ Funciones públicas --------------

void CLI_LeerComando(char* texto)
{
	char comando[10];
	char param[10];
	
	// Obtener comando y parámetro escrito a continuación
	PARSER_Split(texto, comando, param);
	
	// Comprobar si el comando existe y actuar de acuerdo al parámetro
	if (PARSER_Equals(comando, "SEL"))
	{
		if (PARSER_Equals(param, "R")) Controlador_Select(OP_RED);
		else if (PARSER_Equals(param, "G")) Controlador_Select(OP_GREEN);
		else if (PARSER_Equals(param, "B")) Controlador_Select(OP_BLUE);
		else if (PARSER_Equals(param, "*")) Controlador_Select(OP_ALL);
		else CLI_EscribirMensaje("--> Opciones validas: R, G, B, *");
	} else if (PARSER_Equals(comando, "RGB")) {
		uint8_t r,g,b,status;
		status = PARSER_Convert(param, &r, &g, &b);
		if (status == OK) Controlador_Set_RGB(r,g,b);
		else if (status == ERROR_PARAM) CLI_EscribirMensaje("--> Se esperaban 3 parametros");
		else if (status == ERROR_CAST) CLI_EscribirMensaje("--> Los parametros deben ser numeros");
	} else if (PARSER_Equals(comando, "ADC")) {
		if (PARSER_Equals(param, "ON")) Controlador_ADC_ON();
		else if (PARSER_Equals(param, "OFF")) Controlador_ADC_OFF();
		else CLI_EscribirMensaje("--> Opciones validas: ON, OFF");
	} else if (PARSER_Equals(comando, "CLR")) Controlador_Clear();
	else CLI_EscribirMensaje("--> Comandos validos: RGB, SEL, ADC, CLR");
}

void CLI_MostrarMenu()
{
	Buffer_TX_PushLine("RGB - Asigna las 3 intensidades al LED asociado");
	Buffer_TX_PushLine("SEL [R,G,B,*] - Selecciona un LED");
	Buffer_TX_PushLine("ADC [ON,OFF] - Activa/desactiva ADC");
	Buffer_TX_PushLine("CLR - Apaga los LEDs");
	SerialPort_TX_Interrupt_Enable();
}

void CLI_EscribirMensaje(char* texto)
{
	Buffer_TX_PushLine(texto);
	SerialPort_TX_Interrupt_Enable();
}