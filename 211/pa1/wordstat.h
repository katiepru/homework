typedef struct Node
{
	char *word;
	int count;
	int num_vars;
	struct Node *variations;
	struct Node *next;
} Node;

typedef struct TrieNode
{
	char letter;
	struct TrieNode *children;
	struct TrieNode *parent;
	struct Node *full_word;
} TrieNode;
