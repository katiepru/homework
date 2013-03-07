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
			nums[k] = dec_to_bin(ascii_to_int(argv[k+2]));
		}
		else
		{
			fprintf(stderr, "ERROR: Unrecognized format\n");
			return 3;
		}
	}


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
		result = multiply(nums[0], nums[1]);
	}

	c = argv[4][0];
	switch (c)
	{
		case 'b':
			result = strip_zeroes(result);
			break;
		case 'o':
			result = bin_to_oct(result);
			break;
		case 'x':
			result = bin_to_hex(result);
			break;
		case 'd':
			if(result[0] == '-')
			{
				result = int_to_ascii(bin_to_dec(result), 1);
			}
			else
			{
				result = int_to_ascii(bin_to_dec(result), 0);
			}
			break;
		default:
			fprintf(stderr, "Invalid output base.\n");
			return 3;
	}

	printf("%s\n", result);
	free(result);

	return 0;
}

/*Start arithmetic functions*/

/*-----------------------------------------------------------------------------/
/ -Wrapper function to call add or subtract depending on negativity of params -/
/ ----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------/
/ -Adds two binary numbers. Must have same signs. -----------------------------/
/ ----------------------------------------------------------------------------*/
char *add(char *num1, char *num2)
{
	char *res = calloc(MAX(strlen(num1), strlen(num2))+1,
		sizeof(char));
	char *result;
	int ind = 0;
	int curr_carry = 0;
	int prev_carry = 0;
	char *max;
	char *min;
	int max_ind;
	int min_ind;
	int res_ind = MAX(strlen(num1), strlen(num2))+1;
	int i;
	int is_neg = 0;

	if(num1[0] == '-')
	{
		is_neg = 1;
		for(i = 0; i <= strlen(num1); i++)
		{
			num1[i] = num1[i + 1];
		}
		for(i = 0; i <= strlen(num2); i++)
		{
			num2[i] = num2[i + 1];
		}
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

	res = strip_zeroes(res);

	if(is_neg)
	{
		result = calloc(strlen(res)+2, sizeof(char));
		result[0] = '-';
		for(i = 0; i < strlen(res); i++)
		{
			result[i+1] = res[i];
		}
		free(res);
	}
	else
	{
		result = res;
	}
	return result;
}

/*-----------------------------------------------------------------------------/
/ -Subtracts num2 from num1. Both are positive. -------------------------------/
/-----------------------------------------------------------------------------*/
char *subtract(char *num1, char *num2)
{
	/*Find out which number is bigger*/
	char *result;
	char *neg;
	int bigger;
	int i;

	bigger = is_bigger(num1, num2);
	if(bigger == 1)
	{
		neg = subtract(num2, num1);
		result = calloc(strlen(neg)+2, sizeof(char));
		result[0] = '-';
		for(i = 0; i < strlen(neg); i++)
		{
			result[i+1] = neg[i];
		}
		return result;
	}

	/*Convert second term to twos complement and add*/
	num2 = to_twos_comp(num2, strlen(num1));
	result = add(num1, num2);

	/*Remove MSB*/
	for(i = 0; i < strlen(result); i++)
	{
		if(result[i] == '1')
		{
			result[i] = '0';
			break;
		}
	}

	result = strip_zeroes(result);

	return result;
}

/*-----------------------------------------------------------------------------/
/ -Multiplies two binary numbers of any sign. ---------------------------------/
/-----------------------------------------------------------------------------*/
char *multiply(char *num1, char *num2)
{
	char *result;
	char *res;
	char *conv_num1;
	char *conv_num2;
	int is_neg = 0;
	int i;
	int free_1 = 0;
	int free_2 = 0;

	if((num1[0] == '-' && num2[0] != '-') || (num1[0] != '-' && num2[0] == '-'))
	{
		is_neg = 1;
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

	printf("num1 = %s, num2 = %s\n", conv_num1, conv_num2);

	result = strdup(conv_num1);
	conv_num2 = subtract(conv_num2, "1");
	while(bin_to_dec(conv_num2) > 0)
	{
		result = add(result, conv_num1);
		conv_num2 = subtract(conv_num2, "1");
	}
	
	if(free_1)
	{
		free(conv_num1);
	}
	if(free_2)
	{
		free(conv_num2);
	}
	result = strip_zeroes(result);

	printf("zeroes = %s\n", result);

	if(is_neg)
	{
		res = calloc(strlen(result)+2, sizeof(char));
		res[0] = '-';
		for(i = 0; i < strlen(result); i++)
		{
			res[i+1] = result[i];
		}
		free(result);
	}
	else
	{
		res = result;
	}
	return res;
}

/*-----------------------------------------------------------------------------/
/ -Converts a binary number to its twos complement representation -------------/
/-----------------------------------------------------------------------------*/
char *to_twos_comp(char *num, int len)
{
	char *result = calloc(len+1, sizeof(char));
	int one_ind;
	int res_ind;
	int num_ind;

	/*Find index of first 1*/
	for(one_ind = strlen(num) - 1; one_ind >= 0; one_ind --)
	{
		if(num[one_ind] == '1')
		{
			break;
		}
	}

	num_ind = strlen(num) - 1;
	for(res_ind = len -1; res_ind >= len-one_ind-1; res_ind--)
	{
		result[res_ind] = num[num_ind];
		num_ind--;
	}

	while(num_ind >= 0)
	{
		if(num[num_ind] == '1')
		{
			result[res_ind] = '0';
		}
		else
		{
			result[res_ind] = '1';
		}
		res_ind--;
		num_ind--;
	}

	while(res_ind >= 0)
	{
		result[res_ind] = '1';
		res_ind--;
	}

	return result;

}

/*-----------------------------------------------------------------------------/
/ -Determines which binary number is larger. Returns 0 if the first is larger -/
/ -and 1 if the second is larger. ---------------------------------------------/
/-----------------------------------------------------------------------------*/
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
	/*They are the same*/
	return 0;
}

/*End arithmetic functions*/


/*Start conversion functions*/

/*-----------------------------------------------------------------------------/
/ -Converts a hex string to a binary string -----------------------------------/
/-----------------------------------------------------------------------------*/
char *hex_to_bin(char *num)
{
	char *result = calloc((strlen(num)*4)+1, sizeof(char));
	int hex_ind = strlen(num)-1;
	int bin_ind;

	for(bin_ind = strlen(num)*4; bin_ind >= 0; bin_ind -= 4)
	{
		switch(num[hex_ind])
		{
			case '0':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '0';
				break;
			case '1':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '0';
				break;
			case '2':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '0';
				break;
			case '3':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '0';
				break;
			case '4':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '0';
				break;
			case '5':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '0';
				break;
			case '6':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '0';
				break;
			case '7':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '0';
				break;
			case '8':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '1';
				break;
			case '9':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '1';
				break;
			case 'a':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '1';
				break;
			case 'b':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '0';
				result[bin_ind-3] = '1';
				break;
			case 'c':
				result[bin_ind] = '0';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '1';
				break;
			case 'd':
				result[bin_ind] = '1';
				result[bin_ind-1] = '0';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '1';
				break;
			case 'e':
				result[bin_ind] = '0';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '1';
				break;
			case 'f':
				result[bin_ind] = '1';
				result[bin_ind-1] = '1';
				result[bin_ind-2] = '1';
				result[bin_ind-3] = '1';
				break;
			case '-':
				result[bin_ind] = '-';
				break;
		}
		hex_ind--;
	}

	result = strip_zeroes(result);

	return result;
}

/*-----------------------------------------------------------------------------/
/ -Converts a long int to a binary string. ------------------------------------/
/-----------------------------------------------------------------------------*/
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

	result = strip_zeroes(result);

	return result;
}

