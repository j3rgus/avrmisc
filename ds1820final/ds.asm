.nolist
.include "tn2313def.inc"
.list

;   F_CPU   4.000.000 Hz
;   clock   250ns

.equ DDR = 0x11      ;DDRD
.equ PIN = 0x10      ;PIND
.equ PORT = 0x12   ;PORTD
.equ DQ = 6         ;PD6

.def reg1 = r16
.def reg2 = r17
.def poc1 = r18
.def poc2 = r19
.def poc3 = r20
.def reg3 = r21
.def bit = r22
.def dbajt = r23
.def cislo = r24
.def poc4 = r25

.cseg
         ldi reg1,RAMEND
         out SPL,reg1
         ;sbi DDRD,0
         ;cbi PORTD,0
         ldi reg1,0xff
         out DDRB,reg1
         clr reg1
         out PORTB,reg1      ;0bit - CP, 1bit - C1, 2bit - C2

main:
         rcall zobraz_teplotu
         rjmp main


zobraz_teplotu:
         rcall one_reset
         rcall send_convert
         rcall convert_time
         rcall one_reset
         rcall send_getTemp
         rcall get_temp
         rcall one_reset
      ret

one_read_byte:
         clr poc4
      opak5:
         lsr bit
         sbi DDR,DQ
         cbi PORT,DQ
         rcall cas_12us      ;pri citani stiahnem na log 0 po chvilke uvolnim a citam bud log 1 alebo log 0
         cbi DDR,DQ         ;podla ktoreho nacitavam
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
         rcall cas_12us      ;pri zapise jednotky stiahni na log 0, max 15us, potom vrat na pin
         cbi DDR,DQ
         rcall cas_60us
         rjmp nav4
      nav3:
         rcall cas_60us      ;pri zapise nuly, nechaj port cely cas na log 0
         cbi DDR,DQ
         rcall cas_12us   
      nav4:      
         lsr bit
         inc poc4
         cpi poc4,8
         brne opak6
      ret

one_reset:                  ;RESET vrati 1 ked sa zariadenie naslo
         sbi DDR,DQ         ;v opacnom pripade vrati 0
         cbi PORT,DQ
         rcall cas_500us
         cbi DDR,DQ
         rcall cas_60us
         sbis PIN,DQ
         rjmp nav1
         rcall cas_500us
         ;ldi stav,0
         rjmp nav2
      nav1:
         rcall cas_500us
         ;ldi stav,1
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
         rcall cas_12ms   
         cpi poc1,40
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
         cpi poc2,45      ;45
         brne opak7
         pop poc1
         cli

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
      ret 
