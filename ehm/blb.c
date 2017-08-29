#include <avr\io.h>
#include <avr/eeprom.h>
#include "ds1820\ds1820.h"
#include "ds1307\ds1307.h"
#include "lcd/lcd_lib.h"
#include <stdio.h>
#include <string.h>

void refresh_udaje(void);
void display1(void);
void CHbit_nastav(void);
void CHbit_nuluj(void);
char decToBcd(char d);
char bcdToDec(char b);
void zisti_den(void);

struct {
	char teplota;
	char sek;
	char min;
	char hod;
	char den;
	char mes;
	char rok;
	char mDen;
	char temp_flag;	//+-°C
	char mon_temp;
} udaje;


char *dni[] = {"Po ", "Ut ", "Str", "Stv", "Pia", "So ", "Ne "};
char lcd_dis[2][16], konf[16], val;
char *str_pis[] = {"Sek: %02x (0-%x) ","Min: %02x (0-%x) ", "Hod: %02x (0-%x) ", "Den: %02x (1-%x) ", "Mes: %02x (1-%x) ", "Rok: %02x (0-%x) "};
char *p_udaje[] = {&udaje.sek, &udaje.min, &udaje.hod, &udaje.mDen, &udaje.den, &udaje.mes, &udaje.rok};
char pct_dni[] = {0x31, 0x28, 0x31, 0x30, 0x31, 0x30, 0x31, 0x31, 0x30, 0x31, 0x30, 0x31};

int main(void)
{

  
  while (1) {
    zisti_den();
	    
  } 
}


void refresh_udaje(void)
{
  //one_reset();
  //send_convert();
  for (unsigned char i = 0; i < 7; i++)
    *(p_udaje[i]) = read_ds1307(i);
  sprintf(lcd_dis[1], "%3s  %2x.%2x. 20%02x", dni[udaje.mDen - 1], udaje.den, udaje.mes, udaje.rok);
  sprintf(lcd_dis[0], "%2x:%02x:%02x%+6d", udaje.hod, udaje.min, udaje.sek, udaje.teplota);
  _delay_ms(200);
  _delay_ms(200);
  one_reset();
  send_getTemp();
  udaje.teplota = one_read_byte();
  udaje.temp_flag = one_read_byte();
  one_reset();
  if ((udaje.temp_flag = 0))
    udaje.teplota >>= 1;
  else 
    udaje.teplota = -(((~udaje.teplota)/2)+1);
}

void display1(void)
{
  refresh_udaje();
  LCDhome();
  LCDstring(lcd_dis[0], strlen(lcd_dis[0]));
  LCDsendChar(0xdf);LCDsendChar('C');
  LCDGotoXY(0,1);
  LCDstring(lcd_dis[1], strlen(lcd_dis[1]));
}


void CHbit_nastav(void)
{
  udaje.sek = read_ds1307(0);
  write_ds1307(0, (udaje.sek|0x80));
}

void CHbit_nuluj(void)
{
  udaje.sek = read_ds1307(0);
  write_ds1307(0, (udaje.sek&0x7f));
}

char decToBcd(char d)
{
  return (((d/10)<<4) + (d%10));
}

char bcdToDec(char b)
{
  return (((b>>4)*10) + ((b&0x0f)));
}

void zisti_den(void)
{
  unsigned char d = 0, j = 0, i = 0;
  char c;
  int c_den = 0;

  c = bcdToDec(udaje.rok);
  c = 9;
  while (( c -= 4) > 0)
    i++;
  d = bcdToDec(udaje.rok) + i;
  for (i = 6 + d; i > 7; i -= 7)
    ;
  if (udaje.rok == 0)
    i = 6;
  else i--;
  for (j = 0; j < bcdToDec(udaje.mes)-1; j++)
    c_den +=  (unsigned char) bcdToDec(pct_dni[j]);
  c_den += (unsigned char) bcdToDec(udaje.den);
  c_den += i;
  for (; bcdToDec(c_den) > 7; c_den -= 7)
    ;
  udaje.mDen = (char) c_den;
}
