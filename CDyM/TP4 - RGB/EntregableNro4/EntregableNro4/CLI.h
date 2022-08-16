/*
 * CLI.h
 *
 * Created: 7/7/2022 03:13:15
 *  Author: Calderón Sergio
 */ 


#ifndef GUI_H_
	#define CLI_H_

	// ------------------------- Includes --------------------

	// Tipos de datos enteros estandarizados
	#include <stdint.h>
	
	// Utilidad para convertir String a entero
	#include <stdlib.h>
	
	// Biblioteca Parser de utilidades
	#include "parser.h"
	
	// Biblioteca Buffer
	#include "buffer.h"
	
	// Biblioteca del USART
	#include "serialPort.h"
	
	// Archivo de cabecera del Controlador
	#include "controlador.h"
	
	// ------------------- Prototipos de funciones públicas ---------
	
	// Muestra el menú de comandos disponibles
	void CLI_MostrarMenu();
	
	// Interpreta el texto pasado por parámetro como un comando y lo ejecuta
	void CLI_LeerComando(char* texto);
	
	// Agrega el texto pasado por parámetro al Buffer TX y activa la transmisión
	void CLI_EscribirMensaje(char*);

#endif /* GUI_H_ */