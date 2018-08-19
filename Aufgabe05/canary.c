#include <stdio.h>
#define pre_canary 4278848768

 void overflow_detected()
{
	printf("Bufferoverflow detected, program will abort...\n");
	exit(0);
}

void main()
{
	unsigned int canary = pre_canary;
	char buffer[8];
	gets(buffer);

	if ((canary ^ pre_canary) != 0)
		overflow_detected();

	int i;
	for (i = 0; i < sizeof(buffer) / sizeof(char); i++)
		if (buffer[i] == '\0')
			break;
		else if (i == 7)
			overflow_detected();
}