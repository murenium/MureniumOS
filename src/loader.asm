[bits 16]
extern _lmain
extern _disk
extern _CurrentAdres
global _print
global _DapWrite
global _DapRead
global _GetKey
global _MemDump
global _ClearScreen
global _KrnlCopy
global _putc
global _BeginDraw
global start
start:
	mov [_disk], dl
	in     al, 92h
    or     al, 02h
    out    92h, al
   xor ax, ax             ; make it zero
   mov ds, ax             ; DS=0
   mov ss, ax             ; stack starts at seg 0
   mov sp, 0x9c00         ; 2000h past code start, 
                          ; making the stack 7.5k in size
 
   cli                    ; no interrupts
   push ds                ; save real mode
 
   lgdt [gdtinfo]         ; load gdt register
 
   mov  eax, cr0          ; switch to pmode by
   or al,1                ; set pmode bit
   mov  cr0, eax
   jmp 0x8:pmode
 
pmode:
   mov  bx, 0x10          ; select descriptor 2
   mov  ds, bx            ; 10h = 10000b
 
   and al,0xFE            ; back to realmode
   mov  cr0, eax          ; by toggling bit again
   jmp 0x0:unreal
 
unreal:
   pop ds                 ; get back old segment
   sti
   mov eax, 0x100000
   mov byte [eax], 0xff
   call _lmain
   jmp $                  ; loop forever
 
gdtinfo:
   dw gdt_end - gdt - 1   ;last byte in table
   dd gdt                 ;start of table
 
gdt:        dd 0,0        ; entry 0 is always unused
codedesc:   db 0xff, 0xff, 0, 0, 0, 10011010b, 00000000b, 0
flatdesc:   db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
gdt_end:


_print:
	push si
	mov si, ax
    mov ah, 0x0E
    .l
    lodsb
    test al, al
    jz .q
    int 10h
	cmp al, 10
	jnz .l
	mov al, 13
	int 10h
    jmp .l
    .q
	pop si
    ret
_putc:
	mov ah, 0x0E
	int 10h
	ret
_GetKey:
	mov ah, 0
	int 16h
	mov ah, 1
	int 16h
	ret
_DapWrite:
	push si
	push bx
	push es
	mov si, ax
	xor ax, ax
	mov es, ax
	mov bx, 0x9000
	mov ah, 0x43
	mov dl, [_disk]
	mov bh, 0
	int 13h
	pop si
	pop bx
	pop es
	ret
_DapRead:
	push si
	push bx
	push es
	mov si, ax
	xor ax, ax
	mov es, ax
	mov bx, 0x9000
	mov ah, 0x42
	mov dl, [_disk]
	mov bh, 0
	int 13h
	pop si
	pop bx
	pop es
	ret
_MemDump:
	push ax
	mov ax, dm_msg1
	call _print
	mov ah, 3h
	mov al, 1
	mov ch, 0
	mov cl, 1
	mov dh, 0
	mov dl, 0
	xor bx, bx
	mov es, bx
	pop bx
	int 13h
	cli
	hlt
_ClearScreen:
	mov ax, 3
	int 10h
	ret
_KrnlCopy:
	push ebx
	mov ebx, 0x8000
	.l
	mov eax, [_CurrentAdres]
	mov dl, [ebx]
	mov [eax], dl
	inc ebx
	inc dword [_CurrentAdres]
	cmp ebx, 0x8800
	je .q
	jmp .l
	.q
	pop ebx
	ret
_BeginDraw:
	mov ah, 0
	mov al, 13h
	int 10h
	ret
ret_adr dw 0
dm_msg1 db 10, 13,"Dumping memory to disk A:...", 10, 13, "System halted.", 0