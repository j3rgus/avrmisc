#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#define i2c_PORT (*(volatile unsigned char *) 0x32)
#define i2c_PIN  (*(volatile unsigned char *) 0x30)
#define i2c_DDR  (*(volatile unsigned char *) 0x31)
#define SDA 5
#define SCL 6

void USART_Transmit(char data);
char USART_Receive(void);
void USART_Init(void);
//void i2c_init(void);
char i2c_start(void);
void i2c_stop(void);
char i2c_write(char data);
char i2c_read(void);
void write_ds1307(char addr, char data);
char read_ds1307(char addr);

int main(void)
{
  char sek;

  //i2c_init();
  USART_Init();
  
  write_ds1307(0x00, 0x04);
  while (1) {
    sek = read_ds1307(0x00);
	USART_Transmit(' ');
	USART_Transmit(sek);
	USART_Transmit(' ');
	_delay_ms(200);
	_delay_ms(200);
	_delay_ms(200);
  }
}

/*void i2c_init(void)
{
  i2c_PORT &= ~(1 << SCL);
  i2c_PORT &= ~(1 << SDA);
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~SCL;
  i2c_DDR &= ~(1 << SDA);
}*/

char i2c_start(void)
{
  i2c_DDR &= ~(1 << SCL);
  i2c_DDR &= ~(1 << SDA);
  asm("nop");
  asm("nop");
  if (bit_is_clear(i2c_PIN, SDA))
    return 0;
  if (bit_is_clear(i2c_PIN, SCL))
    return 0;
  _delay_us(5);
  i2c_DDR |= (1 << SDA);
  i2c_PORT &= ~(1 << SDA);
  _delay_us(5);
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~(1 << SCL);
  return 1;
}

void i2c_stop(void)
{
  i2c_DDR |= (1 << SDA);
  i2c_PORT &= ~(1 << SDA);
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~(1 << SCL);
  _delay_us(10);
  i2c_DDR &= ~(1 << SCL);
  _delay_us(5);
  i2c_DDR &= ~(1 << SDA);
}

char i2c_write(char data)
{
  char i, res = 1;
  
  for (i = 0; i < 8; i++) {
    if ((data & 0x80) == 0) {
	  i2c_DDR |= (1 << SDA);
	  i2c_PORT &= ~(1 << SDA);
	}
	else
	  i2c_DDR &= ~(1 << SDA);
	
	_delay_us(10);
	i2c_DDR &= ~(1 << SCL);
	_delay_us(5);
	while (bit_is_clear(i2c_PIN, SCL))
	  ;
	_delay_us(5);
	i2c_DDR |= (1 << SCL);
	i2c_PORT &= ~(1 << SCL);
	data <<= 1;
  }
  i2c_DDR &= ~(1 << SDA);
  _delay_us(5);
  i2c_DDR &= ~(1 << SCL);
  _delay_us(10);
  if (bit_is_set(i2c_PIN, SDA))
    res = 0;
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~(1 << SCL);
  return res;
}

char i2c_read(void)
{
  char i, data = 0;

  for (i = 0; i < 8; i++) {
    data <<= 1;
    i2c_DDR &= ~(1 << SCL);
	while (bit_is_clear(i2c_PIN, SCL))
	  ;
	_delay_us(5);
	if (bit_is_clear(i2c_PIN, SDA))
	  data |= 0;
	else data |= 1;
	_delay_us(10);
	i2c_DDR |= (1 << SCL);
	i2c_PORT &= ~(1 << SCL);
	_delay_us(10);
  }
  i2c_DDR &= ~(1 << SDA);
  _delay_us(5);
  i2c_DDR &= ~(1 << SCL);
  _delay_us(10);
  i2c_DDR |= (1 << SCL);
  i2c_PORT &= ~(1 << SCL);
  _delay_us(5);
  i2c_DDR &= ~(1 << SDA);
  _delay_us(5);
  return data;
}

void write_ds1307(char addr, char data)
{
 i2c_start();
 i2c_write(0xD0);
 i2c_write(addr);
 i2c_write(data);
 i2c_stop();
 _delay_ms(20);
}

char read_ds1307(char addr)
{       
 char data;
 i2c_start();
 i2c_write(0xD0);
 i2c_write(addr);
 i2c_start();
 i2c_write(0xD1);
 data = i2c_read();
 i2c_stop();
 return data;
}

char USART_Receive(void)
{
  char l_data;
  asm volatile (
    "15:"			"\n\t"
    "sbis 0x0B,7"	"\n\t"
	"rjmp 15b"		"\n\t"
	"in %0,0x0C"	"\n\t"
	:"=&r" (l_data)
	:
  );
  return l_data;
}

void USART_Transmit(char data)
{
  asm volatile (
  	"12:"			"\n\t"
    "sbis 0x0B,5"	"\n\t"
	"rjmp 12b"		"\n\t"
	"out 0x0c,%0"	"\n\t"
	:
	:"r" (data)
  );
  _delay_ms(1);
}

void USART_Init(void)
{
  asm volatile (
   	"clr r16"		"\n\t"
	"out 0x0B,r16"	"\n\t"
	"out 0x20,r16"	"\n\t"
	"ldi r16,0x18"	"\n\t"
	"out 0x0A,r16"	"\n\t"
	"ldi r16,0x86"	"\n\t"
	"out 0x20,r16"	"\n\t"
	"ldi r16,0x0c"	"\n\t"
	"out 0x09,r16"	"\n\t"
  );
}
