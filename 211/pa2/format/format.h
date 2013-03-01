#ifndef _FORMAT_H
#define _FORMAT_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include <assert.h>

void print_int(char *num);
void int_to_ascii(int num, int is_neg);
void print_float(char *num);
void print_sci(float num);
float power(float base, int exp);
struct Node *create_node(char data);
struct Stack *create_stack(struct Node *n);
struct Node *pop(struct Stack *s);
struct Node *peek(struct Stack *s);
void push(struct Stack *s, struct Node *n);
void destroy_stack(struct Stack *s);

#endif

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
