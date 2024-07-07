all: compil_boot disk run
CC=gcc
CFLAGS=-c -nostartfiles -fleading-underscore -nostdlib -I src/include/ -m32 -fno-pic
DSRCDIR=src/drivers/
DODIR=obj/drivers/
compil_boot:
	@echo " ===>Compiling..."
	@echo -n "NASM: Compiling bootloader..."
	@nasm src/boot.asm -o bin/boot.bin -fbin
	@nasm src/mboot32.asm -o bin/kernel/SYSTEM/MBOOT32.SEF -fbin
	@nasm src/loader.asm -o obj/loader1.o -f elf
	printf "#define KERNEL_VERSION \"ALPHA-0.2-ia32\"\n#define KERNEL_BUILD \"$(shell date +"%d.%m %Y %H:%M")\"">src/include/ver.h
	@ia16-elf-gcc src/loader.c -c -o obj/loader2.o -nostartfiles -fleading-underscore -nostdlib -I src/include16/
	@$(CC) src/kernel.c $(CFLAGS) -o obj/krnl2.o
	@$(CC) $(DSRCDIR)vga.c -o $(DODIR)vga.o $(CFLAGS)
	@$(CC) $(DSRCDIR)memory.c -o $(DODIR)memory.o $(CFLAGS)
	@$(CC) $(DSRCDIR)ports.c -o $(DODIR)ports.o $(CFLAGS)
	@$(CC) $(DSRCDIR)picapic.c -o $(DODIR)picapic.o $(CFLAGS)
	@$(CC) $(DSRCDIR)idt.c -o $(DODIR)idt.o $(CFLAGS)
	@nasm src/kernel.asm -o obj/krnl1.o -f elf32
	@ld obj/krnl1.o obj/krnl2.o obj/drivers/vga.o $(DODIR)idt.o $(DODIR)picapic.o obj/drivers/ports.o obj/drivers/memory.o -o bin/kernel/SYSTEM/KERNEL.SEF -Tlink.ld -melf_i386
	@ia16-elf-ld obj/loader1.o obj/loader2.o -o bin/kernel/MBOOT.SEF -Tlink2.ld
	@dd if=/dev/urandom of=bin/kernel/SYSTEM/MURENIUM/BUILD.BIN bs=1024 count=1
	@echo ok
	@echo "Complete!"
disk:compil_boot
	@echo " ===>Seting disk image..."
	@echo -n "Coping disk image..."
	@cp disk/fs.img disk/os.img
	@echo "ok"
	@echo "Installing system..."
	@sudo mount disk/os.img disk/disk
	@sudo cp bin/kernel/* disk/disk/ -r
	@sudo umount disk/disk
	@echo -n "Installing bootloader to disk image..."
	@dd if=bin/boot.bin of=disk/os.img conv=notrunc
	@echo "ok"
	@echo "Complete!"
run:
	@echo " ===>Running system in qemu vm..."
	@qemu-system-i386 -hda disk/os.img -fda disk/sysdata.ima -nic none -m 32 -debugcon stdio
	@echo "All complted, system is ready."
clean:
	@echo " ===>Cleaning..."
	@echo -n "Removing disk image..."
	@rm disk/os.img
	@echo "ok"
	@echo -n "Removing binaris..."
	@rm bin/*
	@rm bin/kernel/*
	@echo "ok"
	@echo -n "Removing objactive..."
	@rm obj/*
	@echo "ok"
	@echo "Complete!"
	
install:
	@echo " ===>Intsalling applications..."
	@echo You need start it as root.
	@echo
	@echo Updating your system...
	@apt update
	@apt upgrade
	@echo
	@echo Installing nasm...
	@apt install nasm
	@echo
	@echo Installing dd...
	@apt install dd
	@echo
	@echo Installing gcc...
	@apt install gcc
	@echo
	@echo Installing qemu...
	@apt install qemu-system-x86
	@echo
	@echo Complete!
uninstall:
	@echo " ===>Unintsalling applications..."
	@echo You need start it as root.
	@echo
	@echo Updating your system...
	@apt update
	@apt upgrade
	@echo
	@echo Uninstalling nasm...
	@apt remove nasm
	@echo
	@echo Uninstalling dd...
	@apt remove dd
	@echo
	@echo Uninstalling gcc...
	@apt remove gcc
	@echo
	@echo Uninstalling qemu...
	@apt remove qemu-system
	@echo
	@echo Complete!
