#include <avr/io.h>
#include <util/delay.h>

void USART_transmit(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
		;
	/* Put data into buffer, sends the data */
	UDR = data;
}

uint8_t USART_receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
		;
	/* Get and return received data from buffer */
	return UDR;
}

void USART_init(void)
{
	/* Set baud rate 2400 - 4Mhz crystal */
	UBRRH = 0;
	UBRRL = 103;
	/* Enable receiver and transmitter */
	UCSRB = 0x18;
	/* Set frame format: 8N1 */
	UCSRC = 0x06;
}

void delay(int count)
{
	int i;
	for (i = 1; i <= count; i++)
		_delay_ms(100);
}

int main(void)
{
 	uint8_t dat;
	
 	DDRD |= (1 << PD5);
	USART_init();
// 	while (1) {
// 		USART_transmit(0x55);
// 	}
	while (1) {
		dat = USART_receive();
		if (dat == 0x55) {
			PORTD |= (1 << PD5);
		} 
		else {
			PORTD &= ~(1 << PD5);
		}
	}
	return 0;
}