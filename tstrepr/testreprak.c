#include <avr/io.h>
#include <util/delay.h>

#define xtal 4000000

int main(void)
{

  DDRB = 0xff;
  while (1) {
    PORTB = 0;
	_delay_us(250);
	PORTB = 1;
	_delay_us(250);
  }
}
