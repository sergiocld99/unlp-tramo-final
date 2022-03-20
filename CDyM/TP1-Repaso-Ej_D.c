/*
 * MiPrimerHolaMundo.c
 *
 * Created: 14/3/2022 15:26:25
 * Author : NotBad
 */ 

#include <avr/io.h>

void mostrar(int8_t);

int main(void)
{
	mostrar(0xAA);	
	while(1);
	return 0;
}

void mostrar(int8_t num){
	
	DDRB = 0xFF;	// puerto B todo salida
	
	for(uint8_t i=0; i<8; i++){
		PORTB = num&0x01;
		num >>= 1;
		
		// demora para que se envíe un bit cada 1 ms
		for (uint8_t i=0; i<247; i++){
			int a;
		}
	}
}

