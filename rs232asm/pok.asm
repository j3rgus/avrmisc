.nolist
.include        "tn2313def.inc"
.list

.cseg
			ldi r16,RAMEND
			out SPL,r16
			rcall USART_Init
main:
			rcall USART_Receive
			rcall USART_Transmit
			rjmp main


USART_Init:
			ldi r16,0x00
			out UCSRA,r16
			ldi r16,0x18
			out UCSRB,r16
			ldi r16,0x06
			out UCSRC,r16
			ldi r16,0x00
			out UBRRH,r16
			ldi r16,0x0c
			out UBRRL,r16
		ret

USART_Transmit:
			sbis UCSRA,UDRE
			rjmp USART_Transmit
			out UDR,r17
		ret

USART_Receive:
			sbis UCSRA, RXC
			rjmp USART_Receive
			in r17, UDR
		ret
