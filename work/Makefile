all:	HelloWorld hello

HelloWorld.o:	HelloWorld.asm
	nasm -f elf32 HelloWorld.asm

HelloWorld:	HelloWorld.o
	ld -s -o HelloWorld HelloWorld.o -melf_i386

clean:
	rm -f *.o HelloWorld hello

hello:	hello.c
	gcc hello.c -o hello
