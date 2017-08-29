#include <avr\io.h>
#include <util/delay.h>

void USART_Transmit(char data);
char USART_Receive(void);
void USART_Init(void);


int main()
{
  USART_Init();
  char c = 0x30;

  while (1) {

    USART_Transmit(USART_Receive());
  }
}

void USART_Transmit(char data)
{
  asm volatile (
  	"12:"			"\n\t"
    "sbis 0x0B,5"	"\n\t"
	"rjmp 12b"		"\n\t"
	"out 0x0c,%0"	"\n\t"
	:
	:"r" (data)
  );
}

char USART_Receive(void)
{
  char l_data;
  asm volatile (
    "15:"			"\n\t"
    "sbis 0x0B,7"	"\n\t"
	"rjmp 15b"		"\n\t"
	"in %0,0x0C"	"\n\t"
	:"=&r" (l_data)
	:
  );
  return l_data;
}

/* 19200kbps 8N1 */
void USART_Init(void)
{
  asm volatile (
   	"clr r16"		"\n\t"
	"out 0x0B,r16"	"\n\t"
	"out 0x20,r16"	"\n\t"
	"ldi r16,0x18"	"\n\t"
	"out 0x0A,r16"	"\n\t"
	"ldi r16,0x86"	"\n\t"
	"out 0x20,r16"	"\n\t"
	"ldi r16,0x0c"	"\n\t"
	"out 0x09,r16"	"\n\t"
  );
}
