org 0x7E00
[bits 16]
start:
	cli
	;hlt
	lgdt[gdt_pointer]
	mov eax, cr0
	or al, 1
	mov cr0, eax
	jmp CODE_SEG:start_pm

gdt_start:
	dq 0x0
	krnl_code:
		dw 0xffff
		dw 0
		db 0
		db 10011010b
		db 11001111b
		db 0x0
	krnl_data:
	    dw 0xffff
    	dw 0x0
    	db 0x0
    	db 10010010b
    	db 11001111b
    	db 0x0
gdt_end:
	gdt_pointer:
		dw gdt_end - gdt_start
		dd gdt_start

CODE_SEG equ krnl_code - gdt_start
DATA_SEG equ krnl_data - gdt_start

[bits 32]
start_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov byte [0xb8F9E], '*'
	mov byte [0xb8F9F], 0x4F
	mov ax, CODE_SEG
	jmp CODE_SEG:0x100000
	hlt