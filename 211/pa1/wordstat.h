typedef struct VarNode
{
	char* word;
	VarNode *next;
} VarNode;

typedef struct Node
{
	char *word;
	int count;
	int num_vars;
	int balance_factor;
	struct VarNode *variations;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
} Node;

typedef struct AVLTree
{
	Node *head;
	int height;
	int size;
} AVLTree;
