#include<stdio.h>
#include<stdlib.h>

int main(int argc, char*argv[]){
unsigned long N = atol(argv[1]);
unsigned long sum = 0;
int i;

	for(i=1;i<=N;i++){
		sum+=i;
	}
	
	if( sum == ((N*(N+1))/2) ){
		printf("Resultado Correcto\n");
	}else{
		printf("ERROR\n");
	}
		
	
}