/*
 * teclado4x4.c
 *
 * Created: 25/4/2022 14:30:55
 *  Author: Barcala
 */ 

#include "teclado4x4.h"

static const uint8_t caracteres[4][4] =
{
	{'1', '2', '3', 'A'} ,
	{'4', '5', '6', 'B'} ,
	{'7', '8', '9', 'C'} ,
	{'*', '0', '#', 'D'}
};

static const uint8_t filas[3] = {PINB4, PINB3, PINB0};
static const uint8_t columnas[4] = {PORTD3, PORTD5, PORTD4, PORTD2};

void KEYPAD_Init()
{
	// Se utilizarán las filas como entradas (PULL-UP)
	//DDRB = 0xE6;	// 1110 0110 (PB0, PB3 y PB4)
	DDRB &= ~((1<<PORTB0) | (1<<PORTB3) | (1<<PORTB4));
	//DDRD = 0x7F;	// 0111 1111 (PD7)
	DDRD &= ~(1<<PORTD7);
	//PORTB = 0x19 = 0001 1001 ;
	PORTB |= (1<<PORTB0) | (1<<PORTB3) | (1<<PORTB4);
	//PORTD = 0x80;
	PORTD |= (1<<PORTD7);
	
	// Se utilizarán las columnas como salidas
	DDRD |= (1 << PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);		// 0011 1100 (PD2, PD3, PD4 y PD5)
	PORTD &= ~((1<< PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5));		// 1100 0011
}

uint8_t KEYPAD_Scan(uint8_t *key)
{
	uint8_t reposo;
	uint8_t presiono = 0;
	
	for (uint8_t c=0; c < 4; c++)
	{
		DDRD |= (1<<columnas[c]);		// poner como salida
		PORTD &= ~(1<<columnas[c]);		// escribir un 0
		
		// filas 0 a 2
		for (uint8_t f=0; f < 3; f++)
		{
			reposo = PINB & (1<<filas[f]);
			if (!reposo)
			{
				*key = caracteres[f][c];
				presiono = 1;
			}
		}
		
		// fila 3
		reposo = PIND & (1<<PIND7);
		if (!reposo)
		{
			//*key = caracteres[3][c];
			//presiono = 1;
		}
		
		PORTD |= (1<<columnas[c]);
		DDRD &= ~(1<<columnas[c]);
	}
	
	return presiono;		// se devuelve 0 si no se presionó nada, o 1 en cc
}