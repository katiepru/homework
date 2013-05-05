#ifndef _Y86ASS_H
#define Y86_ASS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

void pre_assemble(FILE *file, int* function_list, int* jump_list);
void assemble(FILE *file, int* function_list, int* jump_list);
const char *get_reg(char c);
void print_help();

char get_register(char* reg);

void print_as_little_endian(int num);
