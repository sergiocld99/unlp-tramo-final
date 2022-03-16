/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdint.h>

char* convertir(uint8_t);

int main(void)
{
	
	// ejemplo
	
	char* ejemplo = convertir(179);
	printf("%s \n",ejemplo);
	// while(1);
	return 0;
}

char* convertir(uint8_t num){

	uint8_t unidad, decena, centena;
	
	unidad = num%10;
	num = num/10;
	decena = num%10;
	centena = num/10;
	
    static char aux[4];
    aux[0] = centena + '0';
    aux[1] = decena + '0';
    aux[2] = unidad + '0';
    aux[3] = '\0';
    return aux;
}

