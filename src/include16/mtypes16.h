typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned long long qword;

typedef struct{
    byte size;
    byte reserved;
    word block_count;
    word offset;
    word segment;
    qword sector;
} __attribute__((__packed__)) dap; // Disk Address Packet struct, see INT 13h AH=42h

typedef struct{
    byte commands[3];
    char OEMname[8];
    word BytesPerSector;
    byte SectorsPerCluster;
    word ReservedSectors;
    byte NumberOfFAT;
    word MaxRootEntries;
    word SmallSectors;
    byte MediaDescriptor;
    word SectorsPerFAT;
    word SectorsPerTrack;
    word NumberOfHeads;
    dword HiddenSectors;
    dword NumberOfSectors;
    word DriveNumber;
    byte NT;
    byte Signature;
    dword SerialNumber;
    char VolumeName[11];
    char FsName[8];
} __attribute__((__packed__)) bpb; // Bios Parameter Block

typedef struct{
    char name[11];
    byte atr;
    byte res;
    byte CreateTime2;
    word CreateTime;
    word CreateDate;
    word LastAccDate;
    word FirstClusHI;
    word ModTime;
    word ModDate;
    word FirstClusLO;
    dword Size;
} __attribute__((__packed__)) file; // file in FAT16/12

char Equal(char *s1, char *s2, int lenth) {
    char res = 1;
    for (int i = 0; i < lenth; i++) if(s1[i] != s2[i]) res = 0;
    return res;
}