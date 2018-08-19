#include <stdio.h>
#include <string.h>

void overflow_function()
{
	printf("HACKED!!\n");
	system("/bin/date");
}

void silly_function(char *src)
{
	char buffer[8];
	strcpy(buffer, src);
}

void main(int argc, char *argv[])
{
	printf("Adress of overflow_function: %p\n", overflow_function);
	silly_function(argv[1]);
}