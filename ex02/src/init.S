;ARM920T supports seven modes of operation:
;  User (usr): The normal ARM program execution state
;  FIQ (fiq): Designed to support a data transfer or channel process
;  IRQ (irq): Used for general-purpose interrupt handling
;  Supervisor (svc): Protected mode for the operating system
;  Abort mode (abt): Entered after a data or instruction prefetch abort
;  System (sys): A privileged user mode for the operating system
;  Undefined (und): Entered when an undefined instruction is executed



;Pre-defined constants
USERMODE    EQU 	0x10; 10000
FIQMODE     EQU 	0x11; 10001
IRQMODE     EQU 	0x12; 10010
SVCMODE     EQU 	0x13; 10011
ABORTMODE   EQU 	0x17; 10111
UNDEFMODE   EQU 	0x1b; 11011
SYSMODE 	EQU 	0x1f; 11111

MODEMASK    EQU 	0x1f

NOINT       EQU 	0xc0
;  76  5 43210
;  11  0 00000

;The location of stacks
_STACK_BASEADDRESS	EQU 0xFFFF0000
;0x10000 = 1024 * 1024
UserStack		EQU	(_STACK_BASEADDRESS - 0x100000*1)
SVCStack		EQU	(_STACK_BASEADDRESS - 0x100000*2)
UndefStack		EQU	(_STACK_BASEADDRESS - 0x100000*3)
AbortStack		EQU	(_STACK_BASEADDRESS - 0x100000*4)
IRQStack		EQU	(_STACK_BASEADDRESS - 0x100000*5)
FIQStack		EQU	(_STACK_BASEADDRESS - 0x100000*6)
SYSStack		EQU	(_STACK_BASEADDRESS - 0x100000*7)
; S6LED
; Eight colored boxes indicate the state of the VE User LEDs.
; These represent LEDs D[21:14] on the VE hardware, which
;   are mapped to bits [7:0] of the SYS_LED register at address
;   0x1C010008. The boxes correspond to the red/yellow/green
;   LEDs on the VE hardware.

LED 	EQU 0x1C010008


	preserve8
	AREA init__, CODE, READONLY
	ENTRY
__ENTRY

	import start
	import xadd
	export init__
	export __ENTRY

	bl initStacks

;task1
	ldr r0, =0x80014000
	ldr r1, =0xF6F6F6F6
	ldr r2, =4
	ldr r3, =0x80016000

loop1
	str r1, [r0]
	add r0, r0, r2
	cmp r0, r3
	bne loop1

;task2
	ldr r0, =0x80014000
	ldr r1, =0x80018000
	ldr r2, =0
	ldr r3, =0x80016000
	sub r5, r3, r0

loop2
	ldr r4, [r0, r2]
	str r4, [r1, r2]
	add r2, r2, #4
	cmp r2, r5
	bne loop2

	ldr r0, =LED  		; r0 = LED

	ldr r1, =0x100		; r1 = 0x100
0
	mov r1,r1,lsr #1	; r1 = r1 >> 1
	str r1,[r0]			; m[r0] = r1
	cmp r1, #0x0
	bne %B0

	bl start
	ldr r0,=msg
	bl xadd

0
	nop
	b %B0
	align
	gbls version
version sets "this is a long story"
	align
msg dcb "it's a trial\r\n"



	align
initStacks
	mrs	r0,cpsr
	; Move cpsr to r0
	; cpsr / spsr = current / saved program status register

	bic	r0,r0,#MODEMASK		; r0 = BIC(r0, #MODEMASK)
	orr	r1,r0,#UNDEFMODE :OR: NOINT		; r1 = r0 | (#UNDEFMODE | NOINT)
	msr	cpsr_cxsf,r1		;UndefMode
	ldr	sp,=UndefStack		; UndefStack=0x33FF_5C00

	bic	r0,r0,#MODEMASK :OR: NOINT
	orr	r1,r0,#ABORTMODE :OR: NOINT
	msr	cpsr_cxsf,r1		;AbortMode
	ldr	sp,=AbortStack		; AbortStack=0x33FF_6000

	bic	r0,r0,#MODEMASK :OR: NOINT
	orr	r1,r0,#IRQMODE :OR: NOINT
	msr	cpsr_cxsf,r1		;IRQMode
	ldr	sp,=IRQStack		; IRQStack=0x33FF_7000

	bic	r0,r0,#MODEMASK :OR: NOINT
	orr	r1,r0,#FIQMODE :OR: NOINT
	msr	cpsr_cxsf,r1		;FIQMode
	ldr	sp,=FIQStack		; FIQStack=0x33FF_8000

	bic	r0,r0,#MODEMASK :OR: NOINT
	orr	r1,r0,#SVCMODE
	msr	cpsr_cxsf,r1		;SVCMode
	ldr	sp,=SVCStack		; SVCStack=0x33FF_5800

	mov	pc,lr

delay
	export delay

	mov pc,lr

	END
