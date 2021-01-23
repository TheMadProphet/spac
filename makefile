a.out: a.o
	gcc -no-pie a.o
a.o: a.asm
	nasm -felf64 -g -F dwarf a.asm