#include <stdio.h>
#include <stdlib.h>

void main();// __attribute__((no_instrument_function));

int fib(int _n);
void* pop() __attribute__((no_instrument_function));
void push(void *_return_address) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));

//void* main_calls[5];
//void* fib_calls[5];

struct truthful_returns
{
	void *return_address;
	struct truthful_returns *prev;
} *stack_ptr = NULL, *stack_help;

void push(void *_return_address) 
{
	struct truthful_returns *new_element = (struct truthful_returns*)malloc(sizeof(struct truthful_returns));
	new_element->return_address = _return_address;
	new_element->prev = stack_ptr;
	stack_ptr = new_element;
}

void* pop()
{
	void *temp_return_address;
	stack_help = stack_ptr;
	stack_ptr = stack_ptr->prev;
	temp_return_address = stack_help->return_address;
	free(stack_help);
	return temp_return_address;
}

void __cyg_profile_func_enter(void *this_fn, void *call_site) 
{
	printf("ENTER: %p, from %p\n", this_fn, call_site);

	/*
	if(call_site >= fib && call_site < main)
		if (this_fn != fib_calls[0])
			exit(0);
		else
			printf("*** correct enter ***\n");
	else if (call_site >= main)
		if (this_fn != main_calls[0])
			exit(0);
		else
			printf("*** correct enter ***\n");
	*/

	push(call_site);

}

void __cyg_profile_func_exit(void *this_fn, void *call_site) 
{
	void *temp_call_site = pop();
	printf("EXIT:  %p, from %p\n", this_fn, call_site);
	printf("run-time-return: %p | stored-return: %p\n", call_site, temp_call_site);

	//Return-Address-Check
	
	if (call_site == temp_call_site)
		printf("*** correct exit ***\n");
	else
		exit(0);
} 

int fib(int _n)
{
	return _n == 0 ? 0 : (_n == 1 ? 1 : fib(_n - 2) + fib(_n - 1));
}

void main() 
{
	//main_calls[0] = fib;
	//fib_calls[0] = fib;

	char buffer[8];
	gets(buffer);

	int n = buffer[0] - '0';

	if (n >= 0 && n <= 5)
		printf("Fibonacci: %d\n", fib(n));
}

/*
- Überlegung 1 - Kontrollflussverfolgung:

Um feststellen zu können, dass ein Programm zur Laufzeit an die richtige Rücksprungadresse springt,
muss ein Vergleich der aktuellen Rücksprungadresse und einer vorher gespeicherten call-site-address 
bzw. der Herkunftsadresse stattfinden.

Der Vergleich muss immer entweder als letzte Instruktion einer laufenden Funktion oder direkt vor dem 
Eintreten in eine neue Funktion stattfinden. Hierzu eignet sich die Funktion: __cyg_profile_func_exit()

Die Adressen sollten idealerweise mit Hilfe des Stack-Prinzips abgespeichert werden. 
Begründung: Es gibt genauso viele Herkunftsadressen wie Rücksprungadressen. Funktionen können 
weitere Funktionen aufrufen, usw. - Das bedeutet die zuletzt aufgerufene Funktion muss immer die 
zuletzt abgespeicherte Rücksprungadresse für den Vergleich verwenden. 

- Überlegung 2 - Kontrollfluss "hacken":

Um den implementierten Schutz umgehen zu können, muss man die Struktur dahinter kennen.
Beim Eintritt in die main()-Funktion wird die Herkunftsadresse in einen Stack abgespeichert. 
Diese Adresse muss verändert werden, da diese für den späteren Vergleich verwendet wird.

Idee für einen gets()-Input: 

##todo##

*/