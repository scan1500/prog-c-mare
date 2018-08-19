#include <stdio.h>
#include <string.h>

void overflow_function()
{
	printf("HACKED!!\n");
	system("/bin/date");
}

void silly_function()
{
	char buffer[17];
	gets(buffer);
}

void main()
{
	printf("Adress of overflow_function: %p\n", overflow_function);
	silly_function();
}