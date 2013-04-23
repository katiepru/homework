#ifndef _Y86EMUL_H
#define Y86_EMUL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Starting functions*/
void run_program(FILE *file);
int *get_size(char line[200]);
void read_lines(FILE *file, int *base);

/*Functions for each line of input file*/
void parse_byte(char line[200], int *base);
void parse_string(char line[200], int *base);
void parse_long(char line[200], int *base);
int **parse_bss(char line[200], int *base);

/*Other shit*/
char *strdup(const char *str);
void print_help();

#endif
