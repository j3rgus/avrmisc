#include <avr/io.h>

void delay()
{
asm volatile (
			"ldi r16,0"	"\n\t"
		"1:"			"\n\t"
			"inc r16"		"\n\t"
			"cpi r16,10"	"\n\t"
			"brne 1b"	"\n\t"
        );
}

int main()
{
DDRB = 0xff;
delay();
}
