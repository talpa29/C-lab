all: mypipeline

exec:  mypipeline.c
	gcc -m32 -Wall -c mypipeline.c -o mypipeline.o
	gcc -m32 -g -Wall -o mypipeline mypipeline.o



.PHONY: clean
clean:
	rm -rf ./*.o mypipeline