all:shttp.o main.o
	gcc -o shttp main.o shttp.o

CFLAGS=-Wall -g

clean:
	rm -f shttp *.o
