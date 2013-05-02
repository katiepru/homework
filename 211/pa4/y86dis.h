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
