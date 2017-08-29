#define F_CPU 4800000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "74hc164.h"

unsigned char adc[2];

/*
Analog/digital converter 
vyuzivaju sa tu ADCSRA, ADCSRB, ADMUX
ADCSRA
-ADEN - povoli samotny convertor (nutne nastavit pred ADSC)
-ADSC - spusti konvertor (po skonvertovani nastavi spat na nulu, pricom je tu moznost to riadit rucne alebo prerusovacou rutinou),
nastavenie ADSC teda spusti rutinu v oboch modoch
-ADATE - pri nastaveni prebieha automaticka konvertacia na nabeznu hranu (zdroj zalezi od nastavenia ADTS v ADCSRB). Automaticky nastavi ADSC.
-ADIF - nastavene ked konvertacia bola hotova
-ADIE - povoli prerusenie pre ADC
-ADPS - delic frekvencie (50-200kHz je optimalne)

ADCSRB
-ADTS - Zdroj pre ADATE, pri nabeznej hrane ktorejkolvek moznosti je zavolane prerusenie

ADMUX
-REFS0 - VCC referencia pre ADC
-ADLAR - (pokial nie je citane ADCH, nie je mozne prepisat hodnotu v ADCH), ADLAR vymeni ADCH a ADCL medzi sebou tak, ze spodnych 8 bitov bude ulozenych v ADCH, pricom vrchne dva bity v ADCL sa stanu nepotrebnymi
-MUX - nastavenie kanalov 
*/

ISR(ADC_vect)
{
	adc[0] = ADCL;
	adc[1] = ADCH;
}

int main(void)
{
	DDRB &= ~(1 << DDB2);
	hc164_init();

	ADMUX |= (1 << MUX0);
	ADCSRA |= ((1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN));
	asm("sei");
	ADCSRA |= (1 << ADSC);
	while (1) {
	//	ADCSRA |= (1 << ADSC);
	//	while (ADCSRA & (1 << ADSC))
	// 	 ;
		hc164_string(adc, 2);
		_delay_ms(2000);
	}
	return 0;
}
