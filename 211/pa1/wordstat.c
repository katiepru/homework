#include "wordstat.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

void print_help();
struct LinkedList *read_file();
struct Node *create_node();
struct LinkedList *create_linkedlist();
void destroy_node();
void destroy_linkedlist();

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
	struct LinkedList *ll;
	ll=read_file(file);

	return 0;
}


/*-----------------------------------------------------------------------------/
* Read file and create linked list of unique words-----------------------------/
------------------------------------------------------------------------------*/
struct LinkedList *read_file(FILE *file)
{
	struct LinkedList *ll = create_linkedlist(NULL);
	char* word;
	int c;
	while((c = fgetc(file)) != EOF)
	{
		if(isalpha(c) || (isdigit(c) && strlen(word)>0))
		{
			int len = strlen(word);
			word[len]=c;
			word[len+1]='\0';
		}
		else if(strlen(word)>0)
		{

			Node *node = create_node(word, NULL);
			if(ll->head==NULL)
			{
				ll->head=node;
			}
			else
			{
				Node *ptr = ll->head;
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
struct Node *create_node(char *str, Node *next)
{
	struct Node *node = malloc(sizeof(struct Node));
	node->word=strdup(str);
	node->next=next;
	node->count=1;
	node->num_vars=1;
	node->variations=NULL;
	return node;
}


/*-----------------------------------------------------------------------------/
/- Creates a new Linked List from a Node --------------------------------------/
/-----------------------------------------------------------------------------*/
struct LinkedList *create_linkedlist(struct Node *node)
{
	struct LinkedList *ll = malloc(sizeof(struct LinkedList));
	assert(ll!=NULL);
	ll->head=node;
	ll->length = node!=NULL ? 1 : 0;
	return ll;
}


/*-----------------------------------------------------------------------------/
/- Destroys (frees) a Node ----------------------------------------------------/
/-----------------------------------------------------------------------------*/
void destroy_node(struct Node *node)
{
	assert(node!=NULL);
	free(node->word);
	destroy_linkedlist(node->variations);
	free(node);
}


/*-----------------------------------------------------------------------------/
/- Destroys (frees) a Linked List ---------------------------------------------/
/-----------------------------------------------------------------------------*/
void destroy_linkedlist(struct LinkedList *ll)
{
	assert(ll!=NULL);
	free(ll);
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
