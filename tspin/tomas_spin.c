#include <avr/io.h>
#include <util/delay.h>

void pipaj(void)
{
  PORTB |= 0b10000;
  _delay_ms(300);
  PORTB ^= 0b10000;
  _delay_ms(300);
  PORTB |= 0b10000;
  _delay_ms(300);
  PORTB ^= 0b10000;
  _delay_ms(100);
  while (bit_is_clear(PINB, 3))
    ;
  PORTB |= 0b10000;
  _delay_ms(600);
  PORTB ^= 0b10000;

  _delay_ms(1000);
}

int main(void)
{
  //char stav;
  
  asm("sbi 0x17,4");
  asm("cbi 0x17,3");
  //stav = 0;
  while (1) {
    if (bit_is_clear(PINB, 3)) {
	  //stav ^= 0xff;
	  pipaj();
	}
  }
}
