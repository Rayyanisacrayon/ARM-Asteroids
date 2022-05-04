; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
	PRESERVE8
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
; R0=0,    then output "0"
; R0=3,    then output "3"
; R0=89,   then output "89"
; R0=123,  then output "123"
; R0=9999, then output "9999"
; R0=4294967295, then output "4294967295"

fp RN 11
COUNT EQU 0
NUM EQU 4 
LCD_OutDec
	PUSH{R11, LR, R4-R9}
	SUB sp, #8
	MOV fp, sp
	STR R0, [fp, #NUM]
	MOV R4, #0
	STR R4, [fp, #COUNT]
	MOV R5, #10
	LDR R4, [fp, #NUM]
	CMP R4, #0
	BEQ zerocase
dLOOP
	LDR R4, [fp, #NUM]
	CMP R4, #0
	BEQ Next
	UDIV R6, R4, R5
	MUL R7, R6, R5
	SUB R8, R4, R7
	PUSH {R8}
	STR R6, [fp, #NUM]
	LDR R6, [fp, #COUNT]
	ADD R6, #1
	STR R6, [fp, #COUNT]
	B dLOOP
zerocase
	PUSH {R4}
	LDR R6, [fp, #COUNT]
	ADD R6, #1
	STR R6, [fp, #COUNT]
	B Next
Next
	LDR R4, [fp, #COUNT]
	CMP R4, #0
	BEQ Done
	POP {R0}
	ADD R0, #0x30
	BL ST7735_OutChar
	SUB R4, #1
	STR R4, [FP, #COUNT]
	B Next
Done
	ADD sp, #8 ;deallocate
	POP{R4-R9, R11, LR}
      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000"
;       R0=3,    then output "0.003"
;       R0=89,   then output "0.089"
;       R0=123,  then output "0.123"
;       R0=9999, then output "9.999"
;       R0>9999, then output "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
;

fp RN 11
LCD_OutFix
	PUSH{R4-R9, R11, LR}
	
	SUB sp, #8 ;set up stack
	MOV fp, sp ;set up frame pointer
	STR R0, [fp, #NUM] ;store number on stack
	MOV R4, #0
	STR R4, [fp, #COUNT] ;store count of 0 on stack
	MOV R5, #10000
	LDR R4, [fp, #NUM]
	CMP R4, R5
	BHS toobig
	MOV R5, #10
floop
	LDR R4, [fp, #COUNT]
	CMP R4, #5
	BEQ fNext
	LDR R4, [fp, #NUM]
	UDIV R6, R4, R5
	MUL R7, R6, R5
	SUB R8, R4, R7
	PUSH {R8}
	STR R6, [fp, #NUM]
	LDR R6, [fp, #COUNT]
	ADD R6, #1
	STR R6, [fp, #COUNT]
	CMP R6, #3
	BNE floop 
	
	
	MOV R8, #46 ;output period
	SUB R8, #0x30
	PUSH {R8}
	LDR R6, [fp, #COUNT]
	ADD R6, #1
	STR R6, [fp, #COUNT]
	B floop
	
	;branch to done

	
toobig ;number exceeds 10000
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #46
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar
	B fdone
	
fNext
	LDR R4, [fp, #COUNT]
	CMP R4, #0
	BEQ fdone
	POP {R0}
	ADD R0, #0x30
	BL ST7735_OutChar
	SUB R4, #1
	STR R4, [FP, #COUNT]
	B fNext
fdone
	ADD sp, #8 ;deallocate
	POP{R4-R9, R11, LR}
     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
