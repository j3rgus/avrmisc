.device ATtiny2313
.nolist
.include "tn2313def.inc"
.list

.equ dig1 = 0b11111110
.equ dig2 = 0b11111101
.equ dig3 = 0b11111011
.equ dig4 = 0b11110111
.def _dig1 = r20
.def _dig2 = r21
.def _dig3 = r22
.def _dig4 = r23
.def reg = r16
.def poc1 = r17
.def poc2 = r18

.macro nastav
		clr poc1
		clr poc2
		opak_pom:
		mov reg,_dig1			;A
		out PORTB,reg
		ldi reg,@0
		out PORTD,reg
		rcall Delay
		mov reg,_dig2			;H
		out PORTB,reg			
		ldi reg,@1
		out PORTD,reg
		rcall Delay
		mov reg,_dig3			;O
		out PORTB,reg			
		ldi reg,@2
		out PORTD,reg
		rcall Delay
		mov reg,_dig4			;J
		out PORTB,reg			
		ldi reg,@3
		out PORTD,reg
		rcall Delay
		inc poc1
		cpi poc1,@4
		brne opak_pom
		inc poc2
		cpi poc2,@5
		brne opak_pom
.endmacro

.cseg
.org 0x00

		ldi reg,low(RAMEND)
		out SPL,reg
		ldi reg,0xff
		out DDRD,reg
		out DDRB,reg
		clr reg
		out DDRA,reg

RESET:

SVIET:
		rcall Pusti
		ldi r24,255
		ldi r25,5
		rcall Vloz
		nastav 0b01110111,0b01110110,0b00111111,0b00011110,1,1
		ldi r26,0x00
		out PORTD,r26
		sbic PINA,0
		rjmp BLIKAJ
		rjmp SVIET
BLIKAJ:	
		rcall Pusti
		ldi r24,255
		ldi r25,2
		rcall Vloz	
		nastav 0b01110111,0b01110110,0b00111111,0b00011110,100,1
		rcall Vloz
		nastav 0,0,0,0,100,1
		sbic PINA,0
		rjmp HAD
		rjmp BLIKAJ
HAD:
		rcall Pusti
		ldi r24,255
		ldi r25,100
		rcall Vloz
		nastav 1,1,1,1,1,1
		rjmp kraje
		spat:
		ldi _dig1,dig4
		ldi _dig2,dig3
		ldi _dig3,dig2
		ldi _dig4,dig1
		nastav 8,8,8,8,1,1
		rjmp pokracuj
		finish:
		ldi r26,0x00
		out PORTD,r26
		sbic PINA,0
		rjmp SVIET
		rjmp HAD

Vloz:
		ldi _dig1,dig1
		ldi _dig2,dig2
		ldi _dig3,dig3
		ldi _dig4,dig4
	ret

Delay:
		push poc2
		push poc1
		clr poc1
		clr poc2
		opak:
		inc poc1
		cp poc1,r24
		brne opak
		inc poc2
		cp poc2,r25
		brne opak
		pop poc1
		pop poc2
	ret

Pusti:
		_opak:
		sbic PINA,0
		rjmp _opak
	ret

kraje:
		ldi reg,dig4			
		out PORTB,reg			
		ldi reg,2
		out PORTD,reg
		rcall Delay			
		ldi reg,4
		out PORTD,reg
		rcall Delay
		rjmp spat
		pokracuj:
		ldi reg,dig1			
		out PORTB,reg			
		ldi reg,16
		out PORTD,reg
		rcall Delay
		ldi reg,32
		out PORTD,reg
		rcall Delay
		rjmp finish