/*-----------------------------------------------------------------------------/
/ -Converts an octal string to a binary string. -------------------------------/
/-----------------------------------------------------------------------------*/
char *oct_to_bin(char *num)
{
	char *result = calloc(((strlen(num)-1)*3)+1, sizeof(char));
	int oct_ind = strlen(num)-1;
	int bin_ind;

	for(bin_ind = (strlen(num)-1)*3; bin_ind >= 2; bin_ind -= 3)
	{
		if(num[oct_ind] == 'o')
		{
			oct_ind--;
		}
		printf("num is %c\n", num[oct_ind]);
		printf("bin ind = %d\n", bin_ind);
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
		oct_ind--;
	}

	if(oct_ind == 0)
	{
		result[0] = '0';
	}

	result = strip_zeroes(result);

	return result;
}

/*-----------------------------------------------------------------------------/
/ -Converts a binary string to a hex string. ----------------------------------/
/-----------------------------------------------------------------------------*/
char *bin_to_hex(char *num)
{
	char *result = calloc((strlen(num)/4)+1, sizeof(char));
	char *quad = calloc(4, sizeof(char));
	int bin_ind = strlen(num) - 1;
	int res_ind = (strlen(num)/4)-1;

	printf("num is %s\n", num);
	while(bin_ind >= 3)
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
		res_ind--;
		bin_ind -= 4;
	}

	free(quad);

	printf("bin ind = %d\n", bin_ind);

	if(bin_ind == 2)
	{
		if(result[0] == '-')
		{
			if(num[bin_ind] == '0' && num[bin_ind-1] == '0')
			{
				result[res_ind] = '0';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '0')
			{
				result[res_ind] = '2';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '1')
			{
				result[res_ind] = '1';
			}
			else
			{
				result[res_ind] = '3';
			}
		}
		else
		{
			if(num[bin_ind] == '0' && num[bin_ind-1] == '0' && 
				num[bin_ind-2] == '0')
			{
				result[res_ind] = '0';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '0'
				&& num[bin_ind-2] == '1')
			{
				result[res_ind] = '1';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '1'
				&& num[bin_ind-2] == '0')
			{
				result[res_ind] = '2';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '1'
				&& num[bin_ind-2] == '1')
			{
				result[res_ind] = '3';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '0'
				&& num[bin_ind-2] == '0')
			{
				result[res_ind] = '4';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '0'
			&& num[bin_ind-2] == '1')
			{
				result[res_ind] = '5';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '1'
			&& num[bin_ind-2] == '0')
			{
				result[res_ind] = '6';
			}
			else
			{
				result[res_ind] = '7';
			}
		}
	}
	else if(bin_ind == 1)
	{
		if(result[0] == '-')
		{
			result[res_ind] = num[bin_ind];
		}
		else
		{
			if(num[bin_ind] == '0' && num[bin_ind-1] == '0')
			{
				result[res_ind] = '0';
			}
			else if(num[bin_ind] == '1' && num[bin_ind-1] == '0')
			{
				result[res_ind] = '2';
			}
			else if(num[bin_ind] == '0' && num[bin_ind-1] == '1')
			{
				result[res_ind] = '1';
			}
			else
			{
				result[res_ind] = '3';
			}
		}
	}
	else if(bin_ind == '0')
	{
		result[res_ind] = num[bin_ind];
	}

	result = strip_zeroes(result);

	return result;
}

