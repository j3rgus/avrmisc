#include <avr\io.h>
#include <avr\interrupt.h>
#include "ds1820\ds1820.h"
#include <util\delay.h>

#define F_CPU 4000000UL

void USART_Transmit(uchar data);
uchar USART_Receive(void);
void USART_Init(void);

int main()
{
  int i;
  uchar lsb, msb;

  USART_Init();
  while (1) {
    one_reset();
	send_convert();
	_delay_ms(200);
	_delay_ms(200);
	_delay_ms(200);
	one_reset();
	send_getTemp();
	lsb = one_read_byte();
	//msb = one_read_byte();
	one_reset();
	for (i = 0; i < 8; i++) {
	  USART_Transmit((lsb & 1)+'0');
	  lsb >>= 1;
	}
	USART_Transmit(' ');
  }
}

void USART_Transmit(uchar data)
{
  while ( !( UCSRA & (1<<UDRE)) )
    ;
  UDR = data;
}

uchar USART_Receive(void)
{
  while ( !(UCSRA & (1<<RXC)) )
    ;
  return UDR;
}

void USART_Init(void)
{
  UBRRH = 0;
  UBRRL = 12;		/* 19200kbps 8N1 */
  UCSRB = 0x18;
  UCSRC = 0x06;
}
