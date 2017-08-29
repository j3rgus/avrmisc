.device ATtiny2313
.nolist
.include "tn2313def.inc"
.list

.def reg=r16
.cseg	
		
ldi r17,0x00
ldi r16,0xee

	EEPROM_write:
; Wait for completion of previous write
sbic EECR,EEPE
rjmp EEPROM_write
; Set up address (r17) in address register
out EEAR, r17
; Write data (r16) to data register
out EEDR,r16
; Write logical one to EEMPE
sbi EECR,EEMPE
; Start eeprom write by setting EEPE
sbi EECR,EEPE

reset:				
		rjmp reset
