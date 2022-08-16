/*
 * serialPort.h
 *
 * Created: 07/10/2020 03:02:42
 * Author: Calderón Sergio
 */ 

#ifndef SERIALPORT_H_
	#define SERIALPORT_H_

	// ------------------- Includes ----------------------------
	
	// Archivo de cabecera del Microcontrolador
	#include <avr/io.h>
	
	// -------- Prototipos de funciones Publicas ---------------
	
	// Inicializacion de Puerto Serie
	void SerialPort_Init(uint32_t);

	// Inicializacion de Transmisor
	void SerialPort_TX_Enable(void);
	void SerialPort_TX_Interrupt_Enable(void);
	void SerialPort_TX_Interrupt_Disable(void);
	
	// Inicializacion de Receptor
	void SerialPort_RX_Enable(void);
	void SerialPort_RX_Interrupt_Enable(void);
	
	// Transmision de un dato
	void SerialPort_Send_Data(uint8_t);

	// Recepcion de un dato
	uint8_t SerialPort_Receive_Data(void);
	
#endif /* SERIALPORT_H_ */