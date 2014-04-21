CC = gcc
CFLAGS = -ggdb -Wall

all : test

mypthread.o: mypthread.c
	$(CC) $(CFLAGS) -c mypthread.c

test: test.c mypthread.o
	$(CC) $(CFLAGS) test.c mypthread.o -o test


clean :
	@rm -f test
	@rm -f mypthread.o
