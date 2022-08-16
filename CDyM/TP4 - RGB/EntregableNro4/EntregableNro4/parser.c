/*
 * parser.c
 *
 * Created: 30/7/2022 03:30:39
 *  Author: Calderón Sergio
 */ 

// Archivo de cabecera
#include "parser.h"

// Prototipos de funciones privadas
static uint8_t split(char* origen, char* pal1, char* pal2, char* pal3);
static uint8_t getWord(char* origen, char* dest, uint8_t startIndex);
static uint8_t toNumber(char* palabra, uint8_t* numero);
static uint8_t toDigit(char);

// --------------------- Implementación de funciones públicas ----------------

uint8_t PARSER_Equals(const char* s1, const char* s2)
{
	return strcmp(s1, s2) == 0;
}

void PARSER_Split(const char* texto, char* s1, char* s2)
{
	uint8_t i=0, j=0, max=strlen(texto);
	
	// Extraer primera parte
	while (i < max && texto[i] != ' ')
	{
		s1[j] = texto[i];
		i++;
		j++;
	}
	
	// Agregar final de cadena a la primera parte
	s1[j] = '\0';
	
	// Ignorar espacio
	i++;
	
	// Reiniciar indice para la segunda parte
	j = 0;
	
	// Extraer segunda parte	
	while (i < max)
	{
		s2[j] = texto[i];
		i++;
		j++;
	}
	
	// Agregar final de cadena a la segunda parte
	s2[j] = '\0';
}

uint8_t PARSER_Convert(char* texto, uint8_t* n1, uint8_t* n2, uint8_t* n3)
{
	char pal1[32], pal2[32], pal3[32];
	
	// Separar el texto en 3 palabras (delimitador ' ')
	uint8_t estado = split(texto, pal1, pal2, pal3);
	if (estado != OK) return estado;
	
	// Convertir primer palabra a numero
	estado = toNumber(pal1, n1);
	if (estado != OK) return estado;
	
	// Convertir segunda palabra a numero
	estado = toNumber(pal2, n2);
	if (estado != OK) return estado;
	
	// Convertir tercera palabra a numero
	estado = toNumber(pal3, n3);
	
	// Retornar estado (operación exitosa o error)
	return estado;
}

// ------------------- Implementación de funciones privadas ----------------

// Separa un texto de origen en 3 palabras (en reemplazo a strtok)
uint8_t split(char* origen, char* pal1, char* pal2, char* pal3)
{
	uint8_t i, length;
	
	length = strlen(origen);
	if (length == 0) return ERROR_PARAM;
	
	i = getWord(origen, pal1, 0);
	if (i >= length) return ERROR_PARAM;
	
	i = getWord(origen, pal2, i+1);
	if (i >= length) return ERROR_PARAM;
	
	getWord(origen, pal3, i+1);
	return OK;
}

// Obtiene una palabra a partir de un texto e índice de origen
uint8_t getWord(char* origen, char* dest, uint8_t startIndex)
{
	uint8_t j=0, i=startIndex;
	
	while (origen[i] != '\0' && origen[i] != ' ')
	{
		dest[j] = origen[i];
		i++;
		j++;
	}
	
	dest[j] = '\0';
	return i;
}

// Convierte un número en formato texto a entero positivo de 8 bits
static uint8_t toNumber(char* palabra, uint8_t* numero)
{
	uint8_t digito, factor = 1;
	*numero = 0;
	
	// Se comienza a armar el número desde la unidad
	for (int8_t i=strlen(palabra)-1; i>=0; i--)
	{
		// Obtener digito
		digito = toDigit(palabra[i]);
		
		// Si el caracter no es valido, retornar que no es número
		if (digito > 9) return ERROR_CAST;
		
		// Sumar al resultado
		(*numero) += digito * factor;
		
		// Actualizar factor (x10)
		factor *= 10;
	}
	
	// Retornar conversión exitosa
	return OK;
}

// Convierte un caracter a su digito correspondiente
static uint8_t toDigit(char c)
{
	return c - '0';
}
