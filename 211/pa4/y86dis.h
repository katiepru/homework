#ifndef _Y86DIS_H
#define Y86_DIS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

void disassemble(FILE *file, int* function_list, int* jump_list);
const char *get_reg(char c);
void print_help();

void find_functions_and_jumps(FILE *file, int* function_list, int* jump_list);

int check_for_jump(int addr, int* jump_list);

int to_big_endian_int(char* little_endy_str);

/*Arrays of instructions*/
const char math_ops[4][5] = {
	"addl\0",
	"subl\0",
	"xorl\0",
	"mull\0"
};

const char jump_ops[7][4] = {
	"jmp\0",
	"jle\0",
	"jl \0",
	"je \0",
	"jne\0",
	"jge\0",
	"jg \0"
};

const char read_ops[2][6] = {
	"readb",
	"readw"
};

const char write_ops[2][7] = {
	"writeb",
	"writew"
};
