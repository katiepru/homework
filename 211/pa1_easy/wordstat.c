#include "wordstat.h"

int main(int argc, char *argv[])
{
	const char help[3]="-h";
	FILE *file;
	struct LinkedList *ll;
	/*Check that there is the correct number of args*/
	if(argc!=2)
	{
			printf("Wrong number of arguments");
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
		printf("Could not find file: %s. Exiting.\n", argv[1]);
		return 1;
	}

	/*File was successfully found, begin parsing file*/
	ll=read_file(file);
	fclose(file);

	/*Linked list created, now print results*/
	print_ll(ll);

	/*Free linked list*/
	destroy_linkedlist(ll);

	return 0;
}


/*-----------------------------------------------------------------------------/
/- Read file and create linked list of unique words----------------------------/
/-----------------------------------------------------------------------------*/
struct LinkedList *read_file(FILE *file)
{
	struct LinkedList *ll = create_linkedlist(NULL);
	/*Longest word in dictionary is 45 chars, so 100 should be fine*/
	char real_word[100];
	char word[100];
	Node *node;
	int c;
	while((c = fgetc(file)) != EOF)
	{
		if(isalpha(c) || (isdigit(c) && strlen(real_word)>0))
		{
			int len = strlen(real_word);
			real_word[len]=c;
			real_word[len+1]='\0';
		}
		else if(strlen(real_word)>0)
		{
			/*Copy real_word and make it lower case*/
			int i=0;
			for(i=0; i<=strlen(real_word); i++)
			{
				word[i]=tolower(real_word[i]);
			}

			/*Create Node and place it in linked list*/
			node = create_node(word, NULL, real_word);
			if(ll->head==NULL)
			{
				ll->head=node;
			}
			else
			{
				ll = insert_node(ll, node, real_word);
			}
			memset(real_word, 0, strlen(real_word));
		}
	}
	return ll;
}

/*-----------------------------------------------------------------------------/
/- Inserts node into linked list in alphabetical order, or adds case-sensitive-/
/- variation of the word into existing Node if word was already found before --/
/-----------------------------------------------------------------------------*/
struct LinkedList *insert_node(struct LinkedList *ll, struct Node *node,
	char *word)
{
	/*Head is not null*/
	Node *ptr = ll->head;
	int done = 0;

	/*Do checks on the head first*/
	if(strcmp(ptr->word, node->word)>0)
	{
		ll->head=node;
		node->next=ptr;
		done=1;
	}
	else if(strcmp(ptr->word, node->word)==0)
	{
		if(word!=NULL)
		{
			insert_node(ptr->variations, create_node(word, NULL, 
				NULL), NULL);
			done=1;
		}
	}

	/*Done with head, can enter loop*/
	while(!done)
	{
		if(ptr->next==NULL)
		{
			ptr->next=node;
		}
		else if(strcmp(ptr->next->word, node->word)<0)
		{
			/*Word in node still bigger, increment ptr*/
			ptr=ptr->next;
		}
		else if(strcmp(ptr->next->word, node->word)==0)
		{
			/*They are equal. If this is main linked list insert variation.
			Otherwise, we are done.*/
			if(word!=NULL)
			{
				insert_node(ptr->next->variations, create_node(word, NULL,
					NULL), NULL);
			}
			done=1;
		}
		else
		{
			/*Node goes here*/
			node->next=ptr->next;
			ptr->next=node;
			done=1;
		}
	}
	return ll;
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
	if(var!=NULL)
	{
		node->variations=create_linkedlist(create_node(var, NULL, NULL));
	}
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
	if(node==NULL) return;
	free(node->word);
	destroy_linkedlist(node->variations);
	free(node);
}


/*-----------------------------------------------------------------------------/
/- Destroys (frees) a Linked List ---------------------------------------------/
/-----------------------------------------------------------------------------*/
void destroy_linkedlist(struct LinkedList *ll)
{
	struct Node *ptr;
	struct Node *temp;
	if(ll==NULL) return;
	ptr=ll->head;
	while(ptr!=NULL)
	{
		temp = ptr->next;
		destroy_node(ptr);
		ptr=temp;
	}
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

/*-----------------------------------------------------------------------------/
/- Prints data from linked list in order --------------------------------------/
/-----------------------------------------------------------------------------*/
void print_ll(LinkedList *ll)
{
	struct Node *ptr;
	assert(ll!=NULL);
	ptr = ll->head;

	while(ptr!=NULL)
	{
		printf("word= %s \n", ptr->word);
		ptr=ptr->next;
	}
	puts("Done");
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
