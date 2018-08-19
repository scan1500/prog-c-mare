#include <stdio.h>

void main(int argc, char *argv[])
{
	int i;
	for (i = 14; i >= 0; i-=2)
		printf("\\x%c%c", argv[1][i],argv[1][i+1]);
	printf("\n");
}