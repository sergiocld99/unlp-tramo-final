#include "main.h"

static uint8_t estadoFlag();
static void borrarFlag();

int main(void)
{
	/* Setup */
	uint16_t t, periodo;
	
	// Configuración del puerto conectado al motor (PB0 es entrada)
	DDRB &= ~(1<<PORTB0);
	
	// Configuración en modo normal, prescaler de 8 (16 MHz a 2 MHz)
	TCCR1A = 0;
	if (PREESCALER == 1) TCCR1B = (1<<ICES1) | (1<<CS10);
	else if (PREESCALER == 8) TCCR1B = (1<<ICES1)|(1<<CS11);
	
	// Inicialización de LCD
	LCDinit();
	LCDclr();
	
	// Asumiendo numero de 3 cifras, escribo unidad al final del mismo
	uint8_t mensaje[] = "RPM";
	LCDGotoXY(4,0);
	LCDstring(mensaje, 3);
	
    /* Loop */
    while (1) 
    {
		// esperar por primer flanco
		while(estadoFlag()==0);
		t = ICR1;
		borrarFlag();
		
		// esperar por segundo flanco
		while(estadoFlag()==0);
		periodo = ICR1-t;
		borrarFlag();
		
		LCDGotoXY(0,0);
		uint8_t aux = CONVERSION / (periodo);
		LCDescribeDato(aux,3);
    }
	
	return 1;
}

static uint8_t estadoFlag()
{
	return TIFR1 & (1<<ICF1);
}

static void borrarFlag()
{
	TIFR1 = (1<<ICF1);
}
