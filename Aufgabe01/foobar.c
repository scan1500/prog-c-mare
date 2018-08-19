#include <stdio.h>

int foo();
int bar();

const int a = 42;
int b = 43, c, result = 0;
int i = 0;

int main()
{
	for (;i < 5; i++)
		result += (foo() + bar());

	return result;
}

int foo()
{
	static int counter = 0;
	return counter++;
}

int bar()
{
	static int counter = 0;
	return counter++;
}