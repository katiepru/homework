#ifndef _Y86EMUL_H
#define Y86_EMUL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

enum
{
	eax, ecx, edx, ebx, esp, ebp, esi, edi
} Reg;

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
	struct Node *next;
} Node;

/*Starting functions*/
void run_program(FILE *file);
void *get_size(char line[100]);
long read_lines(FILE *file, void *base);
void pipeline(void *base, char *instrs);

/*Fetch, decode, exec, and wb*/
void fetch(char curr[7], char *instrs, int *pc);
void decode(char curr[7]);
void execute(char curr[7], int registers[8], struct Node *mem_vals, 
	int reg_vals[8]);
void writeback(struct Node *mem_vals, int reg_vals[8], int registers[8]);

/*Put and get functions*/
void put_byte(char *addr, char byte);
char get_byte(char *addr);
void put_long(long *addr, long num);
long get_long(long *addr);
void put_string(char *addr, char *str);
char *get_string(char *addr, int len);

/*Other shit*/
char *strdup(const char *str);
void print_help();

