#ifndef _Y86EMUL_H
#define Y86_EMUL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Starting functions*/
void run_program(FILE *file);
int *get_size(char line[200]);

/*Other shit*/
void print_help();

#endif
