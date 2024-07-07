#include <mtypes.h>
#include <drivers/ports.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

void wait() {}

void InitPIC() {
    byte m1 = inb(PIC1_DATA);
    byte m2 = inb(PIC2_DATA);
    outb(PIC1_COMMAND, 0x11);
    wait();
    outb(PIC2_COMMAND, 0x11);
    wait();
    outb(PIC1_DATA, 0x20);
    wait();
    outb(PIC2_DATA, 0x28);
    wait();
    outb(PIC1_DATA, 4);
    wait();
    outb(PIC2_DATA, 2);
    wait();
    outb(PIC1_DATA, 1);
    wait();
    outb(PIC2_DATA, 1);
    wait();
    outb(PIC1_DATA, 0);
    wait();
    outb(PIC2_DATA, 0);
    wait();
    outb(PIC1_DATA, m1);
    wait();
    outb(PIC2_DATA, m2);
    wait();
}
void SendEOI(byte irq) {
	if(irq >= 8) outb(PIC2_COMMAND, 0x20);
	outb(PIC1_COMMAND, 0x20);
}

void MaskIRQ(byte IRQline) {
    word port;
    byte value;
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}
void DisablePIC() {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}