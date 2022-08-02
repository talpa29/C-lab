./all: exec

exec: add.o main.o numbers.o
	gcc -m32 -g -Wall -o task0 add.o main.o numbers.o

numbers.o: numbers.c
	gcc -g -m32 -Wall  -c  numbers.c -o numbers.o

main.o: main.c
	gcc -g -m32 -Wall -ansi -c main.c -o main.o

add.o: add.s
	nasm -g -f elf -w+all -o add.o add.s

.PHONY: clean
clean:
	rm -f *.o task0
