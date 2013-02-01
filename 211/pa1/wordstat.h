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
