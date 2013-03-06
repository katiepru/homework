#include "format.h"

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
		return 1;
	}

	if(strcmp(argv[2], "int") == 0)
	{
		print_int(argv[1]);
		return 0;
	}
	else if(strcmp(argv[2], "float") == 0)
	{
		print_float(argv[1]);
		return 0;
	}
	else
	{
		fprintf(stderr, "Cannot parse type %s. Try int or float\n", argv[2]);
		return 2;
	}
}

/*Prints an int from twos complement*/
void print_int(char *num)
{
	int i;
	int is_neg=0;
	int pow = 1;
	long int result = 0;
	if(num[0]=='1')
	{
		is_neg=1;

		/*Flip bits*/
		for(i=0; i<strlen(num); i++)
		{
			if(num[i]=='1')
			{
				num[i]='0';
			}
			else
			{
				num[i]='1';
			}
		}
		/*Add 1*/
		for(i=strlen(num)-1; i>=0; i--)
		{
			if(num[i]=='0')
			{
				num[i]='1';
				break;
			}
		}
	}

	/*Convert binary to decimal*/
	for(i=strlen(num)-1; i>=0; i--)
	{
		if(num[i] == '1')
		{
			result+=pow;
		}
		pow *= 2;
	}
	printf("%s\n", int_to_ascii(result, is_neg));
	return;
}

char *int_to_ascii(long int num, int is_neg)
{
	char *result = calloc(12, sizeof(char));
	int digit;
	struct Stack *s = create_stack(NULL);
	struct Node *tmp;
	
	if(is_neg == 1)
	{
		result[0] = '-';
	}

	while(num > 0)
	{
		digit = num%10;
		push(s, create_node((char)(((int)'0')+digit)));
		num -= digit;
		num /= 10;
	}

	while(peek(s) != NULL)
	{
		tmp = pop(s);
		result[is_neg] = tmp->data;
		is_neg++;
		free(tmp);
	}

	if((strlen(result) == 0) || (strlen(result) == 1 && result[0] == '-'))
	{
		result[0] = '0';
	}
	return result;
}


void print_float(char *num)
{
	int is_neg = 0;
	int exp = 0;
	float dec = 0;
	int i;
	int two_pow = 1;
	float half_pow = 1;

	if(num[0] == '1')
	{
		is_neg = 1;
	}

	for(i = 8; i >= 1; i--)
	{
		if(num[i] == '1')
		{
			exp += two_pow;
		}
		two_pow *= 2;
	}

	exp -= 127;
	
	dec += half_pow;
	half_pow *= 0.5;

	for(i = 9; i < 32; i++)
	{
		if(num[i] == '1')
		{
			dec += half_pow;
		}

		half_pow *= 0.5;
	}

	dec = dec * power(2.0, exp);

	if(is_neg)
	{
		dec = -dec;
	}

	if(exp == 255-127)
	{
		if(dec == 0)
		{
			if(is_neg)
			{
				printf("ninf\n");
			}
			else
			{
				printf("pinf\n");
			}
		}
		else
		{
			printf("NaN\n");
		}
		return;
	}


	print_sci(dec);
}

void print_sci(float num)
{
	int is_neg = 0;
	int exp_is_neg = 0;
	int exp = 0;
	int precision = 0;
	float after_dec;

	if(num < 0)
	{
		is_neg = 1;
		num = -num;
	}
	while(num < 1 && precision < 7)
	{
		num *= 10;
		exp --;
		precision++;
	}
	while(num >= 10)
	{
		num /= 10;
		exp ++;
	}
	if(exp < 0)
	{
		exp_is_neg = 1;
		exp = -exp;
	}

	after_dec = num - (float)((int) num);

	printf("%s.%se%s\n", int_to_ascii((int) num, is_neg),
		float_to_ascii(after_dec), 	int_to_ascii(exp, exp_is_neg));
	return;
}

char *float_to_ascii(float num)
{
	int tmp_num;

	tmp_num = (int) (num*power(10, 7));
	if(tmp_num == 0)
	{
		return(int_to_ascii(tmp_num, 0));
	}
	while(tmp_num%10 == 0)
	{
		tmp_num /= 10;
	}
	
	return int_to_ascii(tmp_num, 0);
}

float power(float base, int exp)
{
	int i;
	float res = 1;

	if(exp == 0)
	{
		return 1.0;
	}
	else if(exp > 0)
	{
		for(i = 0; i < exp; i++)
		{
			res *= base;
		}
	}
	else
	{
		 base = 1/base;
		 for(i = 0; i > exp; i--)
		 {
			 res *= base;
		 }
	}
	return res;
}

/*Begin Stack Functions*/

struct Node *create_node(char data)
{
	struct Node *n = malloc(sizeof(struct Node));
	n->data = data;
	n->next = NULL;
	return n;
}

struct Stack *create_stack(struct Node *head)
{
	struct Stack *s = malloc(sizeof(struct Stack));
	s->head = head;
	if(head != NULL)
	{
		s->size = 1;
	}
	else
	{
		s->size = 0;
	}
	return s;
}

void destroy_stack(struct Stack *s)
{
	struct Node *tmp;
	while(peek(s) != NULL)
	{
		tmp = pop(s);
		free(tmp);
	}
	free(s);
}

void push(struct Stack *s, struct Node *n)
{
	n->next = s->head;
	s->head = n;
	return;
}

struct Node *pop(struct Stack *s)
{
	struct Node *res = s->head;
	assert(res != NULL);
	s->head = s->head->next;
	s->size--;
	return res;
}

struct Node *peek(struct Stack *s)
{
	return s->head;
}

/*End Stack functions*/
