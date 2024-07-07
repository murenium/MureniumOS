[bits 32]
extern _kmain
extern _inttestc
extern _SendEOI
global _inttest
global _CODESEG
.text
start:
    mov byte [0xb8000], 'o'
    mov byte [0xb8001], 0x0F
    mov byte [0xb8002], 'k'
    mov byte [0xb8003], 0x0F
    mov [_CODESEG], ax
    jmp _kmain
_inttest:
    pushad
    call _inttestc
    popad
    iret
_CODESEG dw 0