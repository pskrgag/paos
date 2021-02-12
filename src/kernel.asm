section .data

Gtd64:
	dq 0x00
	dq 0x0020980000000000
	dq 0x0020f80000000000	; code (ring 3)
	dq 0x0000f20000000000	; data (ring 3)
TssDescr:
	dw TssLen - 1
	dw 0
	db 0
	db 0x89
	db 0x0
	db 0x0
	dq 0x0

GtdLen64:	equ $ - Gtd64

GtdPtr64:
	dw GtdLen64 - 1
	dq Gtd64

Tss:
	dd 0x00
	dq 0x150000
	times 88 db 0
	dd TssLen

TssLen: equ $ - Tss

section .text
extern main
global start

start:
	lgdt [GtdPtr64]

SetTSS:
	mov rax, Tss
	mov [TssDescr + 2], ax
	shr rax, 16
	mov [TssDescr + 4], al
	shr rax, 8
	mov [TssDescr + 7], al
	shr rax, 8
	mov [TssDescr + 8], eax

	mov ax, 0x20
	ltr ax

InitPIT:
	mov al, 0x36		; https://en.wikibooks.org/wiki/X86_Assembly/Programmable_Interval_Timer
	out 0x43, al		; https://wiki.osdev.org/PIT

	mov ax, 11931
	out 0x40, al
	mov al, ah
	out 0x40, al

InitPIC:
	mov al, 0x11
	out 0x20, al
	out 0xa0, al

	mov al, 32
	out 0x21, al		;PIC1 now starts at 32
	mov al, 40
	out 0xa1, al		;PIC2 now starts at 40

	mov al, 0x04
	out 0x21, al		;setup cascading
	mov al, 0x02
	out 0xa1, al

	mov al, 0x01
	out 0x21, al
	out 0xA1, al

	mov al, 11111110b
	out 0x21, al
	mov al, 11111111b
	out 0xa1, al

	push 0x8
	push KernelEntry
	db 0x48	; REX.w
	retf

KernelEntry:
	xor ax,ax
	mov ss,ax

	mov rsp, 0x200000
	call main
NoFPU:
End:
	hlt
	jmp End
