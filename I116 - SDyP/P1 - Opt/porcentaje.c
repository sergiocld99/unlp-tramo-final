#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[]){

	int p= atoi(argv[1]);
	int n= atoi(argv[2]);
	int x = n*p/100;
	printf("El %d%% de %d es %d\n",p,n,x);
	return 0;
}
