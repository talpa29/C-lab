./all: exec

exec: count-words.o 
	gcc -m32 -g -Wall -o task0 count-words.o

count-words.o: count-words.c
	gcc -g -m32 -Wall  -c -g count-words.c -o count-words.o


.PHONY: clean
clean:
	rm -f *.o task0
