./all: exec

exec: task1c.o 
	gcc -m32 -g -Wall -o task0 task1c.o

task1c.o: task1c.c
	gcc -g -m32 -Wall  -c -g task1c.c -o task1c.o


.PHONY: clean
clean:
	rm -f *.o task0
