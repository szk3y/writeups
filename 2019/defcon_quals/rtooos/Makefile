all:
	gcc -o emu -std=gnu11 -g emu.c
	nasm table.asm
	nasm shellcode.asm
	nasm stager.asm
	nasm code.asm
