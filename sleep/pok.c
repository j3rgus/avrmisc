#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT0_vect)
{
  MCUCR &= ~(1 << SE);
  PORTD |= (1 << PD6);
}

ISR(INT1_vect)
{
  MCUCR &= ~(1 << SE);
  PORTD |= (1 << PD6);
}

ISR(TIMER0_OVF_vect)
{
  MCUCR &= ~(1 << SE);
  PORTD ^= (1 << PD6);
}

int main(void) 
{
  unsigned char port;

  DDRD |= (1 << DDD5) | (1 << DDD6);	//nastavenie vystupu na PORTe D 5 a 6
  DDRB = 0;
  MCUCR |= (1 << ISC00) | (1 << ISC10); //INT1 a INT0 - ktorakolvek zmena na PINE (ISC01 a ISC02 su prazdne)
  GIMSK |= (1 << INT0) | (1 << INT1);	//Povolenie externeho prerusenia INT0 a INT1
  MCUCR &= ~((1 << SM0) | (1 << SM1));	//nastavenie IDLE modu (SM0 a SM1 budu prazdne-0)
  TCCR0A = 0;				//nastavenie casovaca na 64ms
  TCCR0B |= (1 << CS00) | (1 << CS02);
  TCNT0 = 250;
  //TIMSK |= (1 << TOIE0);		//spusti casovac
  asm("sei");				//zapne globalne prerusenie I-flag
  _delay_ms(3000);

  while (1) {
    if (PINB & (1 << PINB1)) {
      MCUCR |= (1 << SE);		//spusti sleep mode
      asm("sleep");
    }
    if (PINB & (1 << PINB0))
      PORTD |= (1 << PD5);
    else
      PORTD &= ~(1 << PD5);
  }
}
