#ifndef _WORDSTAT_H
#define _WORDSTAT_H	1
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

char *strdup(const char *str);
struct TrieNode *read_file(FILE *file);
struct Node *create_node(char *str, struct Node *next, char *var);
struct TrieNode *create_trienode(struct TrieNode *parent, char c, struct Node
*word);
void destroy_node(struct Node *node);
void destroy_trienode(struct TrieNode *tree);
void print_results(struct TrieNode *tree);
void print_node(struct Node *node);
void print_help();

typedef struct Node
{
	char *word;
	int count;
	int num_vars;
	struct Node *variations;
	struct Node *next;
} Node;

typedef struct TrieNode
{
	char letter;
	struct TrieNode *parent;
	struct Node *full_word;
	struct TrieNode *children[36];
} TrieNode;
