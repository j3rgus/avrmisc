//*****************************************************************************
//									                                                          *
// Meno súboru		: '74hc164.c'					                                      *
// Popis		: Funkcie pre obsluhu posuvnych registrov 74HC164                 *
// Autor		: Jergu Lysý				                                    	      *
// Vytvorené		: 1.9.2010					                                          *
//									                                                          *
//									                                                          *
//*****************************************************************************

/*
   Funkcia:	 void hc164_init(void)
   Funkcia nastavi prislune porty (nutne zavolat pred samotnou pracou s IO)
	 Neprebieha ziadna obojsmerna komunikacia => Nevrati ziadnu hodnotu
*/

void hc164_init(void)
{
  HC164_DDR |= ((1 << CP) | (1 << DATA));
  HC164_PORT &= ~((1 << CP) | (1 << DATA));
#ifdef _MR_
	HC164_DDR |= (1 << _MR_);
  HC164_PORT |= (1 << _MR_);
#endif
}

/*
   Funkcia:	 void hc164_clr(void)
   Funkcia restartuje IO (vystupy budu na log 0)
	 Nevrati ziadnu hodnotu
*/

void hc164_clr(unsigned char len)
{
	unsigned char i;
#ifdef _MR_
	HC164_PORT &= ~(1 << _MR_);
	HC164_PORT |= (1 << _MR_);
#else
	for (i = 0; i < len; i++)
		hc164_send(0);
#endif
}

/*
   Funkcia:	 void hc164_send(unsigned char data)
   Funkcia odosle byte na IO
	 Nevrati ziadnu hodnotu
*/

void hc164_send(unsigned char data)
{
  unsigned char i;

  for (i = 0; i < 8; i++) {
		if (data & 0x80)
			HC164_PORT |= (1 << DATA);
		else 
			HC164_PORT &= ~(1 << DATA);
		HC164_PORT |= (1 << CP);
		HC164_PORT &= ~(1 << CP);
		data <<= 1;
  }
}

/*
   Funkcia:	 void hc164_string(unsigned char *data)
   Funkcia odosle retazec na IO (pocet IO zavisi na konst. NUMBER def. v hlav. subore)
	 Plati LIFO - pricom posledny odoslany byte je na vystupe posledneho pripojeneho IO 
	 Nevrati ziadnu hodnotu
*/

void hc164_string(unsigned char *data, unsigned char len)
{
	unsigned char i, j;

	for (i = 0; i < (len-1); i++) {
		for (j = 0; j < 8; j++) {
			hc164_send(data[i]);
		}
	}
	hc164_send(data[i]);
}

/*
   Funkcia:	 unsigned char chartoled(unsigned char d)
   Funkcia prerobi bitove poradie ASCII do bitoveho poradia 7seg. displeja
	 Funkcia vyuziva tabulku LED_num_table (pre cislice) a LED_alph_table (pismena)
	 Vrati byte pre vypis na 7seg. displej
*/

unsigned char chartoled(unsigned char d)
{
	if ((d >= '0') && (d <= '9')) {
		return LED_num_table[d - '0'];
	} else if (((d >= 'A') && (d <= 'Z')) || ((d >= 'a') && (d <= 'z'))) {
		d &= ~0x20;
	  return LED_alph_table[d - 'A'];
	} else {
		return 0;
	}
}
