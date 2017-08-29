.nolist
.include "tn2313def.inc"
.list

;	F_CPU	4.000.000 Hz
;	clock	250ns

.equ DDR = 0x11		;DDRD
.equ PIN = 0x10		;PIND
.equ PORT = 0x12	;PORTD
.equ DQ = 6			;PD6

.def reg1 = r16
.def reg2 = r17
.def poc1 = r18
.def poc2 = r19
.def poc3 = r20
.def stav = r21
.def bit = r22
.def dbajt = r23
.def cislo = r24
.def poc4 = r25

.cseg
			ldi reg1,RAMEND
			out SPL,reg1
			ldi reg1,0b01111111
			out DDRB,reg1
			ldi reg1,0x00
			out PORTB,reg1
			sbi DDRD,0
			cbi PORTD,0
			sbi DDRD,1
			cbi PORTD,1
			sbi DDRD,4				
			cbi PORTD,4
			sbi DDRD,5					
			cbi PORTD,5
			ldi ZH,high(Tabulka*2)
			ldi ZL,low(Tabulka*2)
			ldi reg1,0
			ldi reg2,0
			mov r1,reg1
			mov r2,reg2

main:
			rcall convert_time
			rcall one_reset
			rcall send_convert
			rcall convert_time
			rcall one_reset
			rcall send_getTemp
			rcall get_temp
			rcall convert_time
			rcall one_reset
			rcall convert_time
			rcall convert_time
			rjmp main


one_read_byte:
			clr poc4
		opak5:
			lsr bit
			sbi DDR,DQ
			cbi PORT,DQ
			rcall cas_12us		;pri citani stiahnem na log 0 po chvilke uvolnim a citam bud log 1 alebo log 0
			cbi DDR,DQ			;podla ktoreho nacitavam
			rcall cas_12us
			sbis PIN,DQ
			rjmp nav5
			ori bit,0x80
			rjmp nav6
		nav5:
			ori bit,0
		nav6:
			rcall cas_60us
			inc poc4
			cpi poc4,8
			brne opak5
		ret

one_write_byte:
			clr poc4
		opak6:
			sbi DDR,DQ
			cbi PORT,DQ
			sbrs bit,0
			rjmp nav3
			rcall cas_12us		;pri zapise jednotky stiahni na log 0, max 15us, potom vrat na pin
			cbi DDR,DQ
			rcall cas_60us
			rjmp nav4
		nav3:
			rcall cas_60us		;pri zapise nuly, nechaj port cely cas na log 0
			cbi DDR,DQ
			rcall cas_12us	
		nav4:		
			lsr bit
			inc poc4
			cpi poc4,8
			brne opak6
		ret

one_reset:						;RESET vrati 1 ked sa zariadenie naslo
			sbi DDR,DQ			;v opacnom pripade vrati 0
			cbi PORT,DQ
			rcall cas_500us
			cbi DDR,DQ
			rcall cas_60us
			sbis PIN,DQ
			rjmp nav1
			rcall cas_500us
			ldi stav,0
			rjmp nav2
		nav1:
			rcall cas_500us
			ldi stav,1
		nav2:
		ret

cas_12us:
			ldi poc1,0
		opak1:
			inc poc1
			cpi poc1,10
			brne opak1
		ret
cas_60us:
			ldi poc1,0
		opak2:
			inc poc1
			cpi poc1,60
			brne opak2
		ret

cas_500us:
			ldi poc1,0
			ldi poc2,0
		opak3:
			inc poc1
			cpi poc1,250
			brne opak3
			inc poc2
			cpi poc2,2
			brne opak3
		ret

convert_time:
			ldi poc1,0
		opak4:
			inc poc1
			rcall display	
			cpi poc1,19
			brne opak4
		ret

cas_12ms:
			push poc1
			ldi poc1,0
			ldi poc2,0
		opak7:
			inc poc1
			cpi poc1,250
			brne opak7
			inc poc2
			cpi poc2,39		;45
			brne opak7
			pop poc1
		ret

