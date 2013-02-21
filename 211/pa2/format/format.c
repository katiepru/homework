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
	}
	else
	{
		fprintf(stderr, "Cannot parse type %s. Try int or float\n", argv[2]);
		return 2;
	}
	return 0;
}

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

	if(is_neg)
	{
		result = -result;
	}

	printf("%d\n", result);
	return;
}
