section .text
use16
org 0x7C00
; BPB
jmp start
nop
OEM_name db "MURENIUM"
BytesPerSector dw 200h
SectorsPerCluster db 04h
ReservedSectors dw 08h
FATcount db 2
MaxRootEnteries dw 200h
SmallSectors dw 0
MediaDescripter db 0xF8
SectorsPerFat dw 100h
SPT dw 0x3F
NumberOfHeads dw 40h
HiddenSectors dd 1
NumberOfSectors dd 40000h
; EBPB
PartitionNumber dw 80h
db 0
db 29h
SerialNumber dd 0x25101917
VolumeName db "MURENIUM   "
FSName db "FAT16   "
; FS end
start:
	mov ax, cs
	mov ds, ax
	mov es, ax
	cld
	call init_disk
	mov si, msg
	call print;278
	mov bx, 0
	mov es, bx
	mov bx, 0x7000
	movzx ax, byte [FATcount]
	mul word [SectorsPerFat]
	add ax, [ReservedSectors]
	push ax
	movzx eax, ax
	;mov eax, 208h
	call read_disk
	mov bx, 7000h
	.l
	mov si, bx
	mov di, f_name
	mov cx, 11
	repe cmpsb
	jz .c
	cmp bx, 7200h
	jz error
	inc bx
	jmp .l
	
	.c
	add si, 15
	mov ax, [si]
	sub ax, 2
	mov [ffc], ax
	mov al, [ffc]
	mul byte [SectorsPerCluster]
	pop bx
	add ax, bx
	push ax
	mov ax, [MaxRootEnteries]
	mov bx, 32
	mul bx
	mov bx, [BytesPerSector]
	div bx
	mov bx, ax
	pop ax
	add ax, bx
	movzx eax, ax
	mov bx, 0x7000
	mov word [dap_block_count], 4
	call read_disk
	mov dl, [PartitionNumber]
	jmp 0x700:0
	;
	mov ah, 3
	mov bx, 0
	mov es, bx
	mov bx, 0x7000
	mov dl, 0
	mov cl, 1
	mov ch, 0
	mov al, 1
	mov dh, 0
	int 13h
	cmp ah, 0
	jnz error
stop:
	hlt 
	jmp stop
error:
	mov si, error_msg
	call print
	jmp stop
init_disk:
	mov [PartitionNumber], dl
	ret
read_disk:
	mov [dap_sector_low], eax
	mov [dap_segment], es
	mov [dap_offset], bx
	mov ah, 0x42
	mov dl, [PartitionNumber]
	mov si, dap
	int 13h
	ret
debug:
	lodsb
	mov ah, 0x0E
	int 10h
	mov ah, 0
	int 16h
	jmp debug
print:
	mov ah, 0Eh
	.l
	lodsb
	cmp al, 0
	jz .q
	int 10h
	jmp .l
	.q
	ret

;;;;;;;;;
msg db "Starting MureniumOS...", 0
f_name db "MBOOT   SEF"
error_msg db 10, 13, "ERROR!",10,13,"Can't find file /MBOOT.SEF",10,13,"System halted.",0
ffc dw 0
;;;;;;;;;
dap:
	dap_size:		db 0x10		; Size of the data address packet.
	dap_reserved:		db 0x00		; Reserved. Should be 0
	dap_block_count:	dw 0x01		; Number of blocks to read
	dap_offset:		dw 0x0000	; Offset. (Already set with default)
	dap_segment:		dw 0x00		; Segment
	dap_sector_low:		dd 0x01		; Lower 32 bits of sector number
	dap_sector_high:	dd 0x00	
;;;;;;;;;
times 510-$+$$ db 0
db 0x55
db 0xAA