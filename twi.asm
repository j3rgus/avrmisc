	.DSEG
	.ORG 0x160
;       1 #include <mega8.h>
;       2 
;       3 #asm
;       4    .equ __i2c_port=0x12 ;PORTD
   .equ __i2c_port=0x12 ;PORTD
;       5    .equ __sda_bit=5
   .equ __sda_bit=5
;       6    .equ __scl_bit=6
   .equ __scl_bit=6
;       7 #endasm
;       8 #include <i2c.h> 
;       9 #include <delay.h>
;      10 
;      11 // Standard Input/Output functions
;      12 #include <stdio.h>
;      13 
;      14 void write_DS1307(unsigned char Addr, unsigned char Data)
;      15 {

	.CSEG
_write_DS1307:
;      16  i2c_start();
;	Addr -> Y+1
;	Data -> Y+0
	RCALL SUBOPT_0x0
;      17  i2c_write(0xD0);
;      18  i2c_write(Addr);
;      19  i2c_write(Data);
	LD   R30,Y
	ST   -Y,R30
	RCALL _i2c_write
;      20  i2c_stop();
	RCALL _i2c_stop
;      21 }
	ADIW R28,2
	RET
;      22 
;      23 char read_DS1307(unsigned char Addr)
;      24 {       
_read_DS1307:
;      25  char data;
;      26  i2c_start();
	ST   -Y,R16
;	Addr -> Y+1
;	data -> R16
	RCALL SUBOPT_0x0
;      27  i2c_write(0xD0);
;      28  i2c_write(Addr);
;      29  i2c_start();
	RCALL _i2c_start
;      30  i2c_write(0xD1);
	LDI  R30,LOW(209)
	ST   -Y,R30
	RCALL _i2c_write
;      31  data = i2c_read(0);
	RCALL SUBOPT_0x1
	RCALL _i2c_read
	MOV  R16,R30
;      32  i2c_stop();
	RCALL _i2c_stop
;      33  return(data);
	MOV  R30,R16
	LDD  R16,Y+0
	ADIW R28,2
	RET
;      34 }
;      35 
;      36 
;      37 void main(void)
;      38 {
_main:
;      39 
;      40 UCSRA=0x00;
	LDI  R30,LOW(0)
	OUT  0xB,R30
;      41 UCSRB=0x18;
	LDI  R30,LOW(24)
	OUT  0xA,R30
;      42 UCSRC=0x86;
	LDI  R30,LOW(134)
	OUT  0x20,R30
;      43 UBRRH=0x00;
	LDI  R30,LOW(0)
	OUT  0x20,R30
;      44 UBRRL=0x0C;
	LDI  R30,LOW(12)
	OUT  0x9,R30
;      45 
;      46 i2c_init();
	RCALL _i2c_init
;      47 
;      48 write_DS1307(0x00, 0x00);
	RCALL SUBOPT_0x1
	RCALL SUBOPT_0x1
	RCALL _write_DS1307
;      49 read_DS1307(0x00);
	RCALL SUBOPT_0x1
	RCALL _read_DS1307
;      50 /*
;      51 write_DS1307(0x00, 0x00);
;      52 write_DS1307(0x01, 0x00);
;      53 write_DS1307(0x02, 0x00);  */
;      54 while (1)
_0x3:
;      55       {                      /*
;      56         printf("%x:", read_DS1307(0x02));
;      57         printf("%x:", read_DS1307(0x01));
;      58         printf("%x  ", read_DS1307(0x00)); 
;      59         delay_ms(1000);        */
;      60       };
	RJMP _0x3
;      61 }
_0x6:
	RJMP _0x6

_getchar:
     sbis usr,rxc
     rjmp _getchar
     in   r30,udr
	RET
_putchar:
     sbis usr,udre
     rjmp _putchar
     ld   r30,y
     out  udr,r30
	ADIW R28,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x0:
	RCALL _i2c_start
	LDI  R30,LOW(208)
	ST   -Y,R30
	RCALL _i2c_write
	LDD  R30,Y+1
	ST   -Y,R30
	RJMP _i2c_write

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1:
	LDI  R30,LOW(0)
	ST   -Y,R30
	RET

	.equ __i2c_dir=__i2c_port-1
	.equ __i2c_pin=__i2c_port-2
_i2c_init:
	cbi  __i2c_port,__scl_bit
	cbi  __i2c_port,__sda_bit
	sbi  __i2c_dir,__scl_bit
	cbi  __i2c_dir,__sda_bit
	rjmp __i2c_delay2		;10 us - delay 2
_i2c_start:
	cbi  __i2c_dir,__sda_bit
	cbi  __i2c_dir,__scl_bit
	clr  r30
	nop
	sbis __i2c_pin,__sda_bit
	ret
	sbis __i2c_pin,__scl_bit
	ret
	rcall __i2c_delay1		;5,5 us - delay 1
	sbi  __i2c_dir,__sda_bit
	rcall __i2c_delay1
	sbi  __i2c_dir,__scl_bit
	ldi  r30,1
__i2c_delay1:
	ldi  r22,7
	rjmp __i2c_delay2l
_i2c_stop:
	sbi  __i2c_dir,__sda_bit
	sbi  __i2c_dir,__scl_bit
	rcall __i2c_delay2
	cbi  __i2c_dir,__scl_bit
	rcall __i2c_delay1
	cbi  __i2c_dir,__sda_bit
__i2c_delay2:
	ldi  r22,13
__i2c_delay2l:
	dec  r22
	brne __i2c_delay2l
	ret
_i2c_read:
	ldi  r23,8
__i2c_read0:
	cbi  __i2c_dir,__scl_bit
	rcall __i2c_delay1
__i2c_read3:
	sbis __i2c_pin,__scl_bit	;ak PIND SCL bit je 1, skoc
	rjmp __i2c_read3
	rcall __i2c_delay1
	clc
	sbic __i2c_pin,__sda_bit	;ak PIND SDA bit je 0, skoc
	sec
	sbi  __i2c_dir,__scl_bit
	rcall __i2c_delay2
	rol  r30
	dec  r23
	brne __i2c_read0
	ld   r23,y+
	tst  r23
	brne __i2c_read1
	cbi  __i2c_dir,__sda_bit
	rjmp __i2c_read2
__i2c_read1:
	sbi  __i2c_dir,__sda_bit
__i2c_read2:
	rcall __i2c_delay1
	cbi  __i2c_dir,__scl_bit
	rcall __i2c_delay2
	sbi  __i2c_dir,__scl_bit
	rcall __i2c_delay1
	cbi  __i2c_dir,__sda_bit
	rjmp __i2c_delay1

_i2c_write:
	ld   r30,y+
	ldi  r23,8
__i2c_write0:
	lsl  r30
	brcc __i2c_write1
	cbi  __i2c_dir,__sda_bit
	rjmp __i2c_write2
__i2c_write1:
	sbi  __i2c_dir,__sda_bit
__i2c_write2:
	rcall __i2c_delay2
	cbi  __i2c_dir,__scl_bit
	rcall __i2c_delay1
__i2c_write3:
	sbis __i2c_pin,__scl_bit
	rjmp __i2c_write3
	rcall __i2c_delay1
	sbi  __i2c_dir,__scl_bit
	dec  r23
	brne __i2c_write0
	cbi  __i2c_dir,__sda_bit
	rcall __i2c_delay1
	cbi  __i2c_dir,__scl_bit
	rcall __i2c_delay2
	ldi  r30,1
	sbic __i2c_pin,__sda_bit
	clr  r30
	sbi  __i2c_dir,__scl_bit
	ret

;END OF CODE MARKER
__END_OF_CODE:
