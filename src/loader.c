#include "mtypes16.h"
extern void print(char*);
extern putc(char);
extern void debug();  // for debug N1
extern char GetKey();
extern char DapWrite(dap*); // read/write to disk
extern char DapRead(dap*);
extern void MemDump(unsigned short); // for debug N2
extern void ClearScreen();

#include "disk.h"

char disk; // boot disk for int 13h, see loader.asm, line 16

dword CurrentAdres = 0x100000;

extern void KrnlCopy();
extern void BeginDraw(); // for boot logo

void LoadDir() {
    CreateDAP(&test, 5, 0x8000, 0, 
        MainDisk->ReservedSectors + MainDisk->NumberOfFAT * MainDisk->SectorsPerFAT); // root dirictory
    DapRead(&test); // reading
    byte *root = test.offset; // 0x8000 (see include16/mtypes16.h line 6)
    file *mdir = FindFileByName(root, "SYSTEM     "); // finding SYSTEM dirictory in memory at 0x8000
    if(mdir == 0) {
        print("\nERROR: /SYSTEM/KERNEL.SEF - FILE NOT FOUND"); // not found
        return;
    }
    CreateDAP(&test, 5, 0x8000, 0, CalcSectorByCluster(mdir->FirstClusLO)); // Load /system dirictory to 0x8000(main buffer)
    DapRead(&test);
}


void lmain() {
    print("\nMureniumOS bootloader version DEV-0.2\nMurenium (c) 2022-2024. all rigts not reserved.\nLoading kernel...\n/SYSTEM/MBOOT32.SEF");
    LoadDir(); // see line 10
    byte *root = 0x8040; // root locates at main buffer(0x8000) + 32 * 2
    file *mboot32 = FindFileByName(root, "MBOOT32 SEF"); // finding file to switch cpu mode
    // Now code work in real mode, to boot kernel we need to enter protected mode
    if(mboot32 == 0) {
        print(" - ERROR FILE NOT FOUND");
        return;
    }
    CreateDAP(&test, 1, 0x7E00, 0, CalcSectorByCluster(mboot32->FirstClusLO)); // Calculating first cluster(file is very smal)
    DapRead(&test);
    print(" ok");
    print("\n/SYSTEM/MURENIUM/LOGO.BMP skiped"); // don't load logo
    print("\n/SYSTEM/KERNEL.SEF");
    file *kernel = FindFileByName(root, "KERNEL  SEF");
    if(kernel == 0) {
        print(" - ERROR FILE NOT FOUND");
        return; // return = halt computer
    }
    word CurentCluster = kernel->FirstClusLO; // First cluster
    while (CurentCluster <= 0xFFEF) { // if next cluster number in fat table bigger 0xFFEF, we read all file or it's bad cluster(reading is imposible)
        CreateDAP(&test, MainDisk->SectorsPerCluster, 0x8000, 0, 
            CalcSectorByCluster(CurentCluster)); // See disk.h line 16
        DapRead(&test);
        KrnlCopy(); // copy kernel code from 0x8000 to 1M
        CurentCluster = GetNextCluster(CurentCluster);
    }
    asm("jmp 0x7E00"); // Boot /SYSTEM/MBOOT32.SEF
}