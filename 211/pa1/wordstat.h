typedef struct VarNode
{
	char* word;
	struct VarNode *next;
} VarNode;

typedef struct Node
{
	char *word;
	int count;
	int num_vars;
	int balance_factor;
	struct VarNode *variations;
	struct Node *next;
} Node;

typedef struct LinkedList
{
	int length;
	struct Node *head;
	struct Node *tail;
} LinkedList;
