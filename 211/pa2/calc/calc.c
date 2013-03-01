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
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
		return 1;
	}

	/*Iterate through nums to convert to binary*/	
	for(k = 0; k<2; k++)
	{
		j = 0;
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
			fprintf(stderr, "ERROR: Unrecognized format\n");
			return 3;
		}
	}


	printf("num1= %s; num2=%s\n", nums[0], nums[1]);

	if(argv[1][0] == '+')
	{
		add_sub(nums[0], nums[1], 0);
	}
	else if(argv[1][0] == '-')
	{
		add_sub(nums[0], nums[1], 1);
	}
	else if(argv[1][0] == '*')
	{
		/*multiply(nums[0], nums[1]);*/
		puts("maybe later...\n");
	}

	return 0;
}

/*Start arithmetic functions*/

char *add_sub(char *num1, char *num2, int sub)
{
	char *result;
	/*Add two positive numbers*/
	if(num1[0] != '-' && ((num2[0] != '-' && !sub) || 
		(num2[0] == '-' && sub)))
	{
		result = add(num1, num2);
	}
	/*Add two negative numbers*/
	else if(num1[0] == '-' && ((num2[0] == '-' && !sub) ||
		(num2[0] != '-' && sub)))
	{
		result = add(num1, num2);
		/*make neg*/
	}
	else if(num1[0] == '-')
	{
		result = subtract(num2, num1);
	}
	else
	{
		result = subtract(num1, num2);
	}

	return result;
}

char *add(char *num1, char *num2)
{
	char *res = calloc(MAX(strlen(num1), strlen(num2))+1,
		sizeof(char));
	int ind = 0;
	int curr_carry = 0;
	int prev_carry = 0;
	char *max;
	char *min;
	int max_ind;
	int min_ind;
	int res_ind = MAX(strlen(num1), strlen(num2))+1;

	if(num1[0] == '-')
	{
		ind = 1;
		res[0] = '-';
	}

	/*Find longer number*/
	if(strlen(num1) >= strlen(num2))
	{
		max = num1;
		min = num2;
	}
	else
	{
		max = num2;
		min = num1;
	}

	max_ind = strlen(max) - 1;
	min_ind = strlen(min) -1;

	while(min_ind >= ind)
	{
		if(max[max_ind] == '0' && min[min_ind] == '0')
		{
			res[res_ind] = '0';
		}
		else if(max[max_ind] == '1' && min[min_ind] == '1')
		{
			res[res_ind] = '0';
			curr_carry = 1;
		}
		else
		{
			res[res_ind] = '1';
		}

		if(prev_carry)
		{
			if(res[res_ind] == '0')
			{
				res[res_ind] = '1';
			}
			else
			{
				res[res_ind] = '0';
				curr_carry = 1;
			}
		}

		max_ind--;
		min_ind--;
		res_ind--;
		prev_carry = curr_carry;
		curr_carry = 0;
	}

	while(max_ind >= 0)
	{
		res[res_ind] = max[max_ind];
		if(prev_carry == 1)
		{
			if(res[res_ind] == '0')
			{
				res[res_ind] = '1';
				prev_carry = 0;
			}
			else
			{
				res[res_ind] = '0';
			}
		}
		max_ind--;
		res_ind--;
	}

	if(prev_carry == 1)
	{
		res[res_ind] = '1';
		res_ind--;
	}

	while(res_ind >= 0 && res[res_ind] != '-')
	{
		res[res_ind] = '0';
		res_ind--;
	}


	printf("sum is %s\n", res);

	return res;
}

char *subtract(char *num1, char *num2)
{
	return "foo";
}

/*End arithmetic functions*/


/*Start conversion functions*/

char *hex_to_bin(char *num)
{
	return "hex_to_bin";
}

char *dec_to_bin(char *num)
{
	return "dec_to_bin";
}

char *oct_to_bin(char *num)
{
	return "oct_to_bin";
}

char *bin_to_hex(char *num)
{
	return "bin_to_hex";
}

char *bin_to_oct(char *num)
{
	return "bin_to_oct";
}

char *bin_to_dec(char *num)
{
	return "bin_to_dec";
}

/*End conversion functions*/


/*Start Stack Functions*/

struct Node *create_node(char data)
{
	struct Node *n = malloc(sizeof(struct Node));
	n->data = data;
	n->next = NULL;
	return n;
}

struct Stack *create_stack(struct Node *n)
{
	struct Stack *s = malloc(sizeof(struct Stack));
	s->head = n;
	if(n == NULL)
	{
		s->size = 0;
	}
	else
	{
		s->size = 1;
	}
	return s;
}

void destroy_stack(struct Stack *s)
{
	while(peek(s) != NULL)
	{
		free(pop(s));
	}
	free(s);
}

struct Node *pop(struct Stack *s)
{
	struct Node *res;
	if(peek(s) == NULL) return NULL;
	res = s->head;
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
