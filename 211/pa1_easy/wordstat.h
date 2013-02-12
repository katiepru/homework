#ifndef _WORDSTAT_H
#define _WORDSTAT_H	1
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

void print_help();
struct LinkedList *read_file(FILE *file);
struct Node *create_node(char *str, struct Node *next, char *var);
struct LinkedList *create_linkedlist(struct Node *node);
void destroy_node(struct Node *node);
void destroy_linkedlist(struct LinkedList *ll);
struct LinkedList *insert_node(struct LinkedList *ll, struct Node *node, 
	char *word);
void print_ll(struct LinkedList *ll);
char *strdup(const char *str);

typedef struct Node
{
	char *word;
	int count;
	int num_vars;
	struct LinkedList *variations;
	struct Node *next;
} Node;

typedef struct LinkedList
{
	int length;
	struct Node *head;
} LinkedList;