send_convert:
			ldi bit,0xcc				
			rcall one_write_byte

			ldi bit,0x44				
			rcall one_write_byte
		ret

send_getTemp:
			ldi bit,0xcc				
			rcall one_write_byte
			
			ldi bit,0xbe				
			rcall one_write_byte
		ret

get_temp:
			rcall one_read_byte
			mov cislo,bit
			rcall one_read_byte
			mov dbajt,bit
			ldi reg2,0x3f
			ldi ZH,high(2*Tabulka)
			ldi ZL,low(2*Tabulka)
			cpi dbajt,0xff
			breq _minus
			sbrc cislo,0
			ldi reg2,0x6d
			andi cislo,0xfe
			rjmp dalej
			;mov cislo,bit
		_minus:
			ldi reg1,0xff
			eor cislo,reg1
			sbrc cislo,0
			ldi reg2,0x6d
			andi cislo,0xfe
		dalej:
			add ZL,cislo
			lpm r0,Z+
			lpm r1,Z
		ret

display:
			ldi stav,0x3f
			cbi PORTD,4			;4
			sbi PORTD,0
			sbi PORTD,1
			sbi PORTD,5
			out PORTB,r1
			rcall cas_12ms
			sbi PORTD,4
			sbi PORTD,1
			sbi PORTD,0
			cbi PORTD,5			;5
			out PORTB,r0
			rcall cas_12ms
			sbi PORTD,4
			sbi PORTD,5
			sbi PORTD,1
			cbi PORTD,0			;0
			out PORTB,reg2
			rcall cas_12ms
			cpi dbajt,0xff
			breq nula
			rjmp __dalej
		_nula:
			sbi PORTD,0
			sbi PORTD,4
			sbi PORTD,5
			cbi PORTD,1			;1
			ldi reg1,0x40
			out PORTB,reg1
			rcall cas_12ms
		_dalej:
			rjmp __dalej
		nula:
			cp r1,stav
			breq cont
			rjmp _nula
		cont:
			cp r2,stav
			breq cont2
			rjmp _nula
		cont2:
			cpi reg2,0x3f
			breq __dalej
			rjmp _nula
		__dalej:
		ret

Tabulka:
.dw 0x3f3f,0x3f06,0x3f5b,0x3f4f,0x3f66,0x3f6d,0x3f7d,0x3f07,0x3f7f,0x3f6f
.dw 0x063f,0x0606,0x065b,0x064f,0x0666,0x066d,0x067d,0x0607,0x067f,0x066f
.dw 0x5b3f,0x5b06,0x5b5b,0x5b4f,0x5b66,0x5b6d,0x5b7d,0x5b07,0x5b7f,0x5b6f
.dw 0x4f3f,0x4f06,0x4f5b,0x4f4f,0x4f66,0x4f6d,0x4f7d,0x4f07,0x4f7f,0x4f6f
.dw 0x663f,0x6606,0x665b,0x664f,0x6666,0x666d,0x667d,0x6607,0x667f,0x666f
.dw 0x6d3f,0x6d06,0x6d5b,0x6d4f,0x6d66,0x6d6d,0x6d7d,0x6d07,0x6d7f,0x6d6f
.dw 0x7d3f,0x7d06,0x7d5b,0x7d4f,0x7d66,0x7d6d,0x7d7d,0x7d07,0x7d7f,0x7d6f
.dw 0x073f,0x0706,0x075b,0x074f,0x0766,0x076d,0x077d,0x0707,0x077f,0x076f
.dw 0x7f3f,0x7f06,0x7f5b,0x7f4f,0x7f66,0x7f6d,0x7f7d,0x7f07,0x7f7f,0x7f6f
.dw 0x6f3f,0x6f06,0x6f5b,0x6f4f,0x6f66,0x6f6d,0x6f7d,0x6f07,0x6f7f,0x6f6f
