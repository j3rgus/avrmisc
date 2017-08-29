#include <avr/io.h>

int main(void)
{
	DDRB |= (1 << PB4) | (1 << PB3);
	DDRD |= (1 << PD5) | (1 << PD4);
	PORTB |= (1 << PB4) | (1 << PB3);
	PORTD |= (1 << PD5) | (1 << PD4);
	
	while (1) {
		
		
	}

}