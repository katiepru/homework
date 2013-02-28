#ifndef _FORMAT_H
#define _FORMAT_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

void print_int(char *num);
void int_to_ascii(int num, int is_neg);
void print_float(char *num);
void print_sci(float num);
float power(float base, int exp);

#endif
