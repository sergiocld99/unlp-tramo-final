/*
 * parser.h
 *
 * Created: 30/7/2022 03:19:31
 *  Author: Calderón Sergio
 */ 


#ifndef PARSER_H_
	#define PARSER_H_

	// -------------------------- Includes ----------------------------
	
	// Utilidades con Strings
	#include <string.h>
	
	// Tipos de datos enteros estandarizados
	#include <stdint.h>

	// ------------------------- Constantes -------------------------
	
	#define OK 1
	#define ERROR_CAST 2
	#define ERROR_PARAM 3

	// -------------------- Prototipos de funciones públicas --------------
	
	// Compara dos textos y retorna 1 si coinciden, sensible a mayúsculas y minúsculas
	uint8_t PARSER_Equals(const char* s1, const char* s2);
	
	// Separa el texto pasado por parámetro de acuerdo al primer blanco
	void PARSER_Split(const char* texto, char* s1, char* s2);
	
	// Extrae 3 números enteros y positivos de un texto pasado por parámetro
	// Si contiene menos de 3 palabras retorna un error de parámetros insuficientes.
	// Si el contenido no son números enteros y positivos retorna un error de cast
	uint8_t PARSER_Convert(char* texto, uint8_t* n1, uint8_t* n2, uint8_t* n3);

#endif /* PARSER_H_ */