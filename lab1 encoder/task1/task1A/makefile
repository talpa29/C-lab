./all:exec

exec: encoder.o
	gcc -g -m32 -Wall -o enc encoder.o

encoder.o: encoder.c
	gcc -g -m32 -Wall -c -o encoder.o encoder.c

.PHONY: clean
clean:
	rm -f *.o enc
