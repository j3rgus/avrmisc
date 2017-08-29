#include <util/delay.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include "lcd_lib.h"
#include "lcd_lib.c"

#define EEPROM_CAKAJ while (!eeprom_is_ready()) 

int testuj(int n)						//1 - vynulovany
{
	if (bit_is_set(PIND,n)) 
		return 0;
	else
		return 1;
}

void zapis(char p[4])
{
	int i;
	for (i = 0; i < 4; i++) {
		EEPROM_CAKAJ;
		eeprom_write_byte(i,p[i]);
	}
}

int nastavene(void)
{
	int i;

	for (i = 0; i < 4; i++) {
		EEPROM_CAKAJ;
		if (eeprom_read_byte(i) == 0xff)
			return 0;
	}
	return 1;
}

void cisti_eeprom(void)
{
	int i;

	for (i = 0; i < 4; i++) {
		EEPROM_CAKAJ;
		eeprom_write_byte(i,0xff);
	}
}

int je_rovny(char p[4]) 
{
	int i;

	for (i = 0; i < 4; i++) {
		EEPROM_CAKAJ;
		if (eeprom_read_byte(i) != p[i]) {
			LCDGotoXY(0,1);
			LCDstring("Chybne", 6);
			_delay_ms(200);
			return 0;
		}
	}
	LCDGotoXY(0,1);
	cisti_eeprom();
	LCDstring("Spravne", 7);
	_delay_ms(200);
	return 1;
}

void cisti_displej(int x, int y)
{
	LCDGotoXY(x,y);
	LCDstring("        ", 8);
}

void hviezdicky (int n)
{
	if (n <= 4) {
		LCDGotoXY(n-1,1);
		LCDstring("*", 1);
	}
}

int main(void)
{
	char heslo[4];
	int i = 0, n;					//0 citanie, 1 zapis

	DDRD = 0x00;
	LCDinit();
	LCDclr();
	LCDcursorOFF();
spat:
	
	if ((n = nastavene()) == 0) {
		LCDclr();
		LCDGotoXY(0,0);
		LCDstring("Nastav:", 7);
	}
	else {
		LCDclr();
		LCDGotoXY(0,0);
		LCDstring("Vloz:", 5);
	}
		
	LCDGotoXY(0,1);
	while (1) {
			if (bit_is_set(PINA,2)) {
				test1:
				if (testuj(0)) {
					heslo[i++] = 1;
					hviezdicky(i);
					goto pokracuj;;
				}
				else goto test1;
			}
			if (bit_is_set(PIND,0)) {
				test2:
				if (testuj(1)) {
					heslo[i++] = 2;
					hviezdicky(i);
					goto pokracuj;;
				}
				else goto test2;
			}
			if (bit_is_set(PIND,1)) {
				test3:
				if (testuj(2)) {
					heslo[i++] = 4;
					hviezdicky(i);
					goto pokracuj;;
				}
				else goto test3;
			}
			if (bit_is_set(PINA,1)) {
				test4:
				if (testuj(3)) {
					heslo[i++] = 8;
					hviezdicky(i);
					goto pokracuj;
				}
				else goto test4;
			}
			pokracuj:
			if (i >= 4) {
				i = 0;
				if (n == 0) {
					zapis(heslo);
					goto spat;
				}
				else 
					if (je_rovny(heslo) == 1) {
						goto spat;
					}	
					else cisti_displej(0,1);
			}
	}
	return 0;

}