/*-----------------------------------------------------------------------------/
/ -Converts a binary string to an octal string. -------------------------------/
/-----------------------------------------------------------------------------*/
char *bin_to_oct(char *num)
{
	char *result = calloc((strlen(num)/3)+1, sizeof(char));
	char *triplet = calloc(3, sizeof(char));
	int bin_ind = strlen(num)-1;
	int res_ind = strlen(num)/3;

	while(bin_ind >= 2)
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
	if(bin_ind == 1)
	{
		if(num[0] == '-')
		{
			if(num[1] == '0')
			{
				result[res_ind] = '0';
			}
			else
			{
				result[res_ind] = '1';
			}
		}
		else
		{
			if(num[0] == '0' && num[1] == '0')
			{
				result[res_ind] = '0';
			}
			else if(num[0] == '0' && num[1] == '1')
			{
				result[res_ind] = '1';
			}
			else if(num[0] == '1' && num[1] == '0')
			{
				result[res_ind] = '2';
			}
			else
			{
				result[res_ind] = '3';
			}
			res_ind--;
		}
	}

	if(bin_ind == 0)
	{
		result[res_ind] = num[0];
	}

	while(res_ind >= 0)
	{
		result[res_ind] = '0';
		res_ind--;
	}

	free(triplet);

	result = strip_zeroes(result);

	return result;
}

