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
		multiply(nums[0], nums[1]);
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
	/*Find out which number is bigger*/
	char *result;
	int bigger;

	bigger = is_bigger(num1, num2);
	if(bigger == 1)
	{
		return subtract(num2, num1);
	}

	/*Convert second term to twos complement and add*/
	num2 = to_twos_comp(num2);
	result = add(num1, num2);

	/*Remove MSB*/
	result[0] = '0';

	printf("Sub res = %s\n", result);
	return result;
}

char *multiply(char *num1, char *num2)
{
	char *result = calloc(strlen(num1)+strlen(num2)+1, sizeof(char));
	char *conv_num1;
	char *conv_num2;
	int is_neg = 0;
	int i;

	if((num1[0] == '-' && num2[0] != '-') || (num1[0] != '-' && num2[0] == '-'))
	{
		is_neg = 1;
	}

	if(is_neg)
	{
		result[0] = '-';
	}

	if(num1[0] == '-')
	{
		conv_num1 = calloc(strlen(num1)-1, sizeof(char));
		for(i = 1; i < (strlen(num1)); i++)
		{
			conv_num1[i-1] = num1[i];
		}
	}
	else
	{
		conv_num1 = num1;
	}

	if(num2[0] == '-')
	{
		conv_num2 = calloc(strlen(num2)-1, sizeof(char));
		for(i = 1; i < (strlen(num2)); i++)
		{
			conv_num2[i-1] = num2[i];
		}
	}
	else
	{
		conv_num2 = num2;
	}

	result = strdup(num1);
	subtract(num2, "1");
	while(num2 != "0")
	{
		result = add(result, num1);
		num2 = subtract(num2, "1");
	}

	free(conv_num1);
	free(conv_num2);

	return result;
}

char *to_twos_comp(char *num)
{
	char *result = calloc(strlen(num)+1, sizeof(char));
	int one_ind;
	int i;

	/*Fin index of first 1*/
	for(one_ind = strlen(num) - 1; one_ind >= 0; one_ind --)
	{
		if(num[one_ind] == '1')
		{
			break;
		}
	}

	/*Flip bits*/
	for(i = 0; i < one_ind; i++)
	{
		if(num[i] == '0')
		{
			result[i] = '1';
		}
		else
		{
			result[i] = '0';
		}
	}

	/*Copy bits after first one*/
	for(i; i <= strlen(num); i++)
	{
		result[i] = num[i];
	}

	return result;

}

int is_bigger(char *num1, char *num2)
{
	int i;

	if(strlen(num1) > strlen(num2))
	{
		return 0;
	}
	else if(strlen(num2) > strlen(num1))
	{
		return 1;
	}
	else
	{
		for(i = 0; i < strlen(num1); i++)
		{
			if(num1[i] != num2[i])
			{
				if(num1[i] == '1')
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
		}
	}
}

/*End arithmetic functions*/


/*Start conversion functions*/

char *hex_to_bin(char *num)
{
	return "hex_to_bin";
}

char *dec_to_bin(long int num)
{
	char *result = calloc(34, sizeof(char));
	struct Stack *s = create_stack(NULL);
	int i = 0;
	int mod;

	if(num < 0)
	{
		result[0] = '-';
		i = 1;
		num = -num;
	}

	if(num == 0)
	{
		result[i] = '0';
	}

	while(num > 0)
	{
		mod = num % 2;
		push(s, create_node((char)(((int)'0')+mod)));
		num /= 2;
	}

	while(s->size > 0)
	{
		result[i] = pop(s)->data;
		i++;
	}
	result[i] = '\0';

	return result;
}

char *oct_to_bin(char *num)
{
	char *result = calloc((strlen(num)*3)+1, sizeof(char));
	int oct_ind = strlen(num)-1;
	int bin_ind;

	for(bin_ind = strlen(num)*3; bin_ind >= 0; bin_ind -= 3)
	{
		switch(num[oct_ind])
		{
			case '0':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				break;
			case '1':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				break;
			case '2':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				break;
			case '3':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				break;
			case '4':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				break;
			case '5':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				break;
			case '6':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				break;
			case '7':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				break;
			case '-':
				result[bin_ind] = '-';
				break;
			default:
				fprintf(stderr, "ERROR: Bad octal char.\n");
		}
	}

	return result;
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

	free(quad);

	return result;
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

	free(triplet);

	return result;
}

long int bin_to_dec(char *num)
{
	long int result = 0;
	long int two_pow = 1;
	int i;

	/*Iterate through num and add to result*/
	for(i = strlen(num) - 1; i >= 1; i--)
	{
		if(num[i] == '1')
		{
			result += two_pow;
		}
		two_pow *= 2;
	}

	if(num[0] == '-')
	{
		result = -result;
	}

	return result;

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


/*Implementation of strdup, ANSI does not allow inclusion*/
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
