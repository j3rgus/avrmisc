.device ATtiny2313
.nolist
.include "tn2313def.inc"
.list

.equ nultyBit = 0			;pomocka pre rozlisenie bitov
.equ prvyBit = 1
.equ druhyBit = 2
.equ tretiBit = 3
.equ stvrtyBit = 4
.equ piatyBit = 5
.equ siestyBit = 6
.def reg = r16
.def status = r17			;0-zacni nastavovat zamok, 1-zapis do E2PROM, 2-citanie z E2PROM
.def data = r18				;kombinacii zamku
.def poc = r19
.def reg1 = r20
.def nac_data = r21

.eseg


.cseg
.org 0x00
		rjmp RESET
		rjmp IInt0
		rjmp IInt1
		
IInt0:
		;in reg,PORTD
		;ori reg,0b00100000
		;out PORTD,reg
		ldi status,0b00000001	;prerusenie pre nastavenie kombinacii (nastavenie 0-bitu v registry status)
		ret

IInt1:
		ret

RESET:
		ser reg
		out DDRA,reg
		ldi reg,RAMEND
		out SPL,reg			;nastavenie zasobnika (v tomvo pripade 128B SRAM, tzn SPH neberem do uvahy)
		ldi reg,0xc0		;povolenie prerusenie INT0 a INT1
 		out GIMSK,reg
		ldi reg,0x0a
		out MCUCR,reg		;vyvolanie prerusenia zostupnou hranou
		ldi reg,0xc0
		out GIFR,reg		;nastavenie priznakov INT0 a INT1
		ldi reg,0b01100000
		out DDRD,reg
		rcall CITAJ
		cpi nac_data,0xff
		sei
		breq KONTR
		cli
		ldi status,0b00000101
		sbi PORTA,nultyBit
KONTR:
		cpi status,0b00000100
		breq dalej
		sbrc status,prvyBit
		rcall ZAPIS
		sbrs status,nultyBit
		rjmp KONTR
		sbic PIND,nultyBit		;cakaj na prve tlacidlo
		rcall NASTAV			;cakaj na druhe tlacidlo
		sbic PIND,prvyBit
		rcall NASTAV
		sbrc status,druhyBit
		rcall POROV
		rjmp KONTR
		dalej:
		rcall CITAJ
		rjmp KONTR

NASTAV:						;nastavenie kombinacie
		in reg,PIND			;nacitanie PIND registra
		inc poc				;pocitaj, max 2x volat (2 komb)
		in reg1,DDRD		;zalohuj DDRD prepisem nacitantm vstupom, aby som nemusel cakat kym bude pustene tlacitko
		push reg1			;...preto tam kde je log 1 na vstupe zmenim pin na vystupny 
		or reg1,reg
		out DDRD,reg1
		or data,reg			;hlavné data
		cpi poc,2
		brne koniec
		ori status,0b00000011	;do status registra
		rjmp koniec1
		koniec:
		lsl data			;uvolnim miesto pre dalsiu kombinaciu
		lsl data
		koniec1:
		pop reg1
		out DDRD,reg1
	ret

CITAJ:
		opakc:
		sbic EECR,EEPE
		rjmp opakc
		ldi reg,0x00
		out EEAR,reg
		sbi EECR,EERE
		in nac_data,EEDR		;nacitaj do nac_data
		ori status,0b00000001
	ret

ZAPIS:
		opakz:
		sbic EECR,EEPE		;cakaj na dokoncenie predchadzajuci
		rjmp opakz
		ldi reg,0x00
		out EEAR,reg		;zapis na adresu 0x00
		out EEDR,data		;zapis data
		sbi EECR,EEMPE		;zapis
		sbi EECR,EEPE		
		ldi reg,0x00
		ldi reg,0b00000100
		mov status,reg
		sbi PORTA,nultyBit
		clr data
	ret

POROV:
		cp data,nac_data
		brne pokracuj
		cbi PORTA,nultyBit
		clr status
		sei

		pokracuj:
	ret
