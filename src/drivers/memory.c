#include <mtypes.h>
#include <drivers/mem.h>

word DetectMemory() {
    byte *memptr = 0x400000;
    memptr[0] = 0xEE;
    while (memptr[0] == 0xEE) {
        memptr[0] = 0;
        memptr+=BLOCK;
        if(memptr >= 134217728) break;
        memptr[0] = 0xEE;
    }
    dword mem = memptr;
    return mem/BLOCK;
}