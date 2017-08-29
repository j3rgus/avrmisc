#include <avr/io.h>
#include "enc28j60.h"

int main(void) 
{
	DDRD |= (1 << DD7);
	PORTD |= (1 << PD7);
	while (1) {
	}
}