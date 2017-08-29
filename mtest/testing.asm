.nolist
.include "tn2313def.inc"
.list

.cseg
.org 0x00

			ldi r16,RAMEND
			out SPL,r16
			ldi r20,0xfe
			sts 0x70,r20
			ldi ZL,00
			ldi ZH,4
		opak:
			lsl r20
			ld r18,Z+
			rjmp opak
main:
			rjmp main

.dseg
tabulka:
.db 0x55,0x44,0xff,0xcc,0x05
