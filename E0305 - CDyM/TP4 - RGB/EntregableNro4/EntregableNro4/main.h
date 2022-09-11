/*
 * main.h
 *
 * Created: 4/7/2022 15:03:28
 *  Author: Barcala
 */ 


#ifndef MAIN_H_
	#define MAIN_H_

	// ----------------------- Constantes ----------------
	
	#define F_CPU 16000000UL

	// -------------------------- Includes -----------------
	
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/sleep.h>
	#include <string.h>
	
	#include "reloj.h"
	#include "serialPort.h"
	#include "buffer.h"
	#include "led.h"
	#include "CLI.h"
	#include "adc.h"

#endif /* MAIN_H_ */