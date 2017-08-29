.nolist
.include "tn2313def.inc"
.list


.def poc1 = r16
.def poc2 = r17
.def poc3 = r18
.def reg1 = r19
.def reg2 = r20
.def reg3 = r21
.cseg
		dalej:
			ldi r16,RAMEND
			out SPL,r16
			ldi r17,0xff
			out DDRB,r17
			sbi DDRD,0
			cbi PORTD,0
			ldi r18,0b1111110
			out PORTB,r18
			clr r17
			ldi r19,0b0000100
			ldi r20,0b0100000
			ldi reg3,0b1111110
			rcall display
main:
			rjmp main

display:
			ldi poc1,0
		nav7:
			sbrs reg3,0
			rjmp nuluj
			rcall vystup1
			rjmp nav8
		nuluj:
			rcall vystup0
		nav8:
			inc poc1
			lsr reg3
			cpi poc1,8
			brne nav7
		ret

vystup0:
			out PORTB,r19
			ldi reg1,0b0000000
			out PORTB,reg1
		ret

vystup1:
			out PORTB,r19
			out PORTB,r20
		ret
