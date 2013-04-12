#ifndef _Y86EMUL_H
#define Y86_EMUL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Starting functions*/
void run_program(FILE *file);
void parse_line(char *line);

/*Specific instrs*/
void size_instr(char *line, int ind);
void string_instr(char *line, int ind);
void long_instr(char *line, int ind);
void byte_instr(char *line, int ind);
void text_instr(char *line, int ind);
void bss_instr(char *line, int ind);

/*Other shit*/
void print_help();

#endif
