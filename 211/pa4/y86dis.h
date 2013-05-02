#ifndef _Y86DIS_H
#define Y86_DIS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

void disassemble(FILE *file, int* addr_list);
const char *get_reg(char c);
void print_help();

void find_functions(FILE *file, int* addr_list);
