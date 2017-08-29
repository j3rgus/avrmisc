.device ATtiny2313
.nolist
.include "tn2313def.inc"
.list


.equ LED1 = 0x03
.equ LED2 = 0x0C
.def poc1 = r20
.def poc2 = r21
.cseg
.org 0x00
		ldi r16,0x80
		out CLKPR,r16
		ldi r16,0x00
		out CLKPR,r16
		ldi r16,RAMEND
		out SPL,r16
		ldi r16,0xff
		out DDRD,r16
		clr r17
	
MAIN:
		out PORTD,r16
		out PORTD,r17
		rjmp MAIN


CAKAJ1:
		ldi poc1,0
		ldi poc2,0
	opak1:
		inc poc1
		cpi poc1,255
		brne opak1
		inc poc2
		cpi poc2,200
		brne opak1
	ret


CISTI:
		ldi r16,0x00
		out PORTB,r16
	ret
