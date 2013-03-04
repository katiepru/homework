#include "calc.h"

int main(int argc, char *argv[])
{
	char *nums[2];
	char *result;
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
		result = add_sub(nums[0], nums[1], 0);
	}
	else if(argv[1][0] == '-')
	{
		result = add_sub(nums[0], nums[1], 1);
	}
	else if(argv[1][0] == '*')
	{
		/*multiply(nums[0], nums[1]);*/
		puts("maybe later...\n");
	}

	c = argv[5][0];
	switch (c)
	{
		case 'b':
			break;
		case 'o':
			result = bin_to_oct(result);
			break;
		case 'x':
			result = bin_to_hex(result);
			break;
		case 'd':
			result = bin_to_dec(result);
			break;
		default:
			fprintf(stderr, "Invalid output base.\n");
			return 3;
	}

	printf("%s\n", result);

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

char *multiply(char *num1, char *num2)
{
	char *result = calloc(strlen(num1)+strlen(num2), sizeof(char));
	result = strdup(num1);
	subtract(num2, "1");
	while(num2 != "0")
	{
		result = add(result, num1);
		num2 = subtract(num2, "1");
	}
	return result;
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
	char *result = calloc((strlen(num)/4)+1, sizeof(char));
	char *quad = calloc(4, sizeof(char));
	int bin_ind = strlen(num) - 1;
	int res_ind = strlen(num)/4;

	while(bin_ind >= 4)
	{
		quad[0] = num[bin_ind-3];
		quad[1] = num[bin_ind-2];
		quad[2] = num[bin_ind-1];
		quad[3] = num[bin_ind];

		if(strcmp(quad, "0000") == 0)
		{
			result[res_ind] = '0';
		}
		else if(strcmp(quad, "0001") == 0)
		{
			result[res_ind] = '1';
		}
		else if(strcmp(quad, "0010") == 0)
		{
			result[res_ind] = '2';
		}
		else if(strcmp(quad, "0011") == 0)
		{
			result[res_ind] = '3';
		}
		else if(strcmp(quad, "0100") == 0)
		{
			result[res_ind] = '4';
		}
		else if(strcmp(quad, "0101") == 0)
		{
			result[res_ind] = '5';
		}
		else if(strcmp(quad, "0110") == 0)
		{
			result[res_ind] = '6';
		}
		else if(strcmp(quad, "0111") == 0)
		{
			result[res_ind] = '7';
		}
		else if(strcmp(quad, "1000") == 0)
		{
			result[res_ind] = '8';
		}
		else if(strcmp(quad, "1001") == 0)
		{
			result[res_ind] = '9';
		}
		else if(strcmp(quad, "1010") == 0)
		{
			result[res_ind] = 'a';
		}
		else if(strcmp(quad, "1011") == 0)
		{
			result[res_ind] = 'b';
		}
		else if(strcmp(quad, "1100") == 0)
		{
			result[res_ind] = 'c';
		}
		else if(strcmp(quad, "1101") == 0)
		{
			result[res_ind] = 'd';
		}
		else if(strcmp(quad, "1110") == 0)
		{
			result[res_ind] = 'e';
		}
		else if(strcmp(quad, "1111") == 0)
		{
			result[res_ind] = 'f';
		}
		else
		{
			fprintf(stderr, "Unrecognized quad %s\n.", quad);
		}
	}

	return "bin_to_hex";
}

char *bin_to_oct(char *num)
{
	char *result = calloc((strlen(num)/3)+1, sizeof(char));
	char *triplet = calloc(3, sizeof(char));
	int bin_ind = strlen(num)-1;
	int res_ind = strlen(num)/3;

	while(bin_ind >= 3)
	{
		triplet[0] = num[bin_ind-2];
		triplet[1] = num[bin_ind-1];
		triplet[2] = num[bin_ind];

		if(strcmp(triplet, "000") == 0)
		{
			result[res_ind] = '0';
		}
		else if(strcmp(triplet, "001") == 0)
		{
			result[res_ind] = '1';
		}
		else if(strcmp(triplet, "010") == 0)
		{
			result[res_ind] = '2';
		}
		else if(strcmp(triplet, "011") == 0)
		{
			result[res_ind] = '3';
		}
		else if(strcmp(triplet, "100") == 0)
		{
			result[res_ind] = '4';
		}
		else if(strcmp(triplet, "101") == 0)
		{
			result[res_ind] = '5';
		}
		else if(strcmp(triplet, "110") == 0)
		{
			result[res_ind] = '6';
		}
		else
		{
			result[res_ind] = '7';
		}
		res_ind--;
		bin_ind -= 3;
		memset(triplet, 0, sizeof(triplet));
	}

	while(res_ind >= 0)
	{
		result[res_ind] = '0';
		res_ind--;
	}

	return result;
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
