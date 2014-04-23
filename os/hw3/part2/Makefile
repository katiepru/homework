CC = gcc
CFLAGS = -ggdb -Wall

all : test

mypthread.o: mypthread.c
	$(CC) $(CFLAGS) -c mypthread.c

test: test.c mypthread.o
	$(CC) $(CFLAGS) test.c mypthread.o -o test

sort: sort.c mypthread.o
	$(CC) $(CFLAGS) sort.c mypthread.o -o sort


clean :
	@rm -f test sort
	@rm -f mypthread.o
