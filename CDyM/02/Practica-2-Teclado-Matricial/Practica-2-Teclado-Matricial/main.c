/*
 * Practica-2-Teclado-Matricial.c
 *
 * Created: 11/4/2022 14:37:06
 * Author : Barcala
 */ 

#include "main.h"

int main(void)
{
	uint8_t greeting[10] = {'h', 'o', 'l', 'a', ' ', 'm', 'u', 'n', 'd', 'o'};
		
    /* Setup */
	KEYPAD_Init();
	LCDinit();
	_delay_ms(1);
	
	LCDclr();
	_delay_ms(1);
	
	LCDstring(greeting, 10);
	//LCDsendChar(' ');
	//LCDstring(greeting2, 5);
	
	/* Loop */
    while (1) 
    {
		_delay_ms(50);
		uint8_t tecla;
		uint8_t presiono = KEYPAD_Scan(&tecla);
		
		//uint8_t tecla = keypadUpdate();
		if (presiono)
		{
			LCDGotoXY(11,0);		// retrocedo para pisar valor anterior
			LCDsendChar(tecla);		// escribo 1 caracter ASCII
		}
    }
}

/*
uint8_t keypadUpdate(void){
	unsigned char secuencia[] = {0xFE, 0xFD, 0xFB, 0xF7 };
	unsigned char i, renglon = 0, dato;
	
	for(i = 0; i < 4; i++) {
		PORTB = secuencia[i]; // Ubica la salida
		_delay_ms(20);
		dato = PINB & 0xF0; // Lee la entrada (anula la parte baja)
		
		if( dato != 0xF0 ) { // Si se presionó una tecla
			_delay_ms(20); // Evita rebotes
			
			switch(dato) { // Revisa las columnas
				case 0xE0: return caracteres[renglon][0];
				case 0xD0: return caracteres[renglon][1];
				case 0xB0: return caracteres[renglon][2];
				case 0x70: return caracteres[renglon][3];
			}
		}
		
		renglon += 1; // Revisa el siguiente renglón
	}
	
	return 0xFF;
}

*/