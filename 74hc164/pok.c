#define F_CPU 8000000UL

#include <avr/io.h>
#include "74hc164.h"
#include "ds1820/"

ISR(TIMER0_OVF_vect) 
{
	TIMSK = 0;

	TIMSK = 0x02;
}

int main(void)
{
 	unsigned char d[4];
	
	TCCR0A=0x00;
  TCCR0B=0x04;
  TCNT0=0x00;
  TIMSK=0x02;

	hc164_init();
	while (1) {
		
	}
}
