#include "wordstat.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

void print_help();
struct TrieNode *read_file();
struct Node *create_node();
struct TrieNode *create_trienode();
void destroy_node();
void destroy_trienode();

int main(int argc, char *argv[])
{
	//Check that there is the correct number of args
	if(argc!=2)
	{
			printf("Wrong number of arguments");
			print_help();
			return 1;
	}

	//Check if in help mode
	const char help[] = "-h";
	if(strcmp(argv[1], help)==0)
	{
		print_help();
		return 0;
	}

	//Open the file
	FILE *file;
	file=fopen(argv[1], "r");

	//If fopen returns 0, file not found
	if(file==0)
	{
		printf("Could not find file: %s. Exiting.\n", argv[1]);
		return 1;
	}

	//File was successfully found, begin parsing file
	struct TrieNode *tree;
	tree=read_file(file);

	return 0;
}


/*-----------------------------------------------------------------------------/
* Read file and create linked list of unique words-----------------------------/
------------------------------------------------------------------------------*/
struct TrieNode *read_file(FILE *file)
{
	struct TrieNode *tree = create_trienode(NULL, ' ', NULL);
	struct TrieNode *ptr = tree;
	char* real_word;
	int c;
	while((c = fgetc(file)) != EOF)
	{
		if(isalpha(c) || (isdigit(c) && strlen(real_word)>0))
		{
			//First, add case-sensitive version to real_word
			int len = strlen(real_word);
			real_word[len]=c;
			real_word[len+1]='\0';

			//Next, traverse prefix tree case-insensitively
			//Note: proabbly not the way to convert c
			int converted = isalpha(c) ? c-97 : c-12;
			if(ptr->children[converted]==NULL)
			{
				ptr->children[convereted]=create_trienode(ptr, c, NULL);
			}
			ptr=ptr->children[converted];
		}
		else if(strlen(real_word)>0)
		{
			//Copy real_word and make it lower case
			char *word;
			int i=0;
			for(i=0; i<strlen(real_word); i++)
			{
				word[i]=tolower(real_word[i]);
			}
			word[i+1]='\0';

			//Create Node and place it in linked list
			Node *node = create_node(word, NULL);
			if(tree->parent==NULL)
			{
				tree->full_word=node;
			}
			else
			{
				Node *ptr = tree->full_word;
				if(strcmp(ptr->word, word)>0)
				{
					ptr->next=node;
				}
			}
		}
	}
	return NULL;
}


/*-----------------------------------------------------------------------------/
/- Creates a new Node from a word and next Node--------------------------------/
/-----------------------------------------------------------------------------*/
struct Node *create_node(char *str, Node *next, char *var)
{
	struct Node *node = malloc(sizeof(struct Node));
	node->word=strdup(str);
	node->next=next;
	node->count=1;
	node->num_vars=1;
	node->variations=create_node(var, NULL, NULL);
	return node;
}


/*-----------------------------------------------------------------------------/
/- Creates a new Linked List from a Node --------------------------------------/
/-----------------------------------------------------------------------------*/
struct TrieNode *create_trienode(struct TrieNode *parent, char c, struct Node *word)
{
	struct TrieNode *tree = malloc(sizeof(struct TrieNode));
	assert(tree!=NULL);
	tree->parent=parent;
	tree->letter=c;
	tree->full_word=word;
	return tree;
}


/*-----------------------------------------------------------------------------/
/- Destroys (frees) a Node ----------------------------------------------------/
/-----------------------------------------------------------------------------*/
void destroy_node(struct Node *node)
{
	assert(node!=NULL);
	free(node->word);
	free(node);
}


/*-----------------------------------------------------------------------------/
/- Destroys (frees) a Linked List ---------------------------------------------/
/-----------------------------------------------------------------------------*/
void destroy_trienode(struct TrieNode *tree)
{
	assert(tree!=NULL);
	free(tree->children);
	free(tree);
}


/*-----------------------------------------------------------------------------/
/- Print help text ------------------------------------------------------------/
/-----------------------------------------------------------------------------*/
void print_help()
{
	printf("wordstat - a word counting program.\n \
	Usage: wordstat [-h] file\n \
		help (-h)		Display this message");
}
