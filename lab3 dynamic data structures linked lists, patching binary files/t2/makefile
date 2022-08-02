all: task2b

exec: task2b.o 
	gcc -m32 -g -Wall -o run task2b.o

prog.o: task2b.c
	gcc -g -m32  -Wall  -c -g task2b.c -o task2b.o


.PHONY: clean
clean:
	rm -f *.o task2b
