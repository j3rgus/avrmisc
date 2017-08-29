#include <avr\io.h>
#include <avr\interrupt.h>

#define XTAL 8000000UL

ISR(TIMER0_OVF_vect) 
{
  DDRD ^= 0xff;
  TCNT0=0x00;
}

int main()
{
  TCCR0A=0x00;
  TCCR0B=0x03;
  TCNT0=0x00;
  TIMSK0=0x01;

  asm("sei");
  while (1) {
  
  }
}
