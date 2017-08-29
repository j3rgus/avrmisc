#include <avr\io.h>

unsigned char p = 0, tabulka[] = {0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b};
int i = 0;

int main()
{
  char i;

  PORTD = 0x6f;
  for (i = 0; i < 8; i++) {
  	PORTB <<= 1;
    if ((PORTD & 0x80) == 0x80)
	  PORTB |= 1;
	else PORTB |= 0;

	PORTD <<= 1;
  }
}

/*	asm volatile (
			"ldi r16,0"	"\n\t"
			"ldi r17,0"	"\n\t"
			"ldi r18,0"	"\n\t"
		"1:	inc r16"	"\n\t"
			"cpi r16,230""\n\t"
			"brne 1b"	"\n\t"
			"inc r17"	"\n\t"
			"cpi r17,150""\n\t"
			"brne 1b"	"\n\t"
			"inc r18"	"\n\t"
			"cpi r18,16""\n\t"
			"brne 1b"
	);*/

