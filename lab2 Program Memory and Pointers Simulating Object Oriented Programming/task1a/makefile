./all: exec

exec: addresses[1].o 
	gcc -m32 -g -Wall -o task0 addresses[1].o

addresses[1].o: addresses[1].c
	gcc -g -m32 -Wall  -c -g addresses[1].c -o addresses[1].o


.PHONY: clean
clean:
	rm -f *.o task0
