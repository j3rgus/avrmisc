unsigned char chartoled(unsigned char d);
unsigned char *strtoled(unsigned char *d, unsigned char l);
void nastav_casovac(void);
void zobraz_teploty(void);
void vypis(unsigned char data, unsigned char priznak, unsigned char priznak_des);
void nastav(void);

unsigned char LED_num_table[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x27, ~0x7f, ~0x6f};	
unsigned char LED_alph_table[] = {~0x77, ~0x7f, ~0x39, ~0x5e, ~0x79, ~0x71, ~0x3d, ~0x76, ~0x30, ~0x1e, ~0x75, ~0x38, ~0x55, ~0x54, ~0x5c, ~0x73, ~0x67, ~0x50, ~0x6d, ~0x78, ~0x1c, ~0x2a, ~0x6a, ~0x76, ~0x6e, ~0x5b};

unsigned char m[4];
volatile unsigned char casovac = 0, stopky, scitac = 0;

typedef struct {
  volatile unsigned char von_L;
  volatile unsigned char von_H;
  volatile unsigned char vnut_L;
  volatile unsigned char vnut_H;
  volatile unsigned char alarm;
  volatile unsigned char alarm_sign;
} TEPLOTY;

TEPLOTY teploty;

typedef struct {
  unsigned char tep : 1;
  unsigned char init : 1;
  unsigned char btn1 : 1;
  unsigned char start : 1;
  volatile unsigned char nastav : 1;
  unsigned char alarm : 1;
  unsigned char mrzlo : 1;
  unsigned char mrzne : 1;
} FLAGS;

FLAGS flags;

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

unsigned char *strtoled(unsigned char *d, unsigned char l)
{
  unsigned char i;

  for (i = 0; i < l; i++) {
    d[i] = chartoled(d[i]);
  }
  return d;
}

void nastav_casovac(void)  //4 Mhz - cca 65ms
{
  TCCR0A = 0;
  TCCR0B = (1 << CS02) | (1 << CS00);
  TCNT0 = 0;
  TIMSK |= (1 << TOIE0);
  asm("sei");
}

void convert_tep(void)
{
  one_reset(VONK_TEP);
  one_reset(VNUT_TEP);
  send_convert(VONK_TEP);
  send_convert(VNUT_TEP);
}

void citaj_tep(void)
{
  one_reset(VONK_TEP);
  one_reset(VNUT_TEP);
  send_getTemp(VONK_TEP);
  send_getTemp(VNUT_TEP);
  teploty.von_L = one_read_byte(VONK_TEP);
  teploty.vnut_L = one_read_byte(VNUT_TEP);
  teploty.von_H = one_read_byte(VONK_TEP);
  teploty.vnut_H = one_read_byte(VNUT_TEP);
  if (teploty.von_H) teploty.von_L = ~teploty.von_L + 1;
  if (teploty.vnut_H) teploty.vnut_L = ~teploty.vnut_L + 1;
}

void zobraz_teploty(void)
{
  if (!flags.btn1) {
    if ((casovac % 15) == 0)
      vypis(teploty.vnut_L >> 1, teploty.vnut_H, teploty.vnut_L & 1);
  } else {
    if ((casovac % 15) == 0)
      vypis(teploty.von_L >> 1, teploty.von_H, teploty.von_L & 1);
      if (casovac == stopky)
         flags.btn1 = 0;
  }
}

void vypis(unsigned char data, unsigned char priznak_znam, unsigned char priznak_des)
{
   if (data >= 10)
     itoa((int) data, &m[1], 10);
   else 
     itoa((int) data, &m[2], 10);
  if (priznak_des)
    m[3] = '5';
  else m[3] = '0';
  strtoled(m, 4);
  m[2] &= ~128;  // bodka
  if (data >= 10) {
    m[0] = (priznak_znam) ? ~64 : 255; // minus / plus
  }
  else 
    m[1] |= (priznak_znam) ? ~64 : 255;
  zapis_seg(m, 4);
  zobraz_dis();
}

void nastav(void)
{
  unsigned char stopky_nastav;
  flags.nastav = 1;
  stopky_nastav = casovac + 70;

  while (flags.nastav) {
    if (bit_is_set(PIND, BT2)) { 
      teploty.alarm++; 
      stopky_nastav = casovac + 70;
      _delay_ms(150);
    }
    if (bit_is_set(PIND, BT3)) { 
      teploty.alarm--;
      stopky_nastav = casovac + 70;
      _delay_ms(150);
    }
    if (teploty.alarm > 120) 
      teploty.alarm = 6;
    if (teploty.alarm < 6)
      teploty.alarm = 120;
    vypis(teploty.alarm >> 1, teploty.alarm_sign, teploty.alarm & 1);
    if (stopky_nastav == casovac) { flags.nastav = 0; flags.alarm = 1;}
  }
}
