/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdint.h>     // tipos estandarizados de enteros

// Prototipos
void mostrarBits(int8_t);

// uint8_t permite representar hasta 255, y 255^2 es menor a 2^16
// entonces con 16 bits nos alcanza y sobra para retornar el resultado
uint16_t sumarHasta(uint8_t);  

// Secuencia principal
int main()
{
    mostrarBits(0x5A);
    printf("%d \n", sumarHasta(10));
    return 0;
}

// Funciones
void mostrarBits(int8_t N){
    for (uint8_t i=0; i<8; i++){
        // se aplica una máscara para mostrar sólo el bit menos significativo
        printf("%d \n", N&0x01);
        
        // desplazamiento de bits hacia la derecha (>>= lo asigna a N)
        N >>= 1;
        
        // se espera 1 segundo
        sleep(1);
    }
}

uint16_t sumarHasta(uint8_t N){
    uint16_t resultado = 0;
    
    for (uint8_t i=1; i<=N; i++){
        resultado += i;
    }
    
    return resultado;
}