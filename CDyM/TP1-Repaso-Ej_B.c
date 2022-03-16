/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdint.h>

void reordenar(int8_t*, uint8_t);

int main(void)
{
	
	// ejemplo
	int8_t ejemplo[] = {1, 2, 3, 4, 5, 6, 7, 8};
	reordenar(ejemplo, 8);
	
	// while(1);
	return 0;
}

void reordenar(int8_t* vector, uint8_t size){
	int8_t aux;
	
	for(uint8_t i=0; i<size/2; i++){
		aux = vector[size-i-1];
		vector[size-i-1] = vector[i];
		vector[i] = aux;
	}
	
	for(uint8_t i=0; i<size; i++){
	    printf("%d \n", vector[i]);
	}
}

