.nolist
.include "tn2313def.inc"
.list

.def reg = r16
.equ xtal = 8000000

		ldi reg,0b11111100
		out DDRD,reg
		ldi reg,RAMEND
		out spl,reg
MAIN:
		cbi PORTD,2
		sbis PIND,0
		rcall SVIET
		sbis PIND,1
		rcall SVIET
		rjmp MAIN

SVIET:
		sbi PORTD,2
		rcall CASOVAC
over:
		sbis PIND,0
		ret
		sbic PIND,1
		rjmp over
		ret

CASOVAC:
		ldi r17,0xff
		ldi r18,0xff
znova:	
		dec r17
		cpi r17,0
		brne znova
		dec r18
		cpi r18,0
		brne znova
		ret
