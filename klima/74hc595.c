/*
*  DS     -  PB0
*  SH_CP  -  PB1
*  ST_CP  -  PB2
*/
#include <avr/io.h>

#define DS 0
#define SH_CP 1
#define ST_CP 2

#define SegA 1
#define SegB 2
#define SegC 4
#define SegD 8
#define SegE 16
#define SegF 32
#define SegG 64
#define SegH 128

void init_595(void)
{
  DDRB |= ((1 << DS) | (1 << SH_CP) | (1 << ST_CP));
  PORTB &= ~((1 << DS) | (1 << SH_CP) | (1 << ST_CP));
}

void zapis_seg(unsigned char *d, int l)
{
  unsigned char j, i;

  for (i = (l-1); (i < 5) && (i >= 0); i--) {
    for (j = 0; j < 8; j++) {
      if ((d[i] & 128)) 
	PORTB |= (1 << DS);
      else 
	PORTB &= ~(1 << DS);

       PORTB |= (1 << SH_CP);
       PORTB &= ~(1 << SH_CP);
       d[i] = d[i]<<1;
    }
  }
}

void zobraz_dis(void)
{
  PORTB |= (1 << ST_CP);
  PORTB &= ~(1 << ST_CP);
}

/*int main(void)
{
  unsigned char pom[4];

  init_595();

  pom[0] = 0x7f ^ SegD;
  pom[1] = 0x7f ^ SegA ^ SegD;
  pom[2] = 0x7f ^ SegG;
  pom[3] = 0x7f ^ SegF ^ SegG;
  zapis_seg(pom, 4);
  zobraz();
  while (1) {
  }
  
  return 0;
}*/
