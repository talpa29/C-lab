./all: exec

exec: task3.o 
	gcc -m32 -g -Wall -o task0 task3.o

task3.o: task3.c
	gcc -g -m32 -Wall  -c -g task3.c -o task3.o


.PHONY: clean
clean:
	rm -f *.o task0
