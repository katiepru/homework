typedef struct Node
{
	char *word;
	int count;
	int num_vars;
	struct Node *variations;
	struct Node *next;
} Node;
