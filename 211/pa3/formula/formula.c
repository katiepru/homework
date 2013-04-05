#include "formula.h"

int main(int argc, char *argv[])
{
	int num;
	struct timeval *start, *end;
	start = malloc(sizeof(struct timeval));
	end = malloc(sizeof(struct timeval));

	gettimeofday(start, NULL);

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

	gettimeofday(end, NULL);

	printf("Time required: %d microseconds.\n", (int)end->tv_usec - (int)start->tv_usec);

	return 0;
}

void binomial(int num)
{
	int ncr;
	int i;

	if(num == 0)
	{
		printf("1\n");
		return;
	}


	for(i = 1; i < num + 1; i++)
	{
		ncr = nCr(num, i);
		if(ncr == 0)
		{
			fprintf(stderr, "ERROR: Overflow.\n");
			return;
		}
		if(i == num)
		{
			if(num != 1)
			{
				printf("%dx^%d\n", ncr, i);
			}
			else
			{
				printf("1 + ");
				printf("%dx\n", ncr);
			}
		}
		else if(i == 1)
		{
			printf("1 + ");
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
