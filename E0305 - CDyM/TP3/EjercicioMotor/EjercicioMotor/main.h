/*
 * main.h
 *
 * Created: 6/6/2022 15:02:49
 *  Author: Barcala
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define PREESCALER 8
#define F_CPU 16000000UL
#define F_IO (F_CPU / PREESCALER)
#define CONVERSION (60 * F_IO / 25)

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"


#endif /* MAIN_H_ */