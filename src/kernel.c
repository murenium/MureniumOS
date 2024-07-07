#include <mtypes.h>
#include <drivers/vga.h>
#include <drivers/mem.h>
#include <drivers/idt.h>
#include <drivers/picapic.h>
#include <drivers/ports.h>
#include <sys.h>
#include <ver.h>
#include <libc.h>

void kmain() {
    ClearScreen();
    print("MureniumOS kernel version: ");
    print(KERNEL_VERSION);
    print("\nBuild: ");
    print(KERNEL_BUILD);
    print("\nMurenium(c) 2022-2024, all not rights reserved.");
    print("\n __  __                _\n");
    print("|  \\/  |_   _ _ __ ___(_)_   _ _ __ ___\n");
    print("| |\\/| | | | | '__/ _ \\ | | | | '_ ` _ \\\n");
    print("| |  | | |_| | | |  __/ | |_| | | | | | |\n");
    print("|_|  |_|\\__,_|_|  \\___|_|\\__,_|_| |_| |_|\n");
    print("GDT initased.\nDetecting memory...");
    dword mem = (DetectMemory()*4)/1024;
    char memstr[4];
    w2str(memstr, mem);
    print("\nDetected: ");
    print(memstr);
    print("MB\n");
    print("Initialization IDT...");
    idt_init();
    print("\nInitialization PIC...");
    InitPIC();
    DisablePIC();
    outb(0x80, 0);
    __asm__("sti");
    print("\nOS is started!\n");
    outb(0x3d4, 0xff);
    outb(0x3C4, 0);
    outb(0x3Ce, 0);
    halt();
}