dap test;
dap fat;

bpb *MainDisk = 0x7C00; // first 512 bytes of system disk

void CreateDAP(dap *_dap, word block_count, word offset, word seg, qword sector) { // Create Disk Address Packet(DAP) for int 13h
    _dap->size = 0x10;
    _dap->block_count = block_count;
    _dap->reserved = 0;
    _dap->offset = offset;
    _dap->segment = seg;
    _dap->sector = sector;
    return;
}

qword CalcSectorByCluster(word cluster) {
    return MainDisk->ReservedSectors + MainDisk->NumberOfFAT * MainDisk->SectorsPerFAT + (MainDisk->MaxRootEntries*32) / MainDisk->BytesPerSector + MainDisk->SectorsPerCluster*(cluster-2);
}

file *FindFileByName(const byte *dir, char name[11]) {
    while (!Equal(dir, name, 11)) {
        if (dir[0] == 0) {
            return 0;
        }
        dir+=32;
    }
    return dir;
}
word GetNextCluster(word Cluster) {
    CreateDAP(&fat, 1, 0x8000, 0, MainDisk->ReservedSectors);
    DapRead(&fat);
    word *fat2 = 0x8000;
    return fat2[Cluster];
}