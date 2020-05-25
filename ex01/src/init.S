USERMODE    EQU 	0x10; 10000
FIQMODE     EQU 	0x11; 10001
IRQMODE     EQU 	0x12; 10010
SVCMODE     EQU 	0x13; 10011
ABORTMODE   EQU 	0x17; 10111
UNDEFMODE   EQU 	0x1b; 11011
SYSMODE 	EQU 	0x1f; 11111

MODEMASK    EQU 	0x1f

NOINT       EQU 	0xc0


_STACK_BASEADDRESS	EQU 0xFFFF0000

UserStack		EQU	(_STACK_BASEADDRESS - 0x100000*1)
SVCStack		EQU	(_STACK_BASEADDRESS - 0x100000*2)
UndefStack	EQU	(_STACK_BASEADDRESS - 0x100000*3)
AbortStack	EQU	(_STACK_BASEADDRESS - 0x100000*4)
IRQStack		EQU	(_STACK_BASEADDRESS - 0x100000*5)
FIQStack		EQU	(_STACK_BASEADDRESS - 0x100000*6)
SYSStack		EQU	(_STACK_BASEADDRESS - 0x100000*7)

LED 	EQU 0x1C010008


	preserve8
	AREA init__, CODE, READONLY
	ENTRY
__ENTRY

	export init__
	export __ENTRY


	bl initStacks

	ldr r0, =LED

	ldr r1, =0x1
0
	str r1,[r0]
	mov r1,r1,lsl #1
	cmp r1,#0x100
	bne %B0

0
	nop
	b %B0
	align
	gbls version
version sets "this is a long story"
	align
msg dcb "it's a trial"


0
	nop
	b %B0

initStacks
	mrs	r0,cpsr

	bic	r0,r0,#MODEMASK
	orr	r1,r0,#UNDEFMODE :OR: NOINT
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
