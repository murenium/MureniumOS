#include <mtypes.h>
#include <drivers/idt.h>
#include <drivers/vga.h>

#define true 1
#define false 0

extern word CODESEG;

InterruptDescriptor32 idt[0xff];
idtr_t idtr;
extern void inttest();

void idt_set_descriptor(byte vector, void* isr, byte flags) {
    InterruptDescriptor32* descriptor = &idt[vector];
    descriptor->offset1 = (dword)isr & 0xFFFF;
    descriptor->selecotor = CODESEG;
    descriptor->attr = flags;
    descriptor->reserved = 0;
    descriptor->offset2 = (dword)isr >> 16;
}

void exce() {
    TextColor = 0x0F;
    print("exception#??");
    //__asm__("iret");
    __asm__("cli; hlt");
}
void SysInt() {
    print("SYS INT\n");
}

void exc0() {
    TextColor = 0x0F;
    print("exception#DE");
    //__asm__("iret");
    __asm__("cli; hlt");
}
void exc1() {
    TextColor = 0x0F;
    print("exception#DB");
    //__asm__("iret");
    __asm__("cli; hlt");
}
void exc3() {
    TextColor = 0x0F;
    print("exception#BP");
    //__asm__("iret");
    __asm__("cli; hlt");
}
void exc4() {
    TextColor = 0x0F;
    print("exception#OF");
    //__asm__("iret");
    __asm__("cli; hlt");
}
void exc5() {
    TextColor = 0x0F;
    print("exception#BR");
    //__asm__("iret");
    __asm__("cli; hlt");
}
void exc6() {
    TextColor = 0x0F;
    print("exception#UD");
    //__asm__("iret");
    __asm__("cli; hlt");
}
void exc7() {
    TextColor = 0x0F;
    print("exception#NM");
    //__asm__("iret");
    __asm__("cli; hlt");
}
void exc8() {
    TextColor = 0x0F;
    print("exception#DF");
    //__asm__("iret");
    __asm__("cli; hlt");
}

void idt_init() {
    idtr.base = (dword)&idt[0];
    idtr.limit = (word)sizeof(InterruptDescriptor32) * 254;
 
    for (byte vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, *exce, 0x8E);
    }
    idt_set_descriptor(0, *exc0, 0x8E);
    idt_set_descriptor(3, *exc3, 0x8E);
    idt_set_descriptor(4, *exc4, 0x8E);
    idt_set_descriptor(5, *exc5, 0x8E);
    idt_set_descriptor(6, *exc6, 0x8E);
    idt_set_descriptor(7, *exc7, 0x8E);
    idt_set_descriptor(8, *exc8, 0x8E);
    idt_set_descriptor(2, *SysInt, 0x8E);
    for (byte vector = 32; vector < 0xff; vector++) {
        idt_set_descriptor(vector, *inttest, 0x8E);
    }
    __asm__("lidt %0" : : "m"(idtr));
}

void inttestc() {
    print(" INT ");
    SendEOI(1);
}