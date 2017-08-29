.nolist
.include "m168def.inc"
.list

main:
		ldi r16,0x56
		nop
		nop
		rjmp main
