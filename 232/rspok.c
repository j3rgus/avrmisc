#define F_CPU 4000000

#include <avr/io.h>
#include <avr/interrupt.h>

void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while ( !( UCSRA & (1<<UDRE)) )
;
/* Put data into buffer, sends the data */
UDR = data;
}

unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while ( !(UCSRA & (1<<RXC)) )
;
/* Get and return received data from buffer */
return UDR;
}

void USART_Init(void)
{
/* Set baud rate 19200 */
UBRRH = 0;
UBRRL = 12;
/* Enable receiver and transmitter */
UCSRB = 0x18;
/* Set frame format: 8N1 */
UCSRC = 0x06;
}

int main(void)
{
  USART_Init();

  USART_Transmit('a');
  while (1) {
    USART_Transmit(USART_Receive());
  }
}
