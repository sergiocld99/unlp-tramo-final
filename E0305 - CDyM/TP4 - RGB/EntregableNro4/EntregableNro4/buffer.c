/*
 * buffer.c
 *
 * Created: 24/6/2022 22:42:16
 *  Author: Calderon Sergio
 */ 

#include "buffer.h"

// Variables privadas

// Búfferes de tamaño asignado según su función
static uint8_t RX_Data[RX_BUFFER_SIZE];
static uint8_t TX_Data[TX_BUFFER_SIZE];

// Estructura tipo Buffer
typedef struct {
	uint8_t* data;
	uint8_t readIndex;
	uint8_t writeIndex;
} TipoBuffer;

static TipoBuffer RX_Buffer = {RX_Data, 0, 0};
static TipoBuffer TX_Buffer = {TX_Data, 0, 0};

// Funciones públicas

void Buffer_RX_Push(uint8_t dato)
{
	// Asignar dato en la posición de escritura
	RX_Buffer.data[RX_Buffer.writeIndex] = dato;
	
	// Incrementar indice de escritura
	RX_Buffer.writeIndex = (RX_Buffer.writeIndex + 1) % RX_BUFFER_SIZE;
}

uint8_t Buffer_RX_Pop()
{
	// Recuperar dato en la posición de lectura
	uint8_t dato = RX_Buffer.data[RX_Buffer.readIndex];
	
	// Incrementar indice de lectura
	RX_Buffer.readIndex = (RX_Buffer.readIndex + 1) % RX_BUFFER_SIZE;
	
	// Devolver dato
	return dato;
}

void Buffer_RX_PopString(char* texto, uint8_t maxLength)
{
	// Inicializar indice de String en cero
	uint8_t dato, i = 0;
	
	// Hasta alcanzar un fin de cadena o la longitud máxima...
	do 
	{
		// Retirar dato del Buffer RX
		dato = Buffer_RX_Pop();
		
		// Colocar dato en la posición "i" del String
		texto[i] = dato;
		
		// Incrementar indice
		i++;
	} while (dato != '\0' && i < maxLength);
	
	// Adelantar indice de lectura hasta el de escritura
	// Se ignoran el resto de datos que no se retiraron
	RX_Buffer.readIndex = RX_Buffer.writeIndex;
}

void Buffer_TX_Push(uint8_t dato)
{
	// Asignar dato en la posición de escritura
	TX_Buffer.data[TX_Buffer.writeIndex] = dato;
	
	// Incrementar indice de escritura
	TX_Buffer.writeIndex = (TX_Buffer.writeIndex + 1) % TX_BUFFER_SIZE;
}

void Buffer_TX_PushString(char* texto)
{
	// Por cada caracter del String
	for(uint8_t i=0; texto[i] != '\0'; i++)
	{
		// Agregar caracter al Buffer TX
		Buffer_TX_Push(texto[i]);
	}
}

void Buffer_TX_PushLine(char* texto)
{
	// Por cada caracter del String
	for(uint8_t i=0; texto[i] != '\0'; i++)
	{
		// Agregar caracter al Buffer TX
		Buffer_TX_Push(texto[i]);
	}
	
	// Agregar salto de linea
	Buffer_TX_Push('\r');
	Buffer_TX_Push('\n');
}

uint8_t Buffer_TX_Pop()
{
	// Obtener dato en la posición de lectura
	uint8_t dato = TX_Buffer.data[TX_Buffer.readIndex];
	
	// Incrementar indice de lectura
	TX_Buffer.readIndex = (TX_Buffer.readIndex + 1) % TX_BUFFER_SIZE;
	
	// Devolver dato
	return dato;
}

uint8_t Buffer_TX_Empty()
{
	// Devolver si los indices de lectura y escritura coinciden
	return TX_Buffer.readIndex == TX_Buffer.writeIndex;
}