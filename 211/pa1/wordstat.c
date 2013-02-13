#include "wordstat.h"

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

	//Close the file
	fclose(file);

	//Print out results
	print_results(tree);

	return 0;
}


/*-----------------------------------------------------------------------------/
* Read file and create linked list of unique words-----------------------------/
------------------------------------------------------------------------------*/
struct TrieNode *read_file(FILE *file)
{
	struct TrieNode *tree = create_trienode(NULL, ' ', NULL);
	struct TrieNode *ptr = tree;
	char real_word[400];
	int c;
	memset(real_word, 0, strlen(real_word));

	while((c = fgetc(file)) != EOF)
	{
		if(isalpha(c) || (isdigit(c) && strlen(real_word)>0))
		{
			//First, add case-sensitive version to real_word
			int len = strlen(real_word);
			real_word[len]=c;
			real_word[len+1]='\0';

			//Next, traverse prefix tree case-insensitively
			int converted = isalpha(c) ? c-97 : c-12;
			if(ptr->children[converted]==NULL)
			{
				ptr->children[converted]=create_trienode(ptr, c, NULL);
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

			//Create Node and place it in trienode
			ptr->full_word = create_node(word, NULL, real_word);

			//Free real word and word
			free(real_word);
			free(word);

			//Reset ptr to root node
			ptr=tree;
		}
	}
	return tree;
}

/*-----------------------------------------------------------------------------/
/- Prints out results in tree use preorder traversal---------------------------/
/-----------------------------------------------------------------------------*/
void print_results(struct TrieNode *tree)
{
	int i=0;
	if(tree->full_word!=NULL)
	{
		print_node(tree->full_word);
	}
	for(i=0; i<36; i++)
	{
		if(tree->children[i]!=NULL)
		{
			print_results(tree->children[i]);
		}
	}
}

/*-----------------------------------------------------------------------------/
/- Prints out data stored in a node -------------------------------------------/
/-----------------------------------------------------------------------------*/
void print_node(struct Node *node)
{
	printf("word=%s\n", node->word);
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
struct TrieNode *create_trienode(struct TrieNode *parent, char c, struct Node 
*word)
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
	int i;
	assert(tree!=NULL);
	for(i=0; i<36; i++)
	{
		if(tree->children[i]!=NULL)
		{
			destroy_trienode(tree->children[i]);
		}
	}
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
	help (-h)		Display this message\n \
	file			desc here");
}

/*-----------------------------------------------------------------------------/
/--Duplicate a string and return a pointer to the duplicate--------------------/
/-----------------------------------------------------------------------------*/
char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}
