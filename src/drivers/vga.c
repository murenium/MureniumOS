#include <drivers/ports.h>

#define VIDEO_ADDRESS 0xB8000
#define VGA_HEIGHT 80
#define VGA_WIDTH 25 

unsigned char *vidptr = (unsigned char *) VIDEO_ADDRESS;
extern unsigned char TextColor = 0x02;
short CurrentLine = 0;
short CurrentRow = 0;

void SyncCursor() {
    if(CurrentRow > 80) {
        CurrentLine++;
        CurrentRow = 0;
    }
    if(CurrentLine >= 25) {
        CurrentLine--;
        char *mpr1 = VIDEO_ADDRESS;
        for (int i = 0; i < 3840; i++) mpr1[i] = mpr1[i+160];
        for (int i = 0xF00; i < 0xFA0; i+=2) mpr1[i] = 0;
    }
    int pos = VGA_HEIGHT * CurrentLine + CurrentRow;
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8)&0xFF);
}

void SetCursorPos(short x, short y) {
    CurrentLine = y;
    CurrentRow = x;
    SyncCursor();
}

void ClearScreen() {
    for (int i = 0; i <= VGA_WIDTH*VGA_HEIGHT*2; i+=2) {
        vidptr[i] = 0;
        vidptr[i+1] = TextColor;
    }
    SetCursorPos(0, 0);
}

void print(char *msg) {
    int i = 0;
    while (msg[i] != '\0') {
        if(msg[i] == '\n') {
            CurrentLine++;
            CurrentRow = 0;
        }
        else {
            vidptr[2*(CurrentRow+CurrentLine*VGA_HEIGHT)] = msg[i];
            vidptr[2*(CurrentRow+CurrentLine*VGA_HEIGHT)+1] = TextColor;
            CurrentRow++;
        }
        SyncCursor();
        i++;
    }
}

void put_c(char c) {
    vidptr[2*(CurrentRow+CurrentLine*VGA_HEIGHT)] = c;
    vidptr[2*(CurrentRow+CurrentLine*VGA_HEIGHT)+1] = TextColor;
    CurrentRow++;
    SyncCursor();
}

void set_c(char c, short x, short y) {
    vidptr[2*(x+y*80)] = c;
    vidptr[2*(x+y*80)+1] = TextColor;
}