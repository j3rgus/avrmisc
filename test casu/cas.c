#include <avr/io.h>

void delay(unsigned char ms);


int main(void)
{
  delay(3);
  PORTB = 0;
  while (1) { 
  
  }
}

void delay(unsigned char ms)
{
asm volatile (
  	"clr r16"	  "\n\t"
	"clr r17"	  "\n\t"
	"mov r18,%0"  "\n\t"
    "1:" 		  "\n\t"
	"inc r16"  	  "\n\t"
	"cpi r16,250"  "\n\t"
	"brne 1b"	  "\n\t"
	"inc r17"	  "\n\t"
	"cpi r17,200"  "\n\t"
	"brne 1b"	  "\n\t"
	:
	: "r" (ms)
);
}
