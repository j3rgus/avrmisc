//  PB -> |-|-|-|Senz2|Senz1|Dis_RCK|Dis_SCK|Dis_SER|

#define F_CPU 4000000
#define VONK_TEP 0b00001000
#define VNUT_TEP 0b00010000
#define BT1 0  // zobrazenie vonk. teploty / potvrdenie
#define BT2 1  // ++
#define BT3 2  // --
#define OUT 5  //vystup
#define IN 4   //vstup
#define SPK 3  //speaker

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "_ds1820.h"
#include "74hc595.c"
#include "moje.c"

ISR(TIMER0_OVF_vect)
{
  casovac++;
  if (!flags.tep) {
    flags.tep = 1;
  } 
  if (flags.tep && (casovac % 11) == 0) {
    flags.init = 1;
  }
  if (!flags.nastav && bit_is_set(PIND, BT1)) {
    flags.btn1 = 1;       // pouzite pre zobraz_teploty()
    stopky = casovac + 70;
  }
  if (!flags.mrzlo && flags.start && (teploty.von_H || (!teploty.von_L && !teploty.von_H))) {
    flags.mrzne = 1;
  }
  if (flags.mrzne) {
    if ((casovac % 10) == 0) {
      PORTD ^= (1 << SPK);
      scitac++;
    }
    if (scitac >= 6) {
      flags.mrzne = 0;
      flags.mrzlo = 1;
      scitac = 0;
    }
  }
  if (flags.mrzlo) {
    if (!teploty.von_H && (teploty.von_L >= 4))
      flags.mrzlo = 0;
  }
}

int main(void)
{
  DDRD = 0b0101000;	// PD -> |-|OUT|IN|SP|BT3|BT2|BT1|
  PORTD &= ~0b0101000;

  init_595();
  flags.tep = 0;
  flags.init = 0;
  flags.start = 0;
  teploty.alarm = 46;      // nastavenie teploty na 23°C pri spusteni
  teploty.alarm_sign = 0;
  nastav_casovac();

  while (1) {
    if (flags.tep) {
      convert_tep();
    }
    if (flags.init) {
      citaj_tep();
      flags.init = 0; flags.tep = 0; flags.start = 1;
    }
    if (flags.start)
      zobraz_teploty();
    if (bit_is_set(PIND, BT2) || bit_is_set(PIND, BT3)) {
      _delay_ms(150);
      nastav();
    }
    if (bit_is_set(PIND, IN)) {
      if ((teploty.alarm <= teploty.vnut_L) && (teploty.alarm_sign == teploty.vnut_H)) {
        PORTD |= (1 << OUT);
      }
      if ((teploty.vnut_L <= (teploty.alarm - 6)) || flags.alarm) {  //klesne o 6/2 = 3°C alebo sa prenastavi
        PORTD &= ~(1 << OUT);
        flags.alarm = 0;
      }
    } 
    else PORTD &= ~(1 << OUT);
  }
}
