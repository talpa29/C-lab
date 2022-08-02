all: myELF

myELF: skeleton.o
	ld -m elf_i386 skeleton.o -o myELF

skeleton.o: skeleton.s
	nasm -f elf32 skeleton.s -o skeleton.o


.PHONY: clean
clean:
	rm -f *.o myELF
