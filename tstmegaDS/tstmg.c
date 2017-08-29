#include <avr\io.h>
#include <avr/eeprom.h>
#include "ds1820\ds1820.h"
#include "ds1307\ds1307.h"
#include "lcd/lcd_lib.h"
#include <stdio.h>
#include <string.h>

#define F_CPU 4000000UL

void USART_Transmit(char data4);
char USART_Receive(void);
void USART_Init(void);
void USART_Print(char str[]);
void refresh_udaje(void);
void display1(void);
void CHbit_nastav(void);
void CHbit_nuluj(void);
char decToBcd(char d);
char bcdToDec(char b);
void cakaj(char p);
void konfig_hodiny(void);
void nastav(char *p_udaj, char str_ud[], char val, char addr);
void nastav1(void);

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

  USART_Init();
  LCDinit();
  LCDcursorOFF();
  DDRC = 0x00;
  udaje.teplota = 84;
  CHbit_nuluj();

  while (1) {
    display1();
	if (bit_is_set(PINC, 0))
	  konfig_hodiny();
	//if (udaje.mon_temp = 1)
	  //if (bit_is_set(PINC, 1))
	    
	    
  } 
}


void refresh_udaje(void)
{
  one_reset();
  send_convert();
  for (unsigned char i = 0; i < 7; i++)
    *(p_udaje[i]) = read_ds1307(i);
  sprintf(lcd_dis[1], "%3s  %2x.%2x. 20%02x", dni[udaje.mDen - 1], udaje.den, udaje.mes, udaje.rok);
  sprintf(lcd_dis[0], "%2x:%02x:%02x%+6d", udaje.hod, udaje.min, udaje.sek, udaje.teplota);
  //_delay_ms(100);
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

void konfig_hodiny(void)
{
  unsigned char pom;
  LCDclr();
  cakaj(0);
  LCDhome();
  LCDstring("KONFIGURACIA", 12);
  nastav(p_udaje[2], str_pis[2], 0x23, 2);
  nastav(p_udaje[1], str_pis[1], 0x59, 1);
  nastav(p_udaje[0], str_pis[0], 0x59, 0);
  nastav(p_udaje[6], str_pis[5], 0x99, 6);
  nastav(p_udaje[5], str_pis[4], 0x12, 5);
  pom = pct_dni[(unsigned char)bcdToDec(udaje.mes-1)];
  if ((udaje.mes == 2) && ((udaje.rok % 4) == 0))
    pom++;
  nastav(p_udaje[4], str_pis[3], pom, 4);
  nastav1();
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

void nastav(char *p_udaj, char str_ud[], char val, char addr)
{
  while (1) {
  	LCDGotoXY(0,1);
    sprintf(konf, str_ud, (*p_udaj), val);
	LCDstring(konf, strlen(konf));
	if (bit_is_set(PINC, 1)) {
	  if ((*p_udaj) < val) {
	    (*p_udaj)++;
	    if ((bcdToDec(*p_udaj) % 10) == 0)
		  (*p_udaj) += 6;
	  } else (*p_udaj) = ((addr == 5)||(addr == 4)) ? 1 : 0;
	  _delay_ms(100);
	}
	if (bit_is_set(PINC, 0)) {
	  write_ds1307(addr, (*p_udaj));
	  cakaj(0);
	  break;
	}
  }
}

void nastav1(void)
{
  cakaj(0);
  LCDclr();
  LCDhome();
  LCDstring("Snimat teplotu", 14);
  while (1) {
    LCDGotoXY(0,1);
    if (udaje.mon_temp == 0)
      LCDstring("Vypnute", 7);
    else LCDstring("Zapnute", 7);
	if (bit_is_set(PINC, 1)) {
	  udaje.mon_temp ^= 1;
	  LCDGotoXY(0,1); LCDstring("       ", 7);
	  _delay_ms(200);
	}
	if (bit_is_set(PINC, 0)) {
	  cakaj(0);
	  break;
	}
  }
}

/*void zobraz_mon(void)
{
  
}*/

void cakaj(char p)
{
  while (bit_is_set(PINC, p)) ;
}

char decToBcd(char d)
{
  return (((d/10)<<4) + (d%10));
}

char bcdToDec(char b)
{
  return (((b>>4)*10) + ((b&0x0f)));
}

void USART_Transmit(char data)
{
  while ( ( UCSRA & (1<<UDRE)) == 0 )
	;
  UDR = data;
  _delay_us(800);
}

void USART_Print(char str[])
{
  unsigned char i = 0;
  while (str[i] != '\0') 
    USART_Transmit(str[i++]);
  
}

char USART_Receive(void)
{
  while ( !(UCSRA & (1<<RXC)) )
    ;

  return UDR;
}

/* 19200kbps 8N1 */
void USART_Init(void)
{
  UCSRB = 0x18;

  UCSRC = 0x86;
  UBRRH = 0;
  UBRRL = 0x0c;
  UCSRA = 0;
}
