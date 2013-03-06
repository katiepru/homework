#include "calc.h"

int main(int argc, char *argv[])
{
	char *result;
	long int res;
	char c;
	int i;
	long int nums[2];
	
	/*Check for correct number of args*/
	if(argc != 5)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.\n");
		return 2;
	}

	/*Convert input numbers to decimal*/
	for(i = 0; i < 2; i++)
	{
		c = argv[i + 2][0];
		if(c == '-')
		{
			c = argv[i + 2][1];
		}

		/*Check input format and convert everything to decimal*/
		switch(c)
		{
			case 'd':
				nums[i] = str_to_dec(argv[i + 2]);
				break;
			case 'b':
				nums[i] = bin_to_dec(argv[i + 2]);
				break;
			case 'o':
				nums[i] = oct_to_dec(argv[i + 2]);
				break;
			case 'x':
				nums[i] = hex_to_dec(argv[i + 2]);
			default:
				fprintf(stderr, "ERROR: Unrecognized format.\n");
				return 3;
		}
	}

	c = argv[1][0];

	/*Check operand*/
	switch(c)
	{
		case '+':
			res = nums[0] + nums[1];
			break;
		case '-':
			res = nums[0] + nums[1];
			break;
		case '*':
			res = nums[0] + nums[1];
			break;
		default:
			fprintf(stderr, "ERROR: Unrecognized operation.\n");
			return 4;
	}

	c = argv[5][0];

	/*Check output format and convert to string*/
	switch(c)
	{
		case 'd':
			result = dec_to_str(res);
			break;
		case 'b':
			result = dec_to_bin(res);
			break;
		case 'o':
			result = dec_to_oct(res);
			break;
		case 'x':
			result = dec_to_hex(res);
			break;
		default:
			fprintf(stderr, "ERROR: Unrecognized output format.\n");
	}

	printf("%s\n", result);

	return 0;
}

/*Converts string representation of binary number to decimal int*/
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

long int oct_to_dec(char *num)
{
	long int result = 0;
	long int eight_pow = 1;
	int dig;
	int i;

	for(i = strlen(num) - 1; i >= 1; i--)
	{
		if(num[i] != 'o')
		{
			dig = (char)(((int)'0')+num[i]);
			result += dig * eight_pow;
		}
	}

	if(num[0] == '-')
	{
		result = -result;
	}

	return result;
}

long int hex_to_dec(char *num)
{
	long int result = 0;
	long int sixteen_pow = 1;
	int dig;
	int i;

	for(i = strlen(num) -1 ; i >= 1; i--)
	{
		if(num[i] != 'x')
		{

		}
	}
}

/*Converts a decimal int to binary string*/
char *dec_to_bin(long int num)
{
	char *result = calloc(35, sizeof(char));
	struct Stack *s = create_stack(NULL);
	int i = 0;
	int mod;

	if(num < 0)
	{
		result[0] = '-';
		i = 1;
		num = -num;
	}

	result[i] = 'b';
	i++;

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

char *dec_to_oct(long int num)
{
	char *result = calloc(13, sizeof(char));
	struct Stack *s = create_stack(NULL);
	int mod;
	int i = 0;

	if(num < 0)
	{
		result[0] = '-';
		i = 1;
		num = -num;
	}

	result[i] = 'o';
	i++;

	if(num == 0)
	{
		result[i] = '0';
		i++;
	}

	while(num > 0)
	{
		mod = num % 8;
		push(s, create_node((char)(((int)'0')+mod)));
		num /= 8;
	}

	while(s->size > 0)
	{
		result[i] = pop(s)->data;
		i++;
	}
	result[i] = '\0';
	
	return result;
}

char *dec_to_hex(long int num)
{
	char *result = calloc(10, sizeof(char));
	struct Stack *s = create_stack(NULL);
	int mod;
	int i = 0;

	if(num < 0)
	{
		result[0] = '-';
		i = 1;
		num = -num;
	}

	result[i] = 'x';
	i++;

	if(num == 0)
	{
		result[i] = '0';
		i++;
	}

	while(num > 0)
	{
		mod = num % 16;
		if(mod > 9)
		{
			push(s, create_node(get_hex_value(mod)));
		}
		else
		{
			push(s, create_node((char)(((int)'0'+mod))));
		}
		num /= 16;
	}

	while(s->size > 0)
	{
		result[i] = pop(s)->data;
		i++;
	}
	result[i] = '\0';

	return result;
}

char get_hex_value(int mod)
{
	switch(mod)
	{
		case 10:
			return 'a';
		case 11:
			return 'b';
		case 12:
			return 'c';
		case 13:
			return 'd';
		case 14:
			return 'e';
		case 15:
			return 'f';
		default:
			fprintf(stderr, "ERROR: Bad hex value\n");
			return '\0';
	}
}
