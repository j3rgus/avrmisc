#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect)
    {
        PORTB = 42;
    }

int main(void)
{
  TCCR0=0x05;
  TCNT0=1;
  TIMSK=0x01;
  asm("sei");
  while (1) {
  }
}
