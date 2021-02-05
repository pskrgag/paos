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
	mov edi, 0x9000
	xor ebx, ebx
	int 0x15
	jc NotSupport

GetMemoryInfo:
	add edi, 20
	mov eax, 0xe820
	mov edx, 0x534d4150
	mov ecx, 20
	int 0x15
	jc StopMemInfo

	test ebx, ebx
	jnz GetMemoryInfo

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

	mov si, Message
	mov ax, 0xb800
	mov es, ax
	xor di, di
	mov cx, MessageLen

Loop:
	mov al, [si]
	mov [es:di], al
	mov byte[es:di + 1], 0xa
	
	add di, 2
	add si, 1
	loop Loop

ReadError:
NotSupport:
End:
	hlt
	jmp End

DriveId:	db 0
Message:	db "TextMode is set"
MessageLen:	equ $-Message
ReadPacket:
	db 0x10
	db 0x00
	dw 0x0064 ; 100
	dw 0x0000
	dw 0x1000
	db 6	; 6 sector
	times 7 db 0x0000
