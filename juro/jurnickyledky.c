#include <avr\io.h>
#include <util\delay.h>
//#define F_CPU 8000000ul

int main()
{
  DDRD = 0xff;
  DDRB = 0xff;

  PORTB = 0xff;
  PORTD = 0xff;
  _delay_ms(1000L);
  PORTB = 0;
  PORTD = 0;
  _delay_ms(1000L);
  for (;;) {
    PORTD = 0b0001001;
	_delay_ms(100L);
	PORTD = 0b0011000;
	_delay_ms(100L);
	PORTD = 0b0110000;
	_delay_ms(100L);
	PORTD = 0b1100000;
	_delay_ms(100L);
	PORTD = 0b1000000; PORTB = 0b0000001;
	_delay_ms(100L);
	PORTD = 0; PORTB = 0b0000011;
	_delay_ms(100L);
	PORTB = 0b0000110;
	_delay_ms(100L);
	PORTB = 0b0001100;
	_delay_ms(100L);
	PORTB = 0b0011000;
    _delay_ms(100L);

    PORTB = 0b0001100;
	_delay_ms(100L);
    PORTB = 0b0000110;
	_delay_ms(100L);
	PORTB = 0b0000011;
	_delay_ms(100L);
	PORTB = 0b0000001; PORTD = 0b1000000;
	_delay_ms(100L);
	PORTB = 0; PORTD = 0b1100000;
	_delay_ms(100L);
    PORTD = 0b0110000;
	_delay_ms(100L);
	PORTD = 0b0011000;
	_delay_ms(100L);
  } 
}
