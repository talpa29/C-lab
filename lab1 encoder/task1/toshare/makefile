./all: exec

exec: encoder.o encoderer.o encoderB.o encoderC.o 
	gcc -m32 -g -Wall -o enc encoder.o encoderer.o encoderB.o encoderC.o 

encoder.o: encoder.c
	gcc -g -m32 -Wall  -c  encoder.c -o encoder.o

encoderer.o: encoderer.c
	gcc -g -m32 -Wall -ansi -c encoderer.c -o encoderer.o
	
encoderB.o: encoderB.c
	gcc -g -m32 -Wall  -c  encoderB.c -o encoderB.o
	
encoderC.o: encoderC.c
	gcc -g -m32 -Wall  -c  encoderC.c -o encoderC.o

	


.PHONY: clean
clean:
	rm -f *.o enc
