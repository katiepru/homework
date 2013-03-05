#ifndef _CALC_H
#define _CALC_H 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

typedef struct Node
{
	char data;
	struct Node *next;
} Node;

typedef struct Stack
{
	int size;
	struct Node *head;
} Stack;

/*Conversion Functions*/
int bin_to_dec(char *num);
int oct_to_doc(char *num);
int hex_to_dec(char *num);
int str_to_dec(char *num);
char *dec_to_bin(int num);
char *dec_to_oct(int num);
char *dec_to_hex(int num);
char *dec_to_str(int num);

/*Arithmetic functions*/
int add(int num1, int num2);
int subtract(int num2, int num2);
int multiply(int num1, int num2);

/*Stack functions*/
struct Stack *create_stack(struct Node *n);
struct Node *create_node(char data);
struct Node *pop(struct Stack *s);
struct Node *peek(struct Stack *s);
void push(struct Stack *s, struct Node *node);
void destroy_stack(struct Stack *s);

/*String functions*/
char *strdup(char *str);
