#include <avr\io.h>
#include <avr\interrupt.h>

void doit(void);
void doit1(void);

char status;

ISR(TIMER0_OVF_vect) 
{
  if ((status & 1) == 1)
    doit();
  if ((status & 2) == 2)
    doit1();
}

void doit(void)
{
  DDRD ^= 0xff;
}

void doit1(void)
{
  DDRD ^= 0xff;
}

int main()
{
  TCCR0A=0x00;
  TCCR0B=0x05;
  TCNT0=0x00;
  TIMSK0=0x01;

  asm("sei");
  status = 3;
  while (1) {
  //status = 4;
  //status = 5;
  //status = 6;
  //status = 7;
  //status = 8;
  //status = 9;
  //status = 10;
  }
}
