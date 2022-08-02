all: exec

exec: LineParser.c task1.c
	gcc -m32 -Wall -c LineParser.c -o LineParser.o
	gcc -m32 -Wall -c task1.c -o task1.o
	gcc -m32 -g -Wall -o myshell task1.o LineParser.o



.PHONY: clean
clean:
	rm -rf ./*.o myshell
