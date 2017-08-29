.nolist
.include "tn2313def.inc"
.list

.equ xtal=4000000
.def reg=r16
.cseg
.org 0x0000

		rjmp START
		rjmp IInt0
		rjmp IInt1

IInt0:
		ori reg,0b00000001
		out PORTD,reg
		reti
	
IInt1:
		andi reg,0b11111110
		out PORTD,reg
		reti

START:
		ldi reg,RAMEND
		out spl,reg
		ldi reg,0b00001101
		out DDRD,reg
		cbi PORTD,0
		in reg,GIMSK
		ori reg,0xc0
		out GIMSK,reg
		ldi reg,0x0a
		out 0x35,reg
		ldi reg,0xc0
		out 0x3a,reg
		sei

MAIN:	
		
		rjmp MAIN
