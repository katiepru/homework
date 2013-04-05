#include "formula.h"

int main(int argc, char *argv[])
{
	int num;

	if(argc < 2)
	{
		fprintf(stderr, "ERROR: Too few arguments.\n");
		print_help();
		return 1;
	}
	if(argc > 2)
	{
		fprintf(stderr, "ERROR: Too many arguments.\n");
		print_help();
		return 1;
	}
	if(strcmp(argv[1], "-h") == 0)
	{
		print_help();
		return 0;
	}

	num = atoi(argv[1]);
	if(num < 0)
	{
		fprintf(stderr, "ERROR: Negative integer detected.\n");
		print_help();
		return 2;
	}

	binomial(num);

	return 0;
}

void binomial(int num)
{
	int ncr;
	int i;

	printf("1 + ");

	for(i = 1; i < num + 1; i++)
	{
		ncr = nCr(num, i);
		if(ncr == 0)
		{
			fprintf(stderr, "ERROR: Overflow.\n");
			break;
		}
		if(i == num)
		{
			printf("%dx^%d\n", ncr, i);
		}
		else if(i == 1)
		{
			printf("%dx + ", ncr);
		}
		else
		{
			printf("%dx^%d + ", ncr, i);
		}
	}
}

void print_help()
{
	printf("Usage: formula <positive integer>\n");
}
