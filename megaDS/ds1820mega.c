#include <avr\io.h>
#include <avr/eeprom.h>
#include "ds1820\ds1820.h"
#include "ds1307\ds1307.h"
#include "lcd/lcd_lib.h"
#include <string.h>
#include <stdlib.h>

#define F_CPU 4000000UL

void USART_Transmit(char data4);
char USART_Receive(void);
void USART_Init(void);
void USART_Print(char str[]);
void refresh_udaje(void);
char zisti_den(void);
void zobrazenie(void);
void konfig(void);
void udajeNaStr();
void zvysuj(void);
void CHbit_nastav(void);
void CHbit_nuluj(void);
void nast_hodin(char *hms, char X, char podm, char phms[]);
char decToBcd(char d);
char bcdToDec(char b);
void vypis_cas(void);

struct {
	char teplota;
	char sek;
	char min;
	char hod;
	char den;
	char mes;
	char rok;
	char mDen;
} udaje;

char s_t[4], s_s[4], s_m[4], s_h[4], s_d[4], s_me[4], s_r[4];
char *dni[] = {"Po ", "Ut ", "Str", "Stv", "Pia", "So ", "Ne "};

int main(void)
{

  USART_Init();
  LCDinit();
  LCDcursorOFF();
  CHbit_nuluj();
  DDRC = 0x00;

  while (1) {
    zobrazenie();
	if (bit_is_set(PINC, 0))
	  konfig();  
  } 
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

void refresh_udaje(void)
{
  one_reset();
  send_convert();
  udaje.sek = read_ds1307(0);
  udaje.min = read_ds1307(1);
  udaje.hod = read_ds1307(2);
  udaje.mDen = read_ds1307(3);
  udaje.den = read_ds1307(4);
  udaje.mes = read_ds1307(5);
  udaje.rok = read_ds1307(6);
  udajeNaStr();
  _delay_ms(200);
  _delay_ms(200);
  one_reset();
  send_getTemp();
  udaje.teplota = one_read_byte();
  one_reset();
  udaje.teplota >>= 1;
  itoa(udaje.teplota, s_t, 10); s_s[strlen(s_t)] = '\0';
}

void udajeNaStr()
{
  itoa((udaje.sek & 0x7f), s_s, 16); s_s[strlen(s_s)] = '\0';
  itoa(udaje.min, s_m, 16); s_m[strlen(s_m)] = '\0';
  itoa(udaje.hod, s_h, 16); s_h[strlen(s_h)] = '\0';
  itoa(udaje.den, s_d, 16); s_d[strlen(s_d)] = '\0';
  itoa(udaje.mes, s_me, 16); s_me[strlen(s_me)] = '\0';
  itoa(udaje.rok, s_r, 16); s_r[strlen(s_r)] = '\0';
}

void zobrazenie(void)
{
	LCDhome();
	refresh_udaje();
	vypis_cas();
	LCDsendChar(' ');
	LCDGotoXY(11,0);
	if ((udaje.teplota < 100) && (udaje.teplota > 9)) LCDsendChar(' ');
	if ((udaje.teplota < 10)) {LCDsendChar(' ');LCDsendChar(' ');}
	LCDstring((unsigned char *)s_t, strlen(s_t));
	LCDsendChar(0xdf); LCDsendChar('C');
	LCDGotoXY(0,1);
	LCDstring((unsigned char *) (dni[udaje.mDen-1]), 3);
	LCDGotoXY(6,1);
	LCDstring((unsigned char *)s_d,strlen(s_d));LCDsendChar('.');
	LCDstring((unsigned char *)s_me,strlen(s_me));LCDstring(". 20", 4);
	if (udaje.rok < 10) LCDsendChar('0'); LCDstring((unsigned char *)s_r,strlen(s_r));
}

void konfig(void)
{
  LCDclr();
  while (bit_is_set(PINC, 0))
    ;
  LCDhome();
  LCDstring("Nastavenie hodin", 16);
  CHbit_nastav();  
  LCDGotoXY(0,1);
  
  if (udaje.hod < 10) LCDsendChar(' ');
  vypis_cas();
  
  LCDcursorOn();
  nast_hodin(&udaje.hod, 0, 0x23, s_h);
  _delay_ms(150);
  nast_hodin(&udaje.min, 3, 0x59, s_m);
  _delay_ms(150);
  nast_hodin(&udaje.sek, 6, 0x59, s_s);
  LCDcursorOFF();
  LCDclr();
}

void CHbit_nastav(void)
{
  udaje.sek = read_ds1307(0);
  udaje.sek |= 0x80;
  write_ds1307(0, udaje.sek);
}

void CHbit_nuluj(void)
{
  udaje.sek = read_ds1307(0);
  udaje.sek &= 0x7f;
  write_ds1307(0, udaje.sek);
}

void nast_hodin(char *hms, char X, char podm, char phms[])
{
  LCDGotoXY(X,1);
  while (1) {
	if (bit_is_set(PINC, 1)) {
	  if ((*hms) < podm) {
	    (*hms)++;
	    if ((bcdToDec((*hms)) % 10) == 0)
	      (*hms) += 6;
	  }
	  else *hms = 0;
      LCDstring((unsigned char *)phms, strlen(phms));
	  LCDGotoXY(X,1);
	  _delay_ms(200);
	}
	if (bit_is_set(PINC, 0))
	  break;
  }
}

void vypis_cas(void)
{
  LCDstring((unsigned char *)s_h, strlen(s_h));
  LCDsendChar(':');
  if (udaje.min < 10) LCDsendChar('0'); LCDstring((unsigned char *)s_m, strlen(s_m));
  LCDsendChar(':');
  if (udaje.sek < 10) LCDsendChar('0'); LCDstring((unsigned char *)s_s, strlen(s_s));
}

char decToBcd(char d)
{
  return (((d/10)<<4) + (d%10));
}

char bcdToDec(char b)
{
  return (((b>>4)*10) + ((b&0x0f)));
}
