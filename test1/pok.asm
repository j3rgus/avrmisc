 ;********************************************************************************************  
 ;*                              PROGRAM_BLIK03.ASM;                                         *  
 ;*  Procesor ATMEGA16 - DIL                                                                 *  
 ;********************************************************************************************  
   
 ;--------------------------------------------  
 ; popis fce registru:  
 ;   -> R19 - pomocny registr (platny lokalne)  
 ;   -> R1  - slouzi pro pocitadlo preruseni  
 ;   -> R2  - slouzi pro pocitadlo preruseni  
   
   
         .NOLIST  
         .INCLUDE "tn2313def.inc"  
         .LIST  
   
   
         ; PRIRAZENI JMEN PINUM, PORTUM A REGISTRUM   
         .CSEG  
   
         .ORG    0  
         RJMP    RESET  
         .ORG    OC0addr  
         RJMP    PRERUSENI  
   
         .DEF    TEMP       = R19  
         .DEF    INT_CNT = R2  
   
         .EQU    BLIK_PORT_DRIVE = DDRC  
         .EQU    BLIK_PORT_DATA  = PORTC  
         .EQU    BLIK_PIN = PORTC0  
   
           
         ; NASTAVENI ZASOBNIKU  
 RESET:          LDI         R16,LOW(RAMEND)  
         OUT     SPL,R16  
         LDI     R16,HIGH(RAMEND)  
         OUT     SPH,R16  
   
         ; NASTAVENI CASOVACE_0 A JEHO PRERUSENI  
         LDI     R19,12       ; nastavi zdroj signalu   
         OUT     TCCR0,R19    ; casovace 0 na 31250Hz  
         LDI     R19,250      ; nastavi porovnavanou  
         OUT     OCR0,R19     ; hodnotu na 250 (preruseni 125/s)  
         LDI     R19,02       ; povoli preruseni,  
         OUT     TIMSK,R19    ; kdyz TCNT0=OCR0.  
                   
         ; NASTAVENI REGISTRU  
         CLR     INT_CNT      ; smaze pocitadlo preruseni  
         CLR     INT_CNT      ; smaze pocitadlo preruseni  
   
                 LDI     TEMP,255  
         OUT     BLIK_PORT_DRIVE,TEMP  
   
 // ************************ HLAVNI PROGRAM ********************** //  
   
   
 ZACATEK_PROGRAMU:  
   
         SEI  
         // tady neco muzeme delat  
   
     RJMP    ZACATEK_PROGRAMU    ; skoci na zacatek (stale dokola)  
   
 // ************************ PODPOROGRAMY ************************ //  
   
   
   
 // ****************** OBSLUHY PRERUSENI *********************** //  
 PRERUSENI:                                                
         INC INT_CNT     ; inkrementuje R2  
         LDI TEMP,125    
                 CP  R2,TEMP     ; je-li R2 mensi   
         BRLO    PRYC_LABEL  ; nez TEMP jde pryc                                                                      
         CLR INT_CNT  
           
         SBIC    BLIK_PORT_DATA,BLIK_PIN  
         RJMP    CLEAR_PIN_LABEL  
         SBI     BLIK_PORT_DATA,BLIK_PIN  
                 RJMP    PRYC_LABEL  
   
   
 CLEAR_PIN_LABEL:  
         CBI     BLIK_PORT_DATA,BLIK_PIN       
   
   
 PRYC_LABEL:   
         NOP  
 RETI  
