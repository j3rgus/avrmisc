.nolist
.include "tn2313def.inc"
.list

.equ xtal=4000000
.def parne = r16
.def neparne = r17
.def reg = r18
.macro NOPUJ
		nop
		nop
		nop
		nop
.endmacro
.cseg
		ldi reg,0xff
		out DDRD,reg
		ldi parne,0b0101010
		ldi neparne,0b1010101
		ldi reg,RAMEND
		out spl,reg

MAIN:
		out PORTD,parne
		rcall CAKAJ
		out PORTD,neparne
		rcall CAKAJ
		rjmp MAIN

CAKAJ:
		ldi r19,0xff
		ldi reg,0xff
POM:		
		dec r19
		NOPUJ
		cpi r19,0
		brne POM
		dec reg
		NOPUJ
		cpi reg,0
		brne POM
		ret
