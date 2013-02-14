#include "wordstat.h"

int main(int argc, char *argv[])
{
	/*Initialize everything*/
	const char help[] = "-h";
	FILE *file;
	struct TrieNode *tree;

	/*Check that there is the correct number of args*/
	if(argc!=2)
	{
			fprintf(stderr, "Wrong number of arguments");
			print_help();
			return 1;
	}

	/*Check if in help mode*/
	if(strcmp(argv[1], help)==0)
	{
		print_help();
		return 0;
	}

	/*Open the file*/
	file=fopen(argv[1], "r");

	/*If fopen returns 0, file not found*/
	if(file==0)
	{
		fprintf(stderr, "Could not find file: %s. Exiting.\n", argv[1]);
		return 1;
	}

	/*File was successfully found, begin parsing file*/
	tree=read_file(file);

	/*Close the file*/
	fclose(file);

	/*Print out results*/
	puts("Word	Total No. Occurences	No. Case-Sensitive Versions");
	print_results(tree);

	/*Free all the things!*/
	destroy_trienode(tree);

	return 0;
}


/*-----------------------------------------------------------------------------/
* Read file and create linked list of unique words-----------------------------/
------------------------------------------------------------------------------*/
struct TrieNode *read_file(FILE *file)
{
	/*Initialize evrything - words likely less than 400 chars long*/
	struct TrieNode *tree = create_trienode(NULL, ' ', NULL);
	struct TrieNode *ptr = tree;
	char real_word[400];
	char word[400];
	int c;
	int buff;
	int len;
	int converted;
	int i;
	
	/*Make strings ready to use*/
	memset(word, 0, strlen(word));
	memset(real_word, 0, strlen(real_word));

	c = fgetc(file);
	buff = fgetc(file);

	while(c != EOF)
	{
		if(isalpha(c) || (isdigit(c) && strlen(real_word)>0))
		{
			/*First, add case-sensitive version to real_word*/
			len = strlen(real_word);
			real_word[len]=c;
			real_word[len+1]='\0';

			/*Next, traverse prefix tree case-insensitively*/
			c=tolower(c);
			converted = isalpha(c) ? c-97 : c-22;
			if(ptr->children[converted]==NULL)
			{
				ptr->children[converted]=create_trienode(ptr, c, NULL);
			}
			ptr=ptr->children[converted];
		}
		if(strlen(real_word)>0 && ((!isalpha(c) && !isdigit(c)) || buff==EOF))
		{
			/*Copy real_word and make it lower case*/
			for(i=0; i<strlen(real_word); i++)
			{
				word[i]=tolower(real_word[i]);
			}
			word[i+1]='\0';

			/*Create Node and place it in trienode*/
			if(ptr->full_word==NULL)
			{
				ptr->full_word = create_wordnode(word, real_word);
			} 
			/*Word was already found at least once*/
			else
			{
				/*Increment word count*/
				ptr->full_word->count+=1;
				check_variations(ptr->full_word, real_word);
			}

			/*Reset ptr to root node*/
			ptr=tree;

			/*Reset strings*/
			memset(real_word, 0, strlen(real_word));
			memset(word, 0, strlen(word));
		}
		c = buff;
		buff = fgetc(file);
	}
	return tree;
}


void check_variations(struct WordNode *node, char *real_word)
{
	struct NumNode *numptr;
	int i;
	numptr=node->variations;
	for(i=0; i<strlen(real_word)-1; i++)
	{
		if(isupper(real_word[i]))
		{
			if(numptr->right==NULL)
			{
				numptr->right=create_numnode(1);
			}
			numptr=numptr->right;
		}
		else
		{
			if(numptr->left==NULL)
			{
				numptr->left=create_numnode(0);
			}
			numptr=numptr->left;
		}
	}
				
	/*On last char in string..see if variation exists*/
	if(isupper(real_word[i]) && numptr->right==NULL)
	{
		numptr->right=create_numnode(1);
		node->num_vars+=1;
	}
	else if(!isupper(real_word[i]) && numptr->left==NULL)
	{
		numptr->left=create_numnode(0);
		node->num_vars+=1;
	}
}

/*-----------------------------------------------------------------------------/
/- Prints out results in tree use preorder traversal---------------------------/
/-----------------------------------------------------------------------------*/
void print_results(struct TrieNode *tree)
{
	int i=0;
	if(tree->full_word!=NULL)
	{
		print_wordnode(tree->full_word);
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
void print_wordnode(struct WordNode *node)
{
	printf("%s	%d	%d\n", node->word, node->count, 
		node->num_vars);
}

/*-----------------------------------------------------------------------------/
/- Creates a new Node from a word and next Node--------------------------------/
/-----------------------------------------------------------------------------*/
struct WordNode *create_wordnode(char *str, char *var)
{
	int i;
	struct NumNode *ptr;
	struct WordNode *node = malloc(sizeof(struct WordNode));
	node->word=strdup(str);
	node->count=1;
	node->num_vars=1;
	node->variations=create_numnode(-1);
	ptr=node->variations;
	for(i=0; i<strlen(var); i++)
	{
		/*If letter is upcase, go right in tree*/
		if(isupper(var[i]))
		{
			ptr->right=create_numnode(1);
			ptr=ptr->right;
		}
		/*Otherwise, go left*/
		else
		{
			ptr->left=create_numnode(0);
			ptr=ptr->left;
		}
	}
	return node;
}


/*-----------------------------------------------------------------------------/
/- Creates a new num node -----------------------------------------------------/
/-----------------------------------------------------------------------------*/
struct NumNode *create_numnode(short num)
{
	struct NumNode *node = malloc(sizeof(struct NumNode));
	node->num=num;
	node->left=NULL;
	node->right=NULL;
	return node;
}

/*-----------------------------------------------------------------------------/
/- Creates a new trie node ------------- --------------------------------------/
/-----------------------------------------------------------------------------*/
struct TrieNode *create_trienode(struct TrieNode *parent, char c, 
	struct WordNode *word)
{
	struct TrieNode *tree = malloc(sizeof(struct TrieNode));
	assert(tree!=NULL);
	tree->parent=parent;
	tree->letter=c;
	tree->full_word=word;
	return tree;
}


/*-----------------------------------------------------------------------------/
/- Destroys a num node --------------------------------------------------------/
/-----------------------------------------------------------------------------*/
void destroy_numnode(struct NumNode *node)
{
	assert(node!=NULL);
	/*Free the children*/
	if(node->left!=NULL)
	{
		destroy_numnode(node->left);
	}
	if(node->right!=NULL)
	{
		destroy_numnode(node->right);
	}
	free(node);
}


/*-----------------------------------------------------------------------------/
/- Destroys (frees) a Word Node -----------------------------------------------/
/-----------------------------------------------------------------------------*/
void destroy_wordnode(struct WordNode *node)
{
	assert(node!=NULL);
	free(node->word);
	destroy_numnode(node->variations);
	free(node);
}


/*-----------------------------------------------------------------------------/
/- Destroys (frees) a Trie Node-- ---------------------------------------------/
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
	if(tree->full_word!=NULL)
	{
		destroy_wordnode(tree->full_word);
	}
	free(tree);
}


/*-----------------------------------------------------------------------------/
/- Print help text ------------------------------------------------------------/
/-----------------------------------------------------------------------------*/
void print_help()
{
	puts("wordstat - a word counting program.\n\n  \
Usage: wordstat [-h] file\n  \
help (-h)		Display this message\n  \
file			desc here\n");
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
