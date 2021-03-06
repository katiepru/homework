#ifndef _Y86EMUL_H
#define Y86_EMUL_H
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tgmath.h>
#include <limits.h>

enum
{
	OF, ZF, SF
} ConCode;

enum
{
	AOK, HLT, ADR, INS
} StatCode;

typedef struct Node
{
	long addr;
	long data;
	int byte;
	struct Node *next;
} Node;

/*Starting functions*/
void run_program(FILE *file);
void *get_size(unsigned char line[100]);
long read_lines(FILE *file, void *base);
void pipeline(void *base, unsigned char *instrs);

/*Fetch, decode, exec, and wb*/
int fetch(unsigned char curr[7], unsigned char *instrs, int *pc);
void decode(unsigned char curr[7]);
int execute(unsigned char curr[7], int registers[8], struct Node *mem_vals, 
	int flags[3], int *pc, unsigned char *base, unsigned char *instrs);
void writeback(int registers[8], long *base, struct Node *memvals);

/*Helpers*/
void set_flags(long val, int flags[4]);
int check_addr(int addr);

/*Put and get functions*/
void put_byte(unsigned char *addr, unsigned char byte);
unsigned char get_byte(unsigned char *addr);
void put_long(long *addr, long num);
long get_long(long *addr);
void put_string(unsigned char *addr, unsigned char *str);
unsigned char *get_string(unsigned char *addr, int len);

/*Linked list functions*/
struct Node *create_node(long addr, long data, int byte);
void delete_node(long addr, struct Node *head);

/*Other shit*/
void print_help();

