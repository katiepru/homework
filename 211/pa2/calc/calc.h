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
char *hex_to_bin(char *num);
char *dec_to_bin(char *num);
char *oct_to_bin(char *num);
char *bin_to_oct(char *num);
char *bin_to_hex(char *num);
char *bin_to_dec(char *num);

/*Arithmetic functions*/
char *add_sub(char *num1, char *num2, int sub);
char *add(char *num1, char *num2);
char *subtract(char *num1, char *num2);
char *multiply(char *num1, char *num2);
char *to_twos_comp(char *num);
int is_bigger(char *num1, char *num2);

/*Stack functions*/
struct Stack *create_stack(struct Node *n);
struct Node *create_node(char data);
struct Node *pop(struct Stack *s);
struct Node *peek(struct Stack *s);
void push(struct Stack *s, struct Node *node);
void destroy_stack(struct Stack *s);

/*String functions*/
char *strdup(char *str);
