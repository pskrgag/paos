[BITS 16]
[ORG 0x7e00]

start:
	mov [DriveId],dl

	mov eax,0x80000000
	cpuid
	cmp eax,0x80000001
	jb NotSupport

	mov eax,0x80000001
	cpuid
	test edx,(1 << 29)
	jz NotSupport

LoadKernel:
	mov si, ReadPacket
	mov dl, [DriveId]
	mov ah, 0x42
	int 0x13
	jc ReadError

GetMemoryInfoSetup:
	mov eax, 0xe820
	mov edx, 0x534d4150
	mov ecx, 20
	mov dword [0x9000],0
	mov edi, 0x9008
	xor ebx, ebx
	int 0x15
	jc NotSupport

GetMemoryInfo:
	add edi, 20
	inc dword [0x9000]
	test ebx, ebx
	jz StopMemInfo
	
	mov eax, 0xe820
	mov edx, 0x534d4150
	mov ecx, 20
	int 0x15
	jnc GetMemoryInfo

StopMemInfo:

TestA20:
	mov ax, 0xffff
	mov es, ax
	mov word [ds:0x7c00], 0xbeef
	cmp word [es:0x7c10], 0xbeef
	jne IsEnabled
	mov word [ds:0x7c00], 0xdead
	cmp word [es:0x7c10], 0xdead
	je End

IsEnabled:
	xor ax, ax
	mov es, ax

SetTextMode:
	mov eax, 0x3
	int 0x10

	cli
	lgdt [GdtPtr]
	lidt [IdtPtr]

	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 8:ProtecdeModeEntry

NoFPU:
ReadError:
NotSupport:
End:
	hlt
	jmp End

[BITS 32]
ProtecdeModeEntry:
	mov byte [0xb8000], 'P'
	mov byte [0xb8001], 0xa

	mov ax, 0x10 ; 16
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov esp, 0x7c00

	cld
	mov edi, 0x80000
	xor eax, eax
	mov ecx, 0x10000/4
	rep stosd

	mov dword [0x80000], 0x81007
	mov dword [0x81000], 10000111b

	lgdt [GtdPtr64]

	mov eax, cr4
	or eax, (1 << 5)
	mov cr4, eax

	mov eax, 0x80000
	mov cr3, eax

	mov ecx, 0xc0000080
	rdmsr
	or eax, (1 << 8)
	wrmsr

	mov eax, cr0
	or eax, (1 << 31)
	mov cr0, eax

	jmp 8:LongMode

[BITS 64]
LongMode:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov rsp, 0x7c000

	cld
	mov rdi, 0x200000
	mov rsi, 0x10000
	mov rcx, 512 * 100 / 8
	rep movsq

	jmp 0x200000

DriveId:	db 0
Message:	db "Long Mode is enabled"
MessageLen:	equ $-Message
ReadPacket:
	db 0x10
	db 0x00
	dw 0x0064 ; 100
	dw 0x0000
	dw 0x1000
	db 6	; 6 sector
	times 7 db 0x0000

Gtd:
	dq 0x00
Code:
	dw 0xffff
	dw 0x0000
	db 0x00
	db 0x9a
	db 0xcf
	db 0x00
Date:
	dw 0xffff
	dw 0x0000
	db 0x00
	db 0x92
	db 0xcf
	db 0x00

GtdLen:	equ $ - Gtd
GdtPtr:	
	dw GtdLen - 1
	dd Gtd

IdtPtr:
	dw 0x00
	dd 0x00


Gtd64:
	dq 0x00
	dq 0x0020980000000000

GtdLen64:	equ $ - Gtd64

GtdPtr64:
	dw GtdLen64 - 1
	dd Gtd64
