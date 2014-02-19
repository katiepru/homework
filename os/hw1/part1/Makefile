CC = gcc
CFLAGS = -ggdb -Wall

all : cache_size line_size

cache_size: cache_size.c
	$(CC) $(CFLAGS) cache_size.c -o cache_size

line_size: line_size.c
	$(CC) $(CFLAGS) line_size.c -o line_size

clean :
	@rm -f cache_size line_size
