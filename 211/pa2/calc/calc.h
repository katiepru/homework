#ifndef _CALC_H
#define _CALC_H 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	char data;
	struct Node *next;
} Node;

typedef struct Stack
{
	struct Node *head;
} Stack;

/*Conversion Functions*/
char *hex_to_bin(char *num);
char *dec_to_bin(char *num);
char *oct_to_bin(char *num);

/*Arithmetic functions*/
char *add_sub(char *num1, char *num2, int sub);
char *add(char *num1, char *num2);
char *subtract(char *num1, char *num2);
char *mult(char *num1, char *num2);

/*Stack functions*/
struct Node *pop(struct Stack *s);
struct Node *peek(struct Stack *s);
void push(struct Stack *s, struct Node *node);



