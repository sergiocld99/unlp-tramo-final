/*
 * buffer.h
 *
 * Created: 24/6/2022 22:40:20
 *  Author: Calderón Sergio
 */ 


#ifndef BUFFER_H_
	#define BUFFER_H_

	// ------------------- Includes ----------------------------
	
	// Archivo de cabecera del Microcontrolador
	#include <avr/io.h>
	
	// ------------------- Constantes ----------------------------
	
	// Tamaño del buffer en bytes
	#define RX_BUFFER_SIZE 32
	#define TX_BUFFER_SIZE 196
	
	// -------- Prototipos de funciones públicas ---------------
	
	// Operaciones sobre Buffer RX
	void Buffer_RX_Push(uint8_t);
	void Buffer_RX_PopString(char* texto, uint8_t maxLength);
	uint8_t Buffer_RX_Pop();
	
	// Operaciones sobre Buffer TX
	void Buffer_TX_Push(uint8_t);
	void Buffer_TX_PushString(char*);
	void Buffer_TX_PushLine(char*);		// Agrega texto + CR
	uint8_t Buffer_TX_Pop();
	uint8_t Buffer_TX_Empty();	

#endif /* BUFFER_H_ */