.nolist
.include "m8def.inc"
.list

.def poc=r16
.def data=r17
.def addr=r18
.def buf_data=r20
.equ __i2c_port=0x12
.equ __i2c_dir=__i2c_port-1
.equ __i2c_pin=__i2c_port-2
.equ __sda_bit=5
.equ __scl_bit=6

.cseg

		ldi r16,low(RAMEND)
		out SPL,r16
		ldi r16,high(RAMEND)
		out SPH,r16
		rcall _uart_init
		rcall _i2c_init

		ldi addr,0
		ldi buf_data,0x00
		rcall _ds1307_write
main:
		ldi addr,0
		rcall _ds1307_read
		mov r19,buf_data
		rcall _putchar
		rcall _delay
		rjmp main


_getchar:
		sbis UCSRA,rxc
		rjmp _getchar
		in r19,UDR
	ret

_putchar:
		sbis UCSRA,UDRE
		rjmp _putchar
		out UDR,r19
	ret

_uart_init:
		clr r16
		out UCSRA,r16
		out UBRRH,r16
		ldi r16,0x18
		out UCSRB,r16
		ldi r16,0x86
		out UCSRC,r16
		ldi r16,0x0c
		out UBRRL,r16
	ret

_delay:
		ldi poc,255
	__delay:
		rcall _delay1
		rcall _delay1
		rcall _delay1
		rcall _delay1
		rcall _delay1
		rcall _delay1
		rcall _delay1
		rcall _delay1
		rcall _delay1
		rcall _delay1
		dec poc
		brne __delay
	ret

_delay1:
		push poc
		ldi poc,255
	__delay1:
		nop
		nop
		nop
		dec poc
		brne __delay1
		pop poc
	ret

__i2c_delay2:
		ldi poc,13
		___i2c_delay2:
		dec poc
		brne ___i2c_delay2
	ret

__i2c_delay1:
		ldi  poc,7
		___i2c_delay1:
		dec poc
		brne ___i2c_delay1
	ret

_i2c_init:
		cbi  __i2c_port,__scl_bit
		cbi  __i2c_port,__sda_bit
		sbi  __i2c_dir,__scl_bit
		cbi  __i2c_dir,__sda_bit
		rcall __i2c_delay2
	ret

_i2c_start:
		cbi  __i2c_dir,__sda_bit
		cbi  __i2c_dir,__scl_bit
		clr  r30
		nop
		sbis __i2c_pin,__sda_bit
		ret
		sbis __i2c_pin,__scl_bit
		ret
		rcall __i2c_delay1	
		sbi  __i2c_dir,__sda_bit
		rcall __i2c_delay1
		sbi  __i2c_dir,__scl_bit
		ldi  r30,1
		rjmp __i2c_delay1

_i2c_stop:
		sbi  __i2c_dir,__sda_bit
		sbi  __i2c_dir,__scl_bit
		rcall __i2c_delay2
		cbi  __i2c_dir,__scl_bit
		rcall __i2c_delay1
		cbi  __i2c_dir,__sda_bit
		rjmp __i2c_delay2

_i2c_read:
		ldi  r23,8
	__i2c_read0:
		cbi  __i2c_dir,__scl_bit
		rcall __i2c_delay1
	__i2c_read3:
		sbis __i2c_pin,__scl_bit	;ak PIND SCL bit je 1, skoc
		rjmp __i2c_read3
		rcall __i2c_delay1
		clc
		sbic __i2c_pin,__sda_bit	;ak PIND SDA bit je 0, skoc
		sec
		sbi  __i2c_dir,__scl_bit
		rcall __i2c_delay2
		rol  data
		dec  r23
		brne __i2c_read0
		tst  r23
		brne __i2c_read1
		cbi  __i2c_dir,__sda_bit
		rjmp __i2c_read2
	__i2c_read1:
		sbi  __i2c_dir,__sda_bit
	__i2c_read2:
		rcall __i2c_delay1
		cbi  __i2c_dir,__scl_bit
		rcall __i2c_delay2
		sbi  __i2c_dir,__scl_bit
		rcall __i2c_delay1
		cbi  __i2c_dir,__sda_bit
		rjmp __i2c_delay1

_i2c_write:
		ldi  r23,8
	__i2c_write0:
		lsl  data
		brcc __i2c_write1
		cbi  __i2c_dir,__sda_bit
		rjmp __i2c_write2
	__i2c_write1:
		sbi  __i2c_dir,__sda_bit
	__i2c_write2:
		rcall __i2c_delay2
		cbi  __i2c_dir,__scl_bit
		rcall __i2c_delay1
	__i2c_write3:
		sbis __i2c_pin,__scl_bit
		rjmp __i2c_write3
		rcall __i2c_delay1
		sbi  __i2c_dir,__scl_bit
		dec  r23
		brne __i2c_write0
		cbi  __i2c_dir,__sda_bit
		rcall __i2c_delay1
		cbi  __i2c_dir,__scl_bit
		rcall __i2c_delay2
		ldi  r30,1
		sbic __i2c_pin,__sda_bit
		clr  r30
		sbi  __i2c_dir,__scl_bit
	ret

_ds1307_write:
		rcall _i2c_start
		ldi data,0xd0
		rcall _i2c_write
		mov data,addr
		rcall _i2c_write
		mov data,buf_data
		rcall _i2c_write
		rcall _i2c_stop
	ret

_ds1307_read:
		rcall _i2c_start
		ldi data,0xd0
		rcall _i2c_write
		mov data,addr
		rcall _i2c_write
		rcall _i2c_start
		ldi data,0xd1
		rcall _i2c_write
		rcall _i2c_read
		mov buf_data,data
		rcall _i2c_stop
	ret
