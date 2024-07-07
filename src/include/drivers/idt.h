typedef struct{
    word offset1;
    word selecotor;
    byte reserved;
    byte attr;
    word offset2;
} __attribute__((__packed__)) InterruptDescriptor32;

typedef struct {
    word limit;
    dword base;
} __attribute__((packed)) idtr_t;
