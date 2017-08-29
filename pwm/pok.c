#define F_CPU 4000000UL

#include <avr/io.h>

int main(void)
{
  DDRB|=(1<<PORTB3);
  TCCR1A=0xA1;
  OCR1A=220;
  TCCR1B=0x05;
  while (1) {
  }
  return 0;
}
