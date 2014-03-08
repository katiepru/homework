CC = gcc
CFLAGS = -ggdb -Wno-div-by-zero -m32

all : exception_cost

exception_cost: exception_cost.c
	$(CC) $(CFLAGS) exception_cost.c -o exception_cost


clean :
	@rm -f exception_cost
