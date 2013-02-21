#include "calc.h"

int main(int argc, char *argv[])
{
	char *num1;
	char *num2;

	if(argc != 5)
	{
		fprintf(stderr, "ERROR: Wrong number of arguments.");
		return 1;
	}

	num1 = to_binary(argv[2]);
	num2 = to_binary(argv[3]);

	printf("num1= %s; num2=%s\n", num1, num2);

	return 0;
}

char *to_binary(char *input)
{
	int is_neg=0;
	int i = 0;
	char *output;
	if(input[0] == '-')
	{
		is_neg=1;
		i = 1;
	}
	if(input[i] == 'b')
	{
		output = calloc(strlen(input)-1, sizeof(char));
		if(is_neg)
		{
			output[0] = '-';
		}
		for(i=i+1; i<strlen(input); i++)
		{
			output[i-1] = input[1];
		}
	}

	return output;
}
