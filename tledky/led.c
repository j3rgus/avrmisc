#include <avr/io.h>
#include <util/delay.h>
#define _sviet  {PORTD = 3;}
#define _zhasni {PORTD = 0;}

int main(void)
{
  DDRD = 0xff;

  while (1) {
    _sviet;
	_delay_ms(70);
	_zhasni;
	_delay_ms(110);
	_sviet;
	_delay_ms(70);
	_zhasni;
	_delay_ms(110);
	_sviet;
	_delay_ms(70);
	_zhasni;
	_delay_ms(1000);
  }
}
