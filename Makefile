all:
	i686-elf-as boot.s -o boot.o
	i686-elf-gcc -c *.c -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T linker.ld -o narf.bin -ffreestanding -O2 -nostdlib boot.o kernel.o memory.o string.o video.o math.o -lgcc

install: all
	cp narf.bin isodir/boot/narf.bin
	grub2-mkrescue -o narf.iso isodir

run: install
	qemu-system-x86_64 narf.iso

clean:
	rm *.o
	rm narf.bin
	rm narf.iso
	rm isodir/boot/narf.bin

