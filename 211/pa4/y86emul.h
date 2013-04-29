#ifndef _Y86EMUL_H
#define Y86_EMUL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

/*Starting functions*/
void run_program(FILE *file);
void *get_size(char line[100]);
long read_lines(FILE *file, void *base);
void pipeline(void *base, char *instrs);

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

typedef struct Node
{
	long addr;
	struct Node *next;
} Node;

#endif
