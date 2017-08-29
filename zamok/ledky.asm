.device ATtiny2313
.nolist
.include "tn2313def.inc"
.list
 
.equ dig1 = 0b11111101
.equ dig2 = 0b11111110
.equ dig3 = 0b11111011
.equ dig4 = 0b11110111
.def dig = r21
.def poc1 = r18
.def poc2 = r19
.def reg = r20

.cseg
.org 0x00
		ldi reg,0xff
		out DDRD,reg
		out DDRB,reg
		ldi reg,RAMEND
		out SPL,reg
		ldi ZL,low(tabulka*2)
		ldi ZH,high(tabulka*2)
		clr reg
		ldi DIG,dig3
		out PORTB,DIG
		
RESET:
		lpm
		out PORTD,r0
		rcall Delay
		adiw Z,2
		inc reg
		cpi reg,9
		brne reset
		;rcall Delay
		clr reg
		;out PORTD,reg
		sbiw Z,18
		in dig,PORTB
		cpi dig,dig3
		breq rovna1
		ldi dig,dig3
		out PORTB,dig
		rjmp RESET
rovna1:
		ldi dig,dig4
		out PORTB,dig
		rjmp RESET
		
Delay:
		ldi poc1,0
		ldi poc2,0
		opak:
		inc poc1
		nop
		nop
		nop
		nop
		cpi poc1,255
		brne opak
		inc poc2
		nop
		nop
		nop
		nop
		nop
		cpi poc2,255
		brne opak
		ret

tabulka:
.dw 0b00000110,0b01011011,0b01001111,0b01100110,0b01101101
.dw 0b01111101,0b00000111,0b01111111,0b01101111
