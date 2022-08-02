all: task1c

exec: task1c.o 
	gcc -m32 -g -Wall -o task1c task1c.o

prog.o: prog.c
	gcc -g -m32  -Wall  -c -g task1c.c -o task1c.o


.PHONY: clean
clean:
	rm -f *.o task1c
