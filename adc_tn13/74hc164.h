//*****************************************************************************
//									                                                          *
// Meno súboru		: '74hc164.h'					                                      *
// Popis		: Kninica pre obsluhu posuvnych registrov 74HC164               *
// Autor		: Jergu Lysý				                                    	      *
// Vytvorené		: 1.9.2010					                                          *
//									                                                          *
//									                                                          *
//*****************************************************************************

#ifndef _74_HC164
#define _74_HC164

#define HC164_DDR		(*(volatile unsigned char *) 0x37)
#define HC164_PORT	(*(volatile unsigned char *) 0x38)
//#define _MR_ 0	//Reset (high -> low)
#define CP 0			//Pin na ktorom je pripojeny Clock
#define DATA 1		//Pin na ktorom je pripojeny DATA vstup
#define DP 0x80		//bodka

unsigned char LED_num_table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f};	
unsigned char LED_alph_table[] = {0x77, 0x7f, 0x39, 0x5e, 0x79, 0x71, 0x3d, 0x76, 0x30, 0x1e, 0x75, 0x38, 0x55, 0x54, 0x5c, 0x73, 0x67, 0x50, 0x6d, 0x78, 0x1c, 0x2a, 0x6a, 0x76, 0x6e, 0x5b};

void hc164_init(void);
void hc164_clr(unsigned char len);
void hc164_send(unsigned char data);
void hc164_string(unsigned char *data, unsigned char len);
unsigned char chartoled(unsigned char d);

#include "74hc164.c"

#endif
