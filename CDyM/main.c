/*
 * TP1.c
 *
 * Created: 28/3/2022 14:52:49
 * Autores: Ercoli Juan & Calderon Sergio
 */ 

#include <avr/io.h>


int main(void)
{
	/* Setup */
	DDRB = 0xFF;			// Todo PORTB es salida
	DDRC &= ~(1<<PORTC0);	// Bit 0 de PORTC es entrada
	
	uint8_t op1 = 1;
	uint8_t op2 = 8;
	uint8_t estado = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		
		for (uint8_t i=0; i<4; i++){
			PORTB = (op2 << 4) | op1;
			
			// Mientras el pulsador esté presionado, esperar.
			if (PINC & (1<<PINC0)){
				while (PINC & (1<<PINC0));
				estado = estado ? 0 : 1;
			}
			
			// Mover operandos
			if (estado){
				op1 >>= 1;
				op2 <<= 1;
			} else {
				op1 <<= 1;
				op2 >>= 1;
			}
			
		}
		
		op1 = 1;
		op2 = 8;
    }
}



