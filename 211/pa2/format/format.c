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
	int result = 0;
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


	printf("%d\n", result);
	int_to_ascii(result, is_neg);
	return;
}

void int_to_ascii(int num, int is_neg)
{
	char *result = calloc(12, sizeof(char));
	int ind = 0;
	int i;
	double conv_num = (double)num;
	double div = 1000000000;
	int tmp;
	if(is_neg)
	{
		result[0] = '-';
		ind++;
	}

	for(i = ind; i < 12; i++)
	{
		tmp = floor(conv_num/div);
		if(tmp > 0)
		{
			result[ind] = (char)(((int)'0')+tmp);
			ind++;
			conv_num = conv_num - (tmp * div);
		}
		div = div/10;
	}
	printf("converted int is %s", result);
	return;
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
	print_sci(dec);
}

void print_sci(float num)
{
	int is_neg = 0;
	int exp = 0;

	if(num == 0 || num == -0)
	{
		printf("%fe0\n", num);
		return;
	}
	if(num < 0)
	{
		is_neg = 1;
		num = -num;
	}
	while(num < 1)
	{
		num *= 10;
		exp --;
	}
	while(num >= 10)
	{
		num /= 10;
		exp ++;
	}

	if(is_neg)
	{
		num = -num;
	}

	printf("%fe%d\n", num, exp);
	return;
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
