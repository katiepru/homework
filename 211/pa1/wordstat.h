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
struct NumNode *create_numnode(short num);
struct WordNode *create_wordnode(char *str, char *var);
struct TrieNode *create_trienode(struct TrieNode *parent, char c, 
	struct WordNode *word);
void check_variations(struct WordNode *node, char *real_word);
void destroy_numnode(struct NumNode *node);
void destroy_wordnode(struct WordNode *node);
void destroy_trienode(struct TrieNode *tree);
void print_results(struct TrieNode *tree);
void print_wordnode(struct WordNode *node);
void print_help();

typedef struct WordNode
{
	char *word;
	int count;
	int num_vars;
	struct NumNode *variations;
} WordNode;

typedef struct TrieNode
{
	char letter;
	struct TrieNode *parent;
	struct WordNode *full_word;
	struct TrieNode *children[36];
} TrieNode;

typedef struct NumNode
{
	short num;
	struct NumNode *left;
	struct NumNode *right;
} NumNode;
