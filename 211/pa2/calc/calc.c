#include "calc.h"

int main(int argc, char *argv[])
{
	char *nums[2];	
	char c;
	int i;
	int j = 0;
	int k;

	if(argc != 5)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.");
		return 1;
	}

	/*Iterate through nums to convert to binary*/	
	for(k = 0; k<2; k++)
	{
		if(argv[k+2][0] == '-')
		{
			c = argv[k+2][1];
		}
		else
		{
			c = argv[k+2][0];
		}
	
		if(c == 'b')
		{
			nums[k] = calloc(strlen(argv[k+2])-1, sizeof(char));
			for(i = 0; i<strlen(argv[k+2]); i++)
			{
				if(argv[k+2][i] == 'b')
				{
					continue;
				}
				nums[k][j] = argv[k+2][i];
				j++;
			}
		}
		else if(c == 'o')
		{
			nums[k] = oct_to_bin(argv[k+2]);
		}
		else if(c == 'x')
		{
			nums[k] = hex_to_bin(argv[k+2]);
		}
		else if(c == 'd')
		{
			nums[k] = dec_to_bin(argv[k+2]);
		}
		else
		{
			fprintf(stderr, "ERROR: Unrecognized format");
			return 3;
		}
	}


	printf("num1= %s; num2=%s\n", nums[0], nums[1]);

	return 0;
}

/*Start arithmetic functions*/

char *add_sub(char *num1, char *num2, int sub)
{
	if(num1[0] != '-' && ((num2[0] != '-' && !sub) || 
		num2[0] == '-' && sub))
	{
		add(num1, num2);
	}
	else if(num1[0] == '-' && ((num2[0] == '-' && !sub) ||
		num2[0] != '-' && sub))
	{
		add(num1, num2);
		/*make neg*/
	}
	else if(num1[0] == '-')
	{
		subtract(num2, num1);
	}
	else
	{
		subtract(num1, num2);
	}
}

char *add(char *num1, char *num2)
{
	char *res = calloc(max(strlen(num1), strlen(num2))+1,
		sizeof(char));
	int i = 0;
	int j = 0;
	int k = 0;

	if(max(strlen(num1), strlen(num2)) == strlen(num1))
	{

	}
}



/*Start Stack Functions*/

struct Node *pop(struct Stack *s)
{
	if(peek(s) == NULL) return NULL;
	struct Node *res = s->head;
	s->head = s->head->next;
	return res;
}

struct Node *peek(struct Stack *s)
{
	return s->head;
}

void push(struct Stack *s, struct Node *node)
{
	node->next = s->head;
	s->head = node;
	return;
}

/*End Stack Functions*/
