/**** A V R  A P P L I C A T I O N  NOTE 1 3 4 ************************** 
 *
 * Title:           Real Time Clock
 * Version:         2.00
 * Last Updated:    24.09.2013
 * Target:          ATmega128 (All AVR Devices with secondary external oscillator)
 *
 * Support E-mail:  avr@atmel.com
 *
 * Description      
 * This application note shows how to implement a Real Time Clock utilizing a secondary 
 * external oscilator. Included a test program that performs this function, which keeps
 * track of time, date, month, and year with auto leap-year configuration. 8 LEDs are used
 * to display the RTC. The 1st LED flashes every second, the next six represents the
 * minute, and the 8th LED represents the hour.
 *
 ******************************************************************************************/ 

#include "main.h"
/*
FUSES =
{
	.low = 0xE3,		//Select cpu clk: Internal calibrated RC-oscillator @ 4MHz
	.high = 0x99,		//Default settings
	.extended = 0xE3,	//Default settings
};
*/
static char not_leap(void);
static void init(void);

typedef struct{
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char date;
	unsigned char month;
	unsigned char year;
	}time;
	
volatile time t={55,59,23,31,12,99};
volatile uint8_t FlagLCD=0;

int main(void)
{
    init();	//Initialize registers and configure RTC.
	LCDinit();
	//LCDGotoXY(0,0);
	//LCDstring((uint8_t *)"Hola mundo",10);
	//LCDGotoXY(0,1);
	//LCDstring((uint8_t *)"DCyMC 2021    ",14);
	while(1)
	{
	//	sleep_mode();										//Enter sleep mode. (Will wake up from timer overflow interrupt)
	//	TCCR2B=(1<<CS20)|(1<<CS22);							//Write dummy value to control register
	//	while(ASSR&((1<<TCN2UB)|(1<<OCR2AUB)|(1<<TCR2AUB)));	//Wait until TC0 is updated
	PORTC=~PORTC;
	if(FlagLCD==1){
		FlagLCD=0;
		LCDGotoXY(4,0);
		LCDescribeDato(t.hour,2);
		LCDsendChar(':');
		LCDescribeDato(t.minute,2);
		LCDsendChar(':');
		LCDescribeDato(t.second,2);
		LCDGotoXY(4,1);
		LCDescribeDato(t.date,2);
		LCDsendChar('/');
		LCDescribeDato(t.month,2);
		LCDsendChar('/');
		LCDescribeDato(t.year,2);
		
		if(t.date == 1 && t.month == 1)
		{
			LCDGotoXY(13,0);
			uint8_t mensaje[3] = {'F', 'A', 'N'};
			LCDstring(mensaje,3);
		}
	 }
	}
}

static void init(void)
{
	//Wait for external clock crystal to stabilize;
/*	for (uint8_t i=0; i<0x40; i++)
	{
		for (int j=0; j<0xFFFF; j++);
	}*/
	DDRC = 0xFF;											//Configure all eight pins of port B as outputs
	TIMSK2 &= ~((1<<TOIE2)|(1<<OCIE2A));						//Make sure all TC0 interrupts are disabled
	ASSR |= (1<<AS2);										//set Timer/counter0 to be asynchronous from the CPU clock
															//with a second external clock (32,768kHz)driving it.								
	TCNT2 =0;												//Reset timer
	TCCR2B =(1<<CS20)|(1<<CS22);								//Prescale the timer to be clock source/128 to make it
															//exactly 1 second for every overflow to occur (count from 0 to 255, and then overflows)
	while (ASSR & ((1<<TCN2UB)|(1<<OCR2AUB)|(1<<TCR2AUB)))	//Wait until TC0 is updated
	{}
	TIMSK2 |= (1<<TOIE2);									//Set 8-bit Timer/Counter0 Overflow Interrupt Enable
	sei();													//Set the Global Interrupt Enable Bit
//	set_sleep_mode(SLEEP_MODE_PWR_SAVE);					//Selecting power save mode as the sleep mode to be used
//	sleep_enable();											//Enabling sleep mode
}

ISR(TIMER2_OVF_vect)
{
	if (++t.second == 60)        //keep track of time, date, month, and year
	{
		t.second=0;
		if (++t.minute==60)
		{
			t.minute=0;
			if (++t.hour==24)
			{
				t.hour=0;
				if (++t.date==32)
				{
					t.month++;
					t.date=1;
				}
				else if (t.date==31)
				{
					if ((t.month==4) || (t.month==6) || (t.month==9) || (t.month==11))
					{
						t.month++;
						t.date=1;
					}
				}
				else if (t.date==30)
				{
					if(t.month==2)
					{
						t.month++;
						t.date=1;
					}
				}
				else if (t.date==29)
				{
					if((t.month==2) && (not_leap()))
					{
						t.month++;
						t.date=1;
					}
				}
				if (t.month==13)
				{
					t.month=1;
					t.year++;
				}
			}
		}
	}
	PORTC=~(((t.second&0x01)|t.minute<<1)|t.hour<<7);
	FlagLCD=1;
}

static char not_leap(void)      //check for leap year
{
	if (!(t.year%100))
	{
		return (char)(t.year%400);
	}
	else
	{
		return (char)(t.year%4);
	}
}
