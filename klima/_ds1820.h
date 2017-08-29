//*****************************************************************************
//									      *
// Meno súboru		: 'ds1820.h'					      *
// Popis		: Knižnica pre obsluhu DS1820			      *
// Autor		: Jerguš Lysý					      *
// Vytvorené		: 15.8.2009					      *
//									      *
//									      *
//*****************************************************************************
//kniznica LEN pre jeden senzor (upravit!)

#ifndef DS_1820
#define DS_1820

#define uchar char
#define PORT (*(volatile unsigned char *)0x38)
#define PIN  (*(volatile unsigned char *)0x36)
#define DDR  (*(volatile unsigned char *)0x37)
//#define DQ	0b00010000			/* Pin na ktorom je pripojeny senzor */

char one_reset(uchar DQ);
char one_read_byte(uchar DQ);
void one_write_byte(uchar DQ, char data);
void send_convert(uchar DQ);
void send_getTemp(uchar DQ);
//char refresh_temp(void);
//void delay_us(uchar us);

#include "_ds1820.c"
#include <avr/io.h>
#endif
