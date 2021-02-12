nasm -f bin -o boot.bin src/boot.asm
nasm -f bin -o loader.bin src/loader.asm
nasm -f elf64 -o kernel.o src/kernel.asm
nasm -f elf64 -o trap.o src/trap.asm
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/main.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/string.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/idt.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/videobuffer.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/ioport.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/pic.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/math.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/debug.c
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -I include/ -c src/mm.c
ld -nostdlib -T link.lds -o kernel kernel.o main.o string.o trap.o idt.o videobuffer.o ioport.o pic.o math.o debug.o mm.o
objcopy -O binary kernel kernel.bin
dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
dd if=loader.bin of=boot.img bs=512 count=5 seek=1 conv=notrunc
dd if=kernel.bin of=boot.img bs=512 count=100 seek=6 conv=notrunc