/*-----------------------------------------------------------------------------/
/ -Converts a binary string to a long int. ------------------------------------/
/-----------------------------------------------------------------------------*/
long int bin_to_dec(char *num)
{
	long int result = 0;
	long int two_pow = 1;
	int i;

	/*Iterate through num and add to result*/
	for(i = strlen(num) - 1; i >= 0; i--)
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

/*-----------------------------------------------------------------------------/
/ -Converts a decimal string to its long int respresentation. -----------------/
/-----------------------------------------------------------------------------*/
long int ascii_to_int(char* num)
{
	long int result = 0;
	int is_neg = 0;
	int ten_pow = 1;
	int i;
	int dig;

	if(num[0] == '-')
	{
		is_neg = 1;
	}

	for(i = strlen(num)-1; i > 0; i--)
	{
		if(num[i] == 'd')
		{
			break;
		}
		dig = num[i] - '0';
		result += (ten_pow * dig);
		ten_pow *= 10;
	}

	if(is_neg)
	{
		result = -result;
	}
	return result;
}

/*-----------------------------------------------------------------------------/
/ -Converts a long int to its string representation. --------------------------/
/-----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------/
/ -Strips leading zeroes from number. -----------------------------------------/
/-----------------------------------------------------------------------------*/
char *strip_zeroes(char *num)
{
	char *res;
	int i;
	int res_ind = 0;
	int num_zeroes = 0;

	for(i = 0; i < strlen(num); i++)
	{
		if(num[i] != '0')
			break;
		num_zeroes++;
	}

	if(num_zeroes == 0)
	{
		return num;
	}

	if(num_zeroes == strlen(num))
	{
		return "0";
	}
	res = calloc(strlen(num)+1-num_zeroes, sizeof(char));
	for(i = num_zeroes; i<strlen(num); i++)
	{
		res[res_ind] = num[i];
		res_ind++;
	}
	free(num);

	return res;
}


/*End conversion functions*/


/*Start Stack Functions*/

/*----------------------------------------------------------------------------/
/ -Creates a character node. -------------------------------------------------/
/----------------------------------------------------------------------------*/
struct Node *create_node(char data)
{
	struct Node *n = malloc(sizeof(struct Node));
	n->data = data;
	n->next = NULL;
	return n;
}

/*-----------------------------------------------------------------------------/
/ -Creates a stack of nodes. --------------------------------------------------/
/-----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------/
/ -Frees a stack and all its nodes. -------------------------------------------/
/-----------------------------------------------------------------------------*/
void destroy_stack(struct Stack *s)
{
	while(peek(s) != NULL)
	{
		free(pop(s));
	}
	free(s);
}

/*-----------------------------------------------------------------------------/
/ -Pops a node off of a stack. ------------------------------------------------/
/-----------------------------------------------------------------------------*/
struct Node *pop(struct Stack *s)
{
	struct Node *res;
	if(peek(s) == NULL) return NULL;
	res = s->head;
	s->head = s->head->next;
	s->size--;
	return res;
}

/*-----------------------------------------------------------------------------/
/ -Looks at top of stack without popping. -------------------------------------/
/-----------------------------------------------------------------------------*/
struct Node *peek(struct Stack *s)
{
	return s->head;
}

/*-----------------------------------------------------------------------------/
/ -Pushes a node onto the stack. ----------------------------------------------/
/-----------------------------------------------------------------------------*/
void push(struct Stack *s, struct Node *node)
{
	node->next = s->head;
	s->head = node;
	s->size++;
	return;
}

/*End Stack Functions*/

/*-----------------------------------------------------------------------------/
/ -Implementation of strdup, ANSI does not allow inclusion. -------------------/
/-----------------------------------------------------------------------------*/
char *strdup(char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}
