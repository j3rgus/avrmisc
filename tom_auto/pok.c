#define F_CPU 600000UL

#include <avr/io.h>
#include <util/delay.h>

void delay(int count1, int count2)
{
  while (count1--) {
		while (count2--)
			;
	}
}

int main(void)
{
	DDRB = 0b10111;
	PORTB = 0;

	//PORTB = 0b001000;
	//_delay_ms(15000);
	//PORTB = 0;
	while (1) {
		if ((PINB & 0b10000))
			PORTB |= 0b1000;
	}
	return 0;
}
