./all: exec

exec: encoder.o encoderer.o 
	gcc -m32 -g -Wall -o enc encoder.o encoderer.o 

encoder.o: encoder.c
	gcc -g -m32 -Wall  -c  encoder.c -o encoder.o

encoderer.o: encoderer.c
	gcc -g -m32 -Wall -ansi -c encoderer.c -o encoderer.o


.PHONY: clean
clean:
	rm -f *.o task0
